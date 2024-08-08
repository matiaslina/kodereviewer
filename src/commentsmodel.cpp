#include "commentsmodel.h"
#include <qabstractitemmodel.h>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

CommentModel::CommentModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int CommentModel::rowCount(const QModelIndex &) const
{
    return comments.size();
}

QVariant CommentModel::data(const QModelIndex &index, int role) const
{
    auto comment = comments[index.row()];
    switch (role) {
    case IdRole:
        return comment->id();
    case NodeIdRole:
        return comment->nodeId();
    case CreatedAtRole:
        return comment->createdAt();
    case UpdatedAtRole:
        return comment->updatedAt();
    case DescriptionRole:
        return comment->description();
    case UsernameRole:
        return comment->username();
    case Qt::DisplayRole:
        return comment->description();

    }
    return QVariant();
}

QHash<int, QByteArray> CommentModel::roleNames() const
{
    return {
        {IdRole, "id"},
        {NodeIdRole, "nodeId"},
        {CreatedAtRole, "createdAt"},
        {UpdatedAtRole, "updatedAt"},
        {DescriptionRole, "description"},
        {UsernameRole, "username"}
    };
}

void CommentModel::loadData(QByteArray data)
{
    beginResetModel();
    auto json = QJsonDocument::fromJson(data);
    if (!json.isArray()) {
        qDebug() << "CommentModel::loadData(): Expecting array";
        goto clean;
    }
    comments.clear();
    for (auto comment : json.array()) {
        auto document = QJsonDocument(comment.toObject());
        comments.push_back(new Comment(document));
    }
 clean:
    endResetModel();
}
