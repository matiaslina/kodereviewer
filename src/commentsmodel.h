#ifndef COMMENTSMODEL_H
#define COMMENTSMODEL_H

#include <QAbstractListModel>
#include <QJsonDocument>
#include <qabstractitemmodel.h>
#include <qqml.h>
#include <qtmetamacros.h>
#include "apidata.h"

class CommentModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CommentRoles {
        IdRole = Qt::UserRole + 1,
        NodeIdRole,
        CreatedAtRole,
        UpdatedAtRole,
        DescriptionRole,
        UsernameRole
    };

    explicit CommentModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

public slots:
    void loadData(QByteArray data);

private:
    std::vector<Comment*> comments;
};

#endif
