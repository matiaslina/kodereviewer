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

QString Blob::contents() const
{
    const char *content = (const char *) git_blob_rawcontent(ptr);
    return QString::fromUtf8(content);
}
