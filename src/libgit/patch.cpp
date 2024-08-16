#include "patch.h"

#include "utils.h"
#include "diff.h"
#include "blob.h"
#include <git2/patch.h>
#include <QDebug>


Hunk::Hunk()
    : patch(nullptr)
{}

Hunk::Hunk(Patch &patch, int idx)
    : patch(&patch)
{
    handleError(git_patch_get_hunk(&ptr, &lines, patch.ptr, idx));
}

Patch::Patch(Diff &diff, int idx)
{
    handleError(git_patch_from_diff(&ptr, diff.ptr, idx));
}

Patch::Patch(Blob &oldBlob, Blob &newBlob)
    : ptr(nullptr)
{
    qDebug() << oldBlob.id();
    qDebug() <<  newBlob.id();
    handleError(git_patch_from_blobs(&ptr, oldBlob.ptr, NULL, newBlob.ptr, NULL, nullptr));
}

Patch::~Patch()
{
    if (ptr != nullptr) {
        git_patch_free(ptr);
        ptr = nullptr;
    }
}

QString Patch::toString() const
{
    git_buf buf;
    bool error = handleError(git_patch_to_buf(&buf, ptr));
    if (error) {
        return QStringLiteral("");
    }
    return QString::fromUtf8(buf.ptr);
}
