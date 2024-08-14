#include "gitbackend.h"
#include "libgit/gitobject.h"
#include "libgit/tree.h"
#include "libgit/diff.h"

GitBackend::GitBackend(QObject *parent)
    : QObject(parent)
{}

GitBackend::GitBackend(QString &path, QObject *parent)
    : QObject(parent)
    , _path(path)
{
    repository = new Repository(path);
}

GitBackend::~GitBackend() = default;

QString GitBackend::path() const
{
    return _path;
}

void GitBackend::setPath(QString &path)
{
    _path = path;
}


QString GitBackend::sourceRef() const
{
    return _sourceRef;
}

void GitBackend::setSourceRef(QString ref)
{
    _sourceRef = ref;
}

QString GitBackend::targetRef() const
{
    return _targetRef;
}

void GitBackend::setTargetRef(QString ref)
{
    _targetRef = ref;
}

/*************
 * SLOTS
 ***************/

QStringList GitBackend::filesChanged()
{
    QStringList l;
    /// TODO: repository == null
    auto sourceObj = repository->revparseSingle(_sourceRef);
    auto targetObj = repository->revparseSingle(_targetRef);

    auto sourceTree = sourceObj.commit().tree();
    auto targetTree = targetObj.commit().tree();

    Diff diff(*repository, sourceTree, targetTree);

    for (auto delta : diff.deltas()) {
        l << delta->newFile().path();
    }

    return l;
}
