#ifndef TREE_H
#define TREE_H

#include "gitobject.h"
#include "commit.h"
#include <QObject>
#include <QString>
#include <git2.h>

class GitTreeEntry {
public:
    GitTreeEntry(GitTree* tree, int idx);
    GitTreeEntry(GitTree* tree, const QString &path);

    const git_oid* id() const;
    QString name() const;
    QString contents() const;
    GitObject *toObject() const;

private:
    const git_tree_entry* entry;
    GitTree* parent;
};

class GitTree {
public:
    GitTree();
    GitTree(git_tree* ptr);
    ~GitTree();

    git_repository *owner();
    size_t entryCount() const;
    QString id() const;
    GitTreeEntry findEntryByName(const QString &path);

    git_tree* ptr;
};

#endif
