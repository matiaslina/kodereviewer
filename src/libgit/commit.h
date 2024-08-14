#ifndef COMMIT_H
#define COMMIT_H

#include <QObject>
#include <git2.h>

class GitTree;

class Commit {
public:
    Commit();
    Commit(git_commit* ptr);
    ~Commit();

    QString message() const;
    QString summary() const;

    GitTree tree() const;

    git_commit* ptr;
};

#endif
