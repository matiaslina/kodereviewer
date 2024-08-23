#ifndef DIFFMODEL_H
#define DIFFMODEL_H

#include <QQmlEngine>
#include <QAbstractListModel>
#include <qgit2/qgitdiffhunk.h>


class DiffModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum DiffRoles {
        HeaderRole = Qt::UserRole + 1,
        OldStartRole,
        OldLinesRole,
        NewStartRole,
        NewLinesRole,
        HeaderLengthRole
    };
    explicit DiffModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public:
    void insertDiffHunk(LibQGit2::DiffHunk &hunk);

private:
    std::vector<LibQGit2::DiffHunk> diffs;
};

#endif
