#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>

#ifdef __cplusplus
extern "C" {
    #endif
#include <git2.h>
    #ifdef __cplusplus
}
#endif

class GitObject;


class Reference
{
public:
    Reference();
    ~Reference();
    bool valid();

    QString name();

    enum Type {
        Unknown = 0,
        Branch,
        Note,
        Remote,
        Tag
    };

    Type type();
    QString typeStr();
public:
    git_reference *ptr;
};

class Repository
{
public:
    Repository(QString &path);
    ~Repository();

    Reference reference(QString &shorthand);
    GitObject revparseSingle(const QString &spec);

    git_repository *ptr;
};

#endif
