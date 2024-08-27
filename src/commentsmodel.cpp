#include "commentsmodel.h"
#include <qabstractitemmodel.h>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

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
    case UserImageRole:
        return comment->userImage();
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
        {UsernameRole, "username"},
        {UserImageRole, "userImage"}
    };
}

void CommentModel::loadData(QByteArray data)
{
    beginResetModel();
    auto json = QJsonDocument::fromJson(data);
    if (json.isArray()) {
        comments.clear();
        QJsonObject value;
        QJsonArray commentArray = json.array();
        for (auto comment : std::as_const(commentArray)) {
            value = comment.toObject();
            auto document = QJsonDocument(value);
            comments.push_back(new Comment(document));
        }
    }
    endResetModel();
}

void CommentModel::addComment(QByteArray data)
{
    beginInsertRows(QModelIndex(), comments.size(), comments.size());
    auto json = QJsonDocument::fromJson(data);
    comments.push_back(new Comment(json));
    endInsertRows();
}
