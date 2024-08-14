#ifndef DIFF_H
#define DIFF_H

#include <QObject>
#include <git2/diff.h>
#include <vector>

class GitTree;
class Repository;

class DiffFile {
public:
    DiffFile(const git_diff_file& f);

    git_oid id() const;
    QString path() const;
    git_object_size_t size() const;
    uint32_t flags() const;

private:
    git_diff_file file;
};

class Delta {
public:
    Delta(const git_diff_delta* delta);
    ~Delta();

    git_delta_t status() const;
    uint32_t flags() const;
    uint16_t similarity() const;
    uint16_t nfiles() const;
    DiffFile oldFile();
    DiffFile newFile();

    const git_diff_delta* ptr;
};

class Diff {
public:
    Diff();
    Diff(Repository& repo, GitTree& t1, GitTree& t2);
    ~Diff();

    int numDelta() const;
    Delta* delta(size_t idx);
    std::vector<Delta*> deltas();

    git_diff* ptr;
    git_diff_stats* stats;
};

#endif
