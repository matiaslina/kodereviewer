#include "gitbackend.h"

#include <QDebug>
#include <qgit2.h>
#include <qgit2/qgitrepository.h>
#include <qgit2/qgitpatch.h>
#include <qgit2/qgitdiffhunk.h>
#include <QFile>
#include "diffmodel.h"

using namespace LibQGit2;


/*************
 * GITBACKEND
 *************/

GitBackend::GitBackend(QObject *parent)
    : QObject(parent)
    , repository(nullptr)
{}

GitBackend::GitBackend(QString &path, QObject *parent)
    : QObject(parent)
    , _path(path)
    , repository(nullptr)
{
    if (!path.isEmpty()) {
        repository = new Repository(nullptr);
        initializeRepository();
    }
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
    repository = new Repository(nullptr);
    repository->open(path + ".git/");

    emit repositoryInitialized();
}

QString GitBackend::path() const
{
    return _path;
}

void GitBackend::setPath(QString &path)
{
    _path = path;
    if (!_path.isEmpty()) {
        initializeRepository();
    }
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
    auto sourceObj = repository->lookupRevision(_sourceRef);
    auto targetObj = repository->lookupRevision(_targetRef);

    if (sourceObj.isTree() && targetObj.isTree()) {

        auto sourceTree = sourceObj.toTree();
        auto targetTree = targetObj.toTree();

        Diff diff = repository->diffTrees(sourceTree, targetTree);
        for (size_t idx = 0; idx < diff.numDeltas(); idx++) {
            auto delta = diff.delta(idx);
            l << delta.newFile().path();
        }

    }

    return l;
}

bool GitBackend::isRepositoryInitialized()
{
    return repository != nullptr;
}

QString GitBackend::sourceFileContents(QString filename)
{
    if (!isRepositoryInitialized() || _sourceRef.isEmpty()) {
        return "";
    }

    try {
        Tree tree = sourceTree();
        TreeEntry entry = tree.entryByName(filename);
        Blob blob = entry.toObject(*repository).toBlob();
        return QString(blob.content());
    } catch (LibQGit2::Exception &e) {
        qDebug() << "sourceFileContents: " << e.what();

        return QString("");
    }
}

QString GitBackend::targetFileContents(QString filename)
{
    if (!isRepositoryInitialized() || _targetRef.isEmpty()) {
        return QString("");
    }
    Tree tree;
    try {
        tree = targetTree();
        TreeEntry entry = tree.entryByName(filename);
        Blob blob = entry.toObject(*repository).toBlob();
        return QString(blob.content());
    } catch (LibQGit2::Exception &e) {
        qDebug() << "targetFileContents: " << e.what();
        return QString("");
    }

}

QString GitBackend::diff(QString filename, DiffModel *model)
{
    if (!isRepositoryInitialized() || _targetRef.isEmpty() || _sourceRef.isEmpty()) {
        return QString("");
    }
    Diff diff = repository->diffTrees(targetTree(), sourceTree());
    qDebug() << diff.numDeltas();
    Patch patch = diff.patch(0);
    for (size_t i = 0; i < patch.numHunks(); i++) {
        DiffHunk hunk = patch.hunk(i);
        qDebug() << hunk.header();
        if (model != nullptr) {
            model->insertDiffHunk(hunk);
        }
    }
    return QString();
}


/***********
 * Private
 ***********/

Tree GitBackend::targetTree() const
{
    Object obj = repository->lookupRevision(_targetRef);
    Commit commit = obj.toCommit();

    return commit.tree();
}

Tree GitBackend::sourceTree() const
{
    Object obj = repository->lookupRevision(_sourceRef);
    Commit commit = obj.toCommit();
    return commit.tree();
}
