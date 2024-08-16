#ifndef BLOB_H
#define BLOB_H

#include <QObject>
#include <git2.h>

class Blob
{
 public:
    Blob(git_blob *blob);
    ~Blob();

    bool isInitialized();
    QString id() const;
    QString contents() const;

    git_blob *ptr = nullptr;
};

#endif
