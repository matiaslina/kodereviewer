#ifndef PRLISTMODEL_H
#define PRLISTMODEL_H

#include <QAbstractListModel>
#include <QJsonDocument>
#include <qjsonobject.h>
#include "apidata.h"

class PullRequestModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PrListRoles {
        NumberRole = Qt::UserRole + 1,
        TitleRole,
        DescriptionRole,
        StateRole,
        UsernameRole,
        CreatedAtRole,
        UpdatedAtRole
    };

    explicit PullRequestModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;
    QMap<int, QVariant> itemData(const QModelIndex &index) const override;

    QJsonObject getPrData(const QModelIndex &index) const;

public slots:
    void loadData(QByteArray data);
    QList<PullRequest*> getPullRequests();

private:
    std::vector<PullRequest*> pullRequests;
};

#endif
