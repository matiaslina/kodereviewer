#include "tree.h"
#include <git2/tree.h>

GitTree::GitTree()
    : ptr(nullptr)
{
}

GitTree::GitTree(git_tree* tree)
    : ptr(tree)
{
}

GitTree::~GitTree()
{
    if (ptr) {
        git_tree_free(ptr);
        ptr = nullptr;
    }
}

size_t GitTree::entryCount() const
{
    return git_tree_entrycount(ptr);
}

QString GitTree::id() const
{
    const git_oid* oid = git_tree_id(ptr);
    return QString::fromUtf8((char*)oid);
}
