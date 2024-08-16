#include "blob.h"
#include <git2/blob.h>
#include <git2/types.h>

Blob::Blob(git_blob *blob)
    : ptr(blob)
{}

Blob::~Blob()
{
    if (ptr != nullptr) {
        git_blob_free(ptr);
        ptr = nullptr;
    }
}

bool Blob::isInitialized()
{
    return ptr != nullptr;
}

QString Blob::id() const
{
    return QString::fromUtf8((const char*)git_blob_id(ptr));
}

QString Blob::contents() const
{
    unsigned char *content = (unsigned char *) git_blob_rawcontent(ptr);
    return QString::fromUtf8(content);
}
