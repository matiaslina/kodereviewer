#ifndef PATCH_H
#define PATCH_H

#include <QObject>
#include <git2/patch.h>

class Diff;
class Blob;


class Patch
{
public:
    Patch(Diff &diff, int idx);
    Patch(Blob &oldBlob, Blob &newBlob);
    ~Patch();

    QString toString() const;

    git_patch *ptr = nullptr;
};

class Hunk
{
public:
    Hunk();
    Hunk(Patch &patch, int idx);
    ~Hunk();
    static Hunk fromDiff(Patch &patch);

    size_t lines = 0;
    const git_diff_hunk *ptr = nullptr;
    Patch *patch;
};


#endif
