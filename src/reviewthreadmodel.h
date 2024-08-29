#ifndef REVIEWTHREADMODEL_H
#define REVIEWTHREADMODEL_H

#include <QAbstractListModel>
#include <qtmetamacros.h>

class ReviewThread;

class ReviewThreadModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ReviewThreadRoles {
        IdRole = Qt::UserRole + 1,
        NodeIdRole,
        CreatedAtRole,
        UpdatedAtRole,
        DescriptionRole,
        UsernameRole,
        UserImageRole
    };

    explicit ReviewThreadModel(QObject *parent = nullptr);
    ReviewThreadModel(ReviewThread *thread, QObject *parent = nullptr);

    int rowCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    /**
     * Gets the id of the first comment of the thread to post a reply
     * @returns the id of the first comment
     */
    Q_INVOKABLE int getThreadId() const;

    Q_INVOKABLE QString diffHunk() const;

    /**
     * Adds a comment after sending it to github
     */
    Q_INVOKABLE void addComment(QByteArray response);

    void updateModel(ReviewThread *thread);

private:
    ReviewThread *_thread;
};

#endif
