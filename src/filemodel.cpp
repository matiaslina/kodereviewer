#include "filemodel.h"
#include <qabstractitemmodel.h>

#include <QJsonArray>
#include <QJsonObject>

FileModel::FileModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int FileModel::rowCount(const QModelIndex &) const
{
    return files.size();
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    File *f = files[index.row()];

    switch (role) {
    case ShaRole:
        return f->sha();
    case FilenameRole:
        return f->filename();
    case StatusRole:
        return f->status();
    case AdditionsRole:
        return f->additions();
    case DeletionsRole:
        return f->deletions();
    case ChangesRole:
        return f->changes();
    case PatchRole:
        return f->patch();
    case Qt::DisplayRole:
        return f->filename();
    }
    return QVariant();
}

QHash<int, QByteArray> FileModel::roleNames() const
{
    return {
        {ShaRole, "sha"},
        {FilenameRole, "filename"},
        {StatusRole, "status"},
        {AdditionsRole, "additions"},
        {DeletionsRole, "deletions"},
        {ChangesRole, "changes"},
        {PatchRole, "patch"}
    };
}


void FileModel::loadData(QByteArray data)
{
    beginResetModel();
    QJsonDocument json = QJsonDocument::fromJson(data);
    if (!json.isArray()) {
        qDebug() << "FileModel::loadData(): expecting array";
        goto clean;
    }

    files.clear();
    for (auto file : json.array()) {
        QJsonDocument document(file.toObject());
        files.push_back(new File(document));
    }

 clean:
    endResetModel();
}

File *FileModel::get(int index) const
{
    return files[index];
}
