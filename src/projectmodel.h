#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QAbstractItemModel>
#include <QQmlEngine>
#include <QJsonDocument>
#include <QJsonArray>


class Project : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString owner READ owner WRITE setOwner NOTIFY ownerChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QUrl path READ path WRITE setPath NOTIFY pathChanged)

    QML_ELEMENT
public:
    Project(QObject *parent = nullptr);
    Project(QString name, QString owner, QUrl url, QUrl path, QObject *parent = nullptr);

    QString name() const;
    QString owner() const;
    QUrl url() const;
    QUrl path() const;

public slots:
    void setName(QString name);
    void setOwner(QString owner);
    void setUrl(QUrl url);
    void setPath(QUrl path);

signals:
    void nameChanged(QString name);
    void ownerChanged(QString owner);
    void urlChanged(QUrl url);
    void pathChanged(QUrl path);

private:

    QString _name;
    QString _owner;
    QUrl _url;
    QUrl _path;
};

class Project;

/**
 * @class ProjectModel
 *
 * A model to provide known projects
 */
class ProjectModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:

    enum Roles {
        NameRole = Qt::UserRole +1,
        OwnerRole,
        UrlRole,
        PathRole
    };
    Q_ENUM(Roles)

    explicit ProjectModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void saveProjects();
    void append(QString name, QUrl url, QString owner);
    [[nodiscard]] Project *get(int index);

private:
    QList<Project*> projects;
    QUrl config;

    void loadProjects(QJsonArray &projects);
};

#endif
