#include "gitobject.h"
#include <QDebug>
#include <git2/object.h>
#include <git2/types.h>

#include "commit.h"
#include "tree.h"

GitObject::GitObject()
    : ptr(nullptr)
{
}

GitObject::GitObject(git_object* obj)
    : ptr(obj)
{
}

GitObject::~GitObject()
{
    if (ptr) {
        git_object_free(ptr);
        ptr = nullptr;
    }
}

GitObject GitObject::revparseSingle(Repository& repository, QString revspec)
{
    git_object* obj = nullptr;
    git_revparse_single(&obj, repository.ptr, revspec.toStdString().c_str());

    return GitObject(obj);
}

git_object* GitObject::inner()
{
    return ptr;
}

GitObject::Type GitObject::type() const
{
    auto t = git_object_type(ptr);
    return (GitObject::Type(t));
}

QString GitObject::typeStr() const
{
    qDebug() << type();
    git_object_t t = (git_object_t)type();
    return QString::fromUtf8(git_object_type2string(t));
}

git_blob* GitObject::blob() const
{
    if (type() == GitObject::Type::BLOB) {
        return ((git_blob*)ptr);
    }
    return nullptr;
}

Commit GitObject::commit() const
{
    if (type() == GitObject::Type::COMMIT) {
        return Commit((git_commit*)ptr);
    }
    return Commit();
}

GitTree GitObject::tree() const
{
    if (type() == GitObject::Type::TREE) {
        return GitTree((git_tree*)ptr);
    }
    return GitTree();
}
git_tag* GitObject::tag() const
{
    if (type() == GitObject::Type::TAG) {
        return ((git_tag*)ptr);
    }
    return nullptr;
}
