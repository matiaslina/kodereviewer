#include "diffmodel.h"
#include "qgit2/qgitdiffhunk.h"

DiffModel::DiffModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int DiffModel::rowCount(const QModelIndex &) const
{
    return diffs.size();
}

QVariant DiffModel::data(const QModelIndex &index, int role) const
{
    LibQGit2::DiffHunk hunk = diffs[index.row()];

    switch (role) {
    case HeaderRole:
        return hunk.header();
    case OldStartRole:
        return hunk.oldStart();
    case OldLinesRole:
        return hunk.oldLines();
    case NewStartRole:
        return hunk.newStart();
    case NewLinesRole:
        return hunk.newLines();
    case HeaderLengthRole:
        return (int) hunk.headerLength();
    }
    return QVariant();
}

QHash<int, QByteArray> DiffModel::roleNames() const
{
    return {
        {HeaderRole, "header"},
        {OldStartRole, "oldStart"},
        {OldLinesRole, "oldLines"},
        {NewStartRole, "newStart"},
        {NewLinesRole, "newLines"},
        {HeaderLengthRole, "headerLength"}
    };
}

void DiffModel::insertDiffHunk(LibQGit2::DiffHunk &hunk)
{
    beginInsertRows(QModelIndex(), 0, 0);
    diffs.push_back(hunk);
    endInsertRows();
}
