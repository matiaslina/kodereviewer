#include "repository.h"
#include <git2/global.h>
#include <qdebug.h>

void initLibGit()
{
    git_libgit2_init();
}

void shutdownLibGit()
{
    git_libgit2_shutdown();
}

bool handleError(int error)
{
    if (error < 0) {
        const git_error *e = git_error_last();
        qDebug() << QString::fromUtf8("Error %1/%2: %3").arg(error)
            .arg(e->klass)
            .arg(e->message);
        return true;
    }
    return false;
}

Reference::Reference()
    : ptr(nullptr)
{}

Reference::~Reference() = default;

bool Reference::valid()
{
    return ptr != nullptr;
}

QString Reference::name()
{
    return QString::fromUtf8(git_reference_name(ptr));
}

Reference::Type Reference::type()
{
    if (git_reference_is_branch(ptr)) {
        return Type::Branch;
    } else if (git_reference_is_note(ptr)) {
        return Type::Note;
    } else if (git_reference_is_remote(ptr)) {
        return Type::Remote;
    } else if (git_reference_is_tag(ptr)) {
        return Type::Tag;
    }
    return Type::Unknown;
}

QString Reference::typeStr()
{
    switch (type())
    {
    case Type::Branch:
        return "Branch";
    case Type::Note:
        return "Note";
    case Type::Remote:
        return "Remote";
    case Type::Tag:
        return "Tag";
    case Type::Unknown:
        return "Unknown";
    }
    return "";
}


Repository::Repository(QString &path)
{
    handleError(git_repository_open(&this->_repository, path.toStdString().c_str()));
}

Repository::~Repository()
{
    if(this->_repository != nullptr) {
        git_repository_free(this->_repository);
        this->_repository = nullptr;
    }
}

Reference Repository::reference(QString &shorthand)
{
    Reference r;
    handleError(git_reference_dwim(&r.ptr, _repository, shorthand.toStdString().c_str()));
    return r;
}
