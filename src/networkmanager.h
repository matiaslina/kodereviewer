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
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit NetworkManager(QObject* parent = nullptr);
    ~NetworkManager() override;

public slots:
    void setUrl(QString owner, QString repo);

    void getPullRequests(void);
    void getPullRequestComments(int pullRequestNumber);



    void getComments(int pullRequestNumber);
    void getDiff(QString diffUrl);

signals:
    void pullRequestFinished(QByteArray document);
    void pullRequestCommentsFinished(QByteArray document);
    void errorOcurred(QNetworkReply::NetworkError code, QJsonDocument* error);



    // TODO: Cleanup
    void diffFinished(QString* diff);
    void commentsFinished(QJsonDocument* document);


private slots:
    void replyFinished(QNetworkReply* reply);

private:
    QUrl base;
    QNetworkAccessManager* manager;
    QNetworkRequestFactory* requestFactory;
};

#endif
