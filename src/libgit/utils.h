#ifndef GITUTILS_H
#define GITUTILS_H

#include <QObject>

void initLibGit();
void shutdownLibGit();

bool handleError(int error, QString what = "");

#endif
