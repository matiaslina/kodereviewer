#include "gitbackend.h"
#include "libgit/gitobject.h"
#include "libgit/tree.h"
#include "libgit/diff.h"

GitBackend::GitBackend(QObject *parent)
    : QObject(parent)
    , repository(nullptr)
{}

GitBackend::GitBackend(QString &path, QObject *parent)
    : QObject(parent)
    , _path(path)
    , repository(nullptr)
{
    repository = new Repository(path);
}

GitBackend::~GitBackend() = default;

void GitBackend::initializeRepository()
{
    initializeRepository(_path);
}

void GitBackend::initializeRepository(QString &path)
{
    if (repository != nullptr)
    {
        delete repository;
        repository = nullptr;
    }
    repository = new Repository(path);
    emit repositoryInitialized();
}

QString GitBackend::path() const
{
    return _path;
}

void GitBackend::setPath(QString &path)
{
    _path = path;
    initializeRepository();
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

bool GitBackend::isRepositoryInitialized() const
{
    return repository != nullptr;
}

QString GitBackend::sourceFileContents(QString filename) const
{
    if (!isRepositoryInitialized() || _sourceRef.isEmpty()) {
        return "";
    }

    GitTree tree = repository->revparseSingle(_sourceRef).commit().tree();
    GitTreeEntry entry = tree.findEntryByName(filename);
    return entry.contents();
}

QString GitBackend::targetFileContents(QString filename) const
{
    if (!isRepositoryInitialized() || _targetRef.isEmpty()) {
        return QStringLiteral("");
    }
    GitTree tree = repository->revparseSingle(_targetRef).commit().tree();
    GitTreeEntry entry = tree.findEntryByName(filename);
    return entry.contents();
}
