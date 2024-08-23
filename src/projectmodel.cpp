#include "projectmodel.h"
#include <qabstractitemmodel.h>
#include <qnamespace.h>
#include <QStandardPaths>
#include <QFile>
#include <QJsonObject>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

Project::Project(QObject *parent)
    : QObject(parent)
{}

Project::Project(QString name, QString owner, QUrl url, QUrl path, QObject *parent)
    : QObject(parent)
    , _name(name)
    , _owner(owner)
    , _url(url)
    , _path(path)
{
}
QString Project::name() const { return _name; }
QString Project::owner() const { return _owner; }
QUrl Project::url() const { return _url; }
QUrl Project::path() const { return _path; }

void Project::setName(QString name) { _name = name; }
void Project::setOwner(QString owner) { _owner = owner; }
void Project::setUrl(QUrl url) { _url = url; }
void Project::setPath(QUrl path) { _path = path; }


ProjectModel::ProjectModel(QObject *parent)
    : QAbstractListModel(parent)
{
    auto appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    config = QUrl::fromLocalFile(appDataDir + "/projects.json");
    QFile file(config.toLocalFile());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonDocument document = QJsonDocument::fromJson(file.readAll());
        if(document.isArray()) {
            QJsonArray projects = document.array();
            loadProjects(projects);
        }
    }
}

void ProjectModel::loadProjects(QJsonArray &projects)
{
    for (auto value : projects) {
        QJsonObject data = value.toObject();
        Project *project = new Project(data["name"].toString(), data["owner"].toString(),
                                       QUrl(data["url"].toString()),
                                       QUrl(data["path"].toString()));
        this->projects.push_back(project);
    }
}

QVariant ProjectModel::data(const QModelIndex &index, int role) const
{
    Project *p = projects.at(index.row());

    switch (role) {
    case Roles::NameRole:
        return p->name();
    case Roles::OwnerRole:
        return p->owner();
    case Roles::UrlRole:
        return p->url();
    case Roles::PathRole:
        return p->path();
    case Qt::DisplayRole:
        return p->name();
    }

    return QVariant();
}

int ProjectModel::rowCount(const QModelIndex & /* parent */) const
{
    return projects.size();
}

QHash<int, QByteArray> ProjectModel::roleNames() const
{
    return {
        { Roles::NameRole, "name" },
        { Roles::OwnerRole, "owner" },
        { Roles::UrlRole, "url" },
        { Roles::PathRole, "path" }
    };
}

/************
 * slots
 ************/
void ProjectModel::saveProjects()
{
    auto localFile = config.toLocalFile();
    qDebug() << "Writing config into " << localFile;
    QFile file(localFile);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot write project config: " << file.errorString();
        return;
    }

    QJsonArray projectArray;

    for (Project *project : projects) {
        QJsonObject obj;
        obj["name"] = project->name();
        obj["owner"] = project->owner();
        obj["url"] = project->url().toString();
        obj["path"] = project->path().toLocalFile();
        projectArray << obj;
    }
    file.write(QJsonDocument(projectArray).toJson());
}

void ProjectModel::append(QString name, QUrl url, QString owner)
{
    beginInsertRows(QModelIndex(), 0, 0);

    projects.push_back(new Project(name, owner, url, QUrl("")));
    saveProjects();
    endInsertRows();
}

Project *ProjectModel::get(int index)
{
    return projects.at(index);
}
