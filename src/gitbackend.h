#ifndef GITBACKEND_H
#define GITBACKEND_H

#include <qqmlintegration.h>
#include <QObject>
#include <qgit2.h>
#include <qgit2/qgittree.h>
#include <qgit2/qgittreeentry.h>

class DiffModel;

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

    void initializeRepository();
    void initializeRepository(QString &path);

    QString path() const;
    void setPath(QString &path);

    QString sourceRef() const;
    void setSourceRef(QString ref);

    QString targetRef() const;
    void setTargetRef(QString ref);

public slots:
    QStringList filesChanged();
    bool isRepositoryInitialized();

    QString sourceFileContents(QString filename);
    QString targetFileContents(QString filename);
    QString diff(QString filename, DiffModel *model = nullptr);

signals:
    void pathChanged(QString path);
    void sourceRefChanged(QString ref);
    void targetRefChanged(QString ref);
    void repositoryInitialized();

private:
    QString _path;

    QString _sourceRef;
    QString _targetRef;

    LibQGit2::Tree targetTree() const;
    LibQGit2::Tree sourceTree() const;

    LibQGit2::Repository *repository;
};

#endif
