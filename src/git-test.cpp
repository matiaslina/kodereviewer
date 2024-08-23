
#include "qgit2/qgitglobal.h"
#include <QDebug>
#include <qgit2.h>
#include <qgit2/qgitrepository.h>
#include <qgit2/qgittree.h>

using namespace LibQGit2;




int main(void)
{
    LibQGit2::initLibQGit2();
    Repository *r = new Repository;
    QString path = "/home/matias/Workspace/miniaudio-test/";
    r->open(path);

    Object obj = r->lookupRevision("add-readme");
    qDebug() << "Object " << obj.oid().format();
    qDebug() << obj.typeString();
    Commit commit = obj.toCommit();
    qDebug() << "Commit " << commit.oid().format();

    Tree tree = commit.tree();
    qDebug() << tree.oid().format();
    qDebug() << tree.entryCount() << " asdf " << tree.oid().format();
    TreeEntry entry = tree.entryByName("README.org");
    qDebug() << entry.name();
    return 0;
}
