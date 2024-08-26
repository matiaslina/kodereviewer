#include "reviewthreadmodel.h"
#include <qabstractitemmodel.h>
#include "apidata.h"

ReviewThreadModel::ReviewThreadModel(QObject *parent)
    : QAbstractListModel(parent)
    , _thread(nullptr)
{}

ReviewThreadModel::ReviewThreadModel(ReviewThread *thread, QObject *parent)
    : QAbstractListModel(parent)
    , _thread(thread)
{}

int ReviewThreadModel::rowCount(const QModelIndex &) const
{
    if (_thread != nullptr) {
        return _thread->comments().size();
    }
    return 0;
}

QVariant ReviewThreadModel::data(const QModelIndex &index, int role) const
{
    auto comment = _thread->comments()[index.row()];
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
        return comment->avatarUrl();
    case Qt::DisplayRole:
        return comment->description();

    }
    return QVariant();
}

QHash<int, QByteArray> ReviewThreadModel::roleNames() const
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


// Slots

int ReviewThreadModel::getThreadId() const
{
    if (_thread->comments().size() > 0) {
        return _thread->comments().at(0)->id();
    }

    return 0;
}
