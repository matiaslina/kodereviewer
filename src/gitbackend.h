#ifndef GITBACKEND_H
#define GITBACKEND_H

#include "libgit/repository.h"

#include <qqmlintegration.h>


class GitBackend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString sourceRef READ sourceRef WRITE setSourceRef NOTIFY sourceRefChanged)
    Q_PROPERTY(QString targetRef READ targetRef WRITE setTargetRef NOTIFY targetRefChanged)


    QML_ELEMENT
public:
    GitBackend(QObject *parent = nullptr);
    GitBackend(QString &path, QObject *parent = nullptr);
    ~GitBackend();

    QString path() const;
    void setPath(QString &path);

    QString sourceRef() const;
    void setSourceRef(QString ref);

    QString targetRef() const;
    void setTargetRef(QString ref);

public slots:
    QStringList filesChanged();

signals:
    void pathChanged(QString path);
    void sourceRefChanged(QString ref);
    void targetRefChanged(QString ref);

private:
    QString _path;

    QString _sourceRef;
    QString _targetRef;

    Repository *repository;
};

#endif
