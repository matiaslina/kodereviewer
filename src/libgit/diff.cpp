#include "diff.h"
#include "repository.h"
#include "tree.h"
#include "utils.h"
#include <git2/diff.h>

DiffFile::DiffFile(const git_diff_file& f)
    : file(f)
{
}

git_oid DiffFile::id() const { return file.id; }
QString DiffFile::path() const { return QString::fromUtf8(file.path); }
git_object_size_t DiffFile::size() const { return file.size; }
uint32_t DiffFile::flags() const { return file.flags; }

Delta::Delta(const git_diff_delta* delta)
    : ptr(delta)
{
}

Delta::~Delta() = default;

git_delta_t Delta::status() const
{
    return ptr->status;
}

uint32_t Delta::flags() const { return ptr->flags; }
uint16_t Delta::similarity() const { return ptr->similarity; }
uint16_t Delta::nfiles() const { return ptr->nfiles; }
DiffFile Delta::oldFile() { return DiffFile(ptr->old_file); }
DiffFile Delta::newFile() { return DiffFile(ptr->new_file); }

//// DIFF

Diff::Diff()
    : repository(nullptr)
    , ptr(nullptr)
    , stats(nullptr)
{
}

Diff::Diff(Repository &repo, QString pathspec)
    : repository(repo.ptr)
    , ptr(nullptr)
    , stats(nullptr)
{
    git_diff_options opts = GIT_DIFF_OPTIONS_INIT;
    std::string stdpathstring = pathspec.toStdString();
    char *cpathspec = (char *) stdpathstring.c_str();

    opts.context_lines = 3;
    opts.interhunk_lines = 1;
    opts.flags |= GIT_DIFF_INCLUDE_IGNORED | GIT_DIFF_INCLUDE_UNTRACKED;
    opts.pathspec.strings = &cpathspec;
    opts.pathspec.count = 1;

    handleError(git_diff_index_to_workdir(&ptr, repository, NULL, &opts));
    handleError(git_diff_get_stats(&stats, ptr));
}

Diff::Diff(Repository& repo, GitTree& t1, GitTree& t2)
    : repository(repo.ptr)
    , ptr(nullptr)
    , stats(nullptr)
{
    int error = git_diff_tree_to_tree(&ptr, repository, t1.ptr, t2.ptr, nullptr);
    handleError(error);

    error = git_diff_get_stats(&stats, ptr);
    handleError(error);
}

Diff::Diff(Repository& repo, GitTree& t1, GitTree& t2, QString pathspec)
    : repository(repo.ptr)
    , ptr(nullptr)
    , stats(nullptr)
{
    const git_diff_options opts = singleFileOptions(pathspec);
    int error = git_diff_tree_to_tree(&ptr, repository, t1.ptr, t2.ptr, &opts);
    handleError(error);

    error = git_diff_get_stats(&stats, ptr);
    handleError(error);
}

Diff::~Diff()
{
    if (ptr) {
        git_diff_free(ptr);
        ptr = nullptr;
    }
    if (stats) {
        git_diff_stats_free(stats);
        stats = nullptr;
    }
}

int Diff::numDelta() const
{
    return git_diff_num_deltas(ptr);
}

Delta* Diff::delta(size_t idx)
{
    const git_diff_delta* gitDelta = git_diff_get_delta(ptr, idx);
    return new Delta(gitDelta);
}

std::vector<Delta*> Diff::deltas()
{
    std::vector<Delta*> retval;
    for (int i = 0; i < numDelta(); i++) {
        retval.push_back(delta(i));
    }
    return retval;
}

QString Diff::toString() const
{
    git_buf buf;
    handleError(git_diff_to_buf(&buf, ptr, GIT_DIFF_FORMAT_PATCH));

    return QString::fromUtf8(buf.ptr, buf.size);
}

git_diff_options Diff::singleFileOptions(QString &filename)
{
    git_diff_options opts = GIT_DIFF_OPTIONS_INIT;
    std::string stdpathstring = filename.toStdString();
    char *cpathspec = (char *) stdpathstring.c_str();

    opts.context_lines = 3;
    opts.interhunk_lines = 1;
    opts.flags |= GIT_DIFF_INCLUDE_IGNORED | GIT_DIFF_INCLUDE_UNTRACKED;
    opts.pathspec.strings = &cpathspec;
    opts.pathspec.count = 1;

    return opts;
}
