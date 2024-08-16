#include <git2.h>
#include <git2/global.h>
#include <QDebug>


void initLibGit()
{
    git_libgit2_init();
}

void shutdownLibGit()
{
    git_libgit2_shutdown();
}

bool handleError(int error, QString what)
{
    if (error < 0) {
        const git_error *e = git_error_last();
        qDebug() << QString::fromUtf8("Error %1/%2: %3: %4").arg(error)
            .arg(e->klass)
            .arg(e->message)
            .arg(what);
        return true;
    }
    return false;
}
