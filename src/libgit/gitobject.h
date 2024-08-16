#ifndef GITOBJECT_H
#define GITOBJECT_H

#include "repository.h"
#include <QObject>
#include <git2/types.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <git2.h>
#ifdef __cplusplus
}
#endif

class Commit;
class GitTree;
class Blob;

class GitObject {
public:
    enum Type {
        INVALID = GIT_OBJECT_INVALID,
        ANY = GIT_OBJECT_ANY,
        UNKNOWN = 0,
        BLOB = GIT_OBJECT_BLOB,
        COMMIT = GIT_OBJECT_COMMIT,
        TREE = GIT_OBJECT_TREE,
        TAG = GIT_OBJECT_TAG,
        OFS_DELTA = GIT_OBJECT_OFS_DELTA,
        REF_DELTA = GIT_OBJECT_REF_DELTA
    };
    GitObject();
    GitObject(git_object* obj);
    ~GitObject();

    static GitObject *revparseSingle(Repository& repository, QString revspec);

    git_object* inner();
    Type type() const;
    QString typeStr() const;

    Blob blob() const;
    Commit commit() const;
    GitTree tree() const;
    git_tag* tag() const;

    git_object* ptr;
};

#endif
