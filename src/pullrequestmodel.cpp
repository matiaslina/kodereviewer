#include "pullrequestmodel.h"

#include <QFile>
#include <QJsonArray>
#include <QUrl>
#include <qabstractitemmodel.h>
#include <qjsonobject.h>

PullRequestModel::PullRequestModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int PullRequestModel::rowCount(const QModelIndex& /*parent*/) const
{
    return pullRequests.size();
}

QVariant PullRequestModel::data(const QModelIndex& index, int role) const
{
    PullRequest *pr = pullRequests[index.row()];

    switch (role) {
    case NumberRole:
        return pr->number();
    case TitleRole:
        return pr->title();
    case DescriptionRole:
        return pr->description();
    case StateRole:
        if (pr->state() == PullRequest::State::OPEN) {
            return tr("Open");
        } else if (pr->state() == PullRequest::State::CLOSED) {
            return tr("Closed");
        }
        return tr("Unknown state");
    case UsernameRole:
        return pr->user()->username;
    case CreatedAtRole:
        return pr->createdAt();
    case UpdatedAtRole:
        return pr->updatedAt();
    case Qt::DisplayRole:
        return QVariant(
                        tr("#%1 - %2").arg(pr->number()).arg(pr->title()));
    }

    return QVariant();
}

QJsonObject PullRequestModel::getPrData(const QModelIndex& /* index */) const
{
    return {};
}

QHash<int, QByteArray> PullRequestModel::PullRequestModel::roleNames() const
{
    return {
        { NumberRole, "number" },
        { TitleRole, "title" },
        { DescriptionRole, "description" },
        { StateRole, "state" },
        { UsernameRole, "username" },
        {CreatedAtRole, "createdAt"},
        {UpdatedAtRole, "updatedAt"}
    };
}

QMap<int, QVariant> PullRequestModel::itemData(const QModelIndex &index) const
{
    auto pr = pullRequests[index.row()];
    return {
        { NumberRole, pr->number() },
        { TitleRole, pr->title() },
        { DescriptionRole, pr->description() },
        { StateRole, pr->state() },
        { UsernameRole, pr->user()->username },
        {CreatedAtRole, pr->createdAt()},
        {UpdatedAtRole, pr->updatedAt()}
    };
}


/// Slots
void PullRequestModel::loadData(QByteArray data)
{
    beginResetModel();
    auto json = QJsonDocument::fromJson(data);
    if (json.isArray()) {
        pullRequests.clear();

        auto arr = json.array();
        for (auto pr : std::as_const(arr)) {
            auto document = QJsonDocument(pr.toObject());
            pullRequests.push_back(new PullRequest(document));
        }
    }

    endResetModel();
}

QList<PullRequest*> PullRequestModel::getPullRequests()
{
    QList<PullRequest*> retval;
    for(auto pr : pullRequests) {
        retval.append(pr);
    }
    return retval;
}

PullRequest *PullRequestModel::get(int index)
{
    return pullRequests.at(index);
}
