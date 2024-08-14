#include "libgit/gitobject.h"
#include "libgit/repository.h"
#include "libgit/utils.h"
#include <QDebug>
#include <git2/object.h>
#include <git2/revparse.h>
#include <iostream>

#include "libgit/commit.h"
#include "libgit/diff.h"
#include "libgit/tree.h"
#include <git2.h>

void handle_error(int error)
{
    if (error < 0) {
        const git_error* e = git_error_last();
        printf("Error %d/%d: %s\n", error, e->klass, e->message);
        exit(error);
    }
}

void main_libgit()
{
    auto repo_path = "/home/matias/Workspace/miniaudio-test/";
    git_repository* repo;
    int error = git_repository_open(&repo, repo_path);
    handle_error(error);

    git_object* obj = NULL;
    error = git_revparse_single(&obj, repo, "add-readme");
    handle_error(error);
    std::cout << git_object_type2string(git_object_type(obj)) << std::endl;
}

int main(int argc, char** argv)
{
    initLibGit();
    if (argc == 2 && std::string(argv[1]) == "libgit") {
        main_libgit();
    } else {
        QString path = QString::fromUtf8("/home/matias/Workspace/miniaudio-test/");
        Repository r(path);
        QString a = QString::fromUtf8("add-readme");
        QString master = QString::fromUtf8("master");
        auto obj = r.revparseSingle(a);
        auto obj2 = r.revparseSingle(master);

        qDebug() << obj.typeStr();
        qDebug() << obj2.typeStr();

        auto t1 = obj.commit().tree();
        auto t2 = obj2.commit().tree();

        auto diff = Diff(r, t1, t2);

        qDebug() << "Deltas: " << diff.numDelta();
        for (auto d : diff.deltas()) {
            qDebug() << "Filename: " << d->newFile().path();
        }
    }
    return 0;
}
