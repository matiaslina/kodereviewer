#include "commit.h"
#include "tree.h"
#include "utils.h"
#include <git2/commit.h>

Commit::Commit()
    : ptr(nullptr)
{
}

Commit::Commit(git_commit* commit)
    : ptr(commit)
{
}

Commit::~Commit()
{
    if (ptr) {
        git_commit_free(ptr);
        ptr = nullptr;
    }
}

QString Commit::message() const
{
    return QString::fromUtf8(git_commit_message(ptr));
}

QString Commit::summary() const
{
    return QString::fromUtf8(git_commit_summary(ptr));
}

GitTree Commit::tree() const
{
    git_tree* tree;
    handleError(git_commit_tree(&tree, ptr));
    return GitTree(tree);
}
