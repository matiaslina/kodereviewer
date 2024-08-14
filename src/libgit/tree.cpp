#include "tree.h"
#include <git2/tree.h>

#include "utils.h"
#include "blob.h"

GitTreeEntry::GitTreeEntry(GitTree* tree, int idx)
    : parent(tree)
{
    entry = git_tree_entry_byindex(tree->ptr, idx);
}

GitTreeEntry::GitTreeEntry(GitTree* tree, const QString &path)
    : parent(tree)
{
    entry = git_tree_entry_byname(tree->ptr, path.toStdString().c_str());
}

const git_oid* GitTreeEntry::id() const
{
    return git_tree_entry_id(entry);
}

QString GitTreeEntry::name() const
{
    return QString::fromUtf8(git_tree_entry_name(entry));
}

QString GitTreeEntry::contents() const
{
    git_object *o = nullptr;
    int err = git_tree_entry_to_object(&o, parent->owner(), entry);
    if (handleError(err)) {
        return QStringLiteral("");
    }

    GitObject obj(o);

    Blob blob = obj.blob();
    return blob.contents();
}

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

git_repository *GitTree::owner()
{
    return git_tree_owner(ptr);
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

GitTreeEntry GitTree::findEntryByName(const QString &path)
{
    return GitTreeEntry(this, path);
}
