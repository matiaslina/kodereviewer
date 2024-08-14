#ifndef TREE_H
#define TREE_H

#include "libgit/commit.h"
#include <QObject>
#include <git2.h>

class GitTree {
public:
    GitTree();
    GitTree(git_tree* ptr);
    ~GitTree();

    size_t entryCount() const;
    QString id() const;

    git_tree* ptr;
};

#endif
