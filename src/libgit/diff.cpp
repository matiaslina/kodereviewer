#include "diff.h"
#include "repository.h"
#include "tree.h"
#include "utils.h"

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
    : ptr(nullptr)
    , stats(nullptr)
{
}

Diff::Diff(Repository& repo, GitTree& t1, GitTree& t2)
    : ptr(nullptr)
    , stats(nullptr)
{
    int error = git_diff_tree_to_tree(&ptr, repo.ptr, t1.ptr, t2.ptr, nullptr);
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
