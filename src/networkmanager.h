#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QUrl>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequestFactory>
#include <qnetworkreply.h>
#include <qqmlengine.h>
#include <qqmlintegration.h>

class NetworkManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString owner READ owner WRITE setOwner NOTIFY ownerChanged)
    Q_PROPERTY(QString repo READ repo WRITE setRepo NOTIFY repoChanged)
    Q_PROPERTY(QUrl baseUrl READ baseUrl NOTIFY baseUrlChanged)

    QML_ELEMENT

public:
    explicit NetworkManager(QObject* parent = nullptr);
    ~NetworkManager() override;

public slots:
    QString owner() const;
    void setOwner(QString owner);
    QString repo() const;
    void setRepo(QString repo);

    QUrl baseUrl() const;
    void setUrl(QString owner, QString repo);

    void getPullRequests(void);
    void getPullRequestComments(int pullRequestNumber);
    void getPullRequestFiles(int pullRequestNumber);

    void getComments(int pullRequestNumber);
    void getDiff(QString diffUrl);

signals:
    void ownerChanged(QString owner);
    void repoChanged(QString repo);
    void baseUrlChanged(QUrl baseUrl);

    void pullRequestFinished(QByteArray document);
    void pullRequestCommentsFinished(QByteArray document);
    void pullRequestFilesFinished(QByteArray document);
    void errorOcurred(QJsonDocument* error);



    // TODO: Cleanup
    void diffFinished(QString* diff);
    void commentsFinished(QJsonDocument* document);


private slots:
    void replyFinished(QNetworkReply* reply);

private:
    QString _owner;
    QString _repo;
    QUrl base;
    QNetworkAccessManager* manager;
    QNetworkRequestFactory* requestFactory;

    void setUrl();
};

#endif
