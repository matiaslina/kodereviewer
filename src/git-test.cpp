#include <iostream>
#include "libgit/repository.h"
#include <QDebug>
int main()
{
    initLibGit();
    QString path = QString::fromUtf8("/home/matias/Workspace/miniaudio-test/");
    Repository r(path);
    QString a = QString::fromUtf8("add-readme");
    auto ref = r.reference(a);
    qDebug() << "Reference name: " << ref.name();
    qDebug() << "Type: " << ref.typeStr();
    return 0;
}
