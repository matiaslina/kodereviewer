#include "networkmanager.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QSettings>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>

NetworkManager::NetworkManager(QObject* parent)
    : QObject(parent)
{
    QSettings settings;
    manager = new QNetworkAccessManager;
    requestFactory = new QNetworkRequestFactory;
    QHttpHeaders headers;
    headers.append(QHttpHeaders::WellKnownHeader::Accept, "application/vnd.github+json");
    headers.append(QHttpHeaders::WellKnownHeader::Authorization,
        QString::fromUtf8("Bearer %1").arg(settings.value("githubToken").toString()));
    requestFactory->setCommonHeaders(headers);

    connect(manager, &QNetworkAccessManager::finished, this, &NetworkManager::replyFinished);
}

NetworkManager::~NetworkManager() { }

void NetworkManager::setUrl(QString owner, QString repo)
{
    base = QUrl(tr("https://api.github.com/repos/%1/%2")
                    .arg(owner)
                    .arg(repo));
    requestFactory->setBaseUrl(base);
}

void NetworkManager::replyFinished(QNetworkReply* reply)
{
    QString url = reply->url().toString();
    auto contents = reply->readAll();
    qDebug() << url;

    // auto headers = reply->rawHeaderPairs();
    // for (auto header : headers) {
    //     qDebug() << header.first << ": " << header.second;
    // }

    switch (reply->error()) {
    case QNetworkReply::NoError:
        if (url.endsWith(tr("pulls"))) {
            auto document = QJsonDocument::fromJson(contents);
            emit pullRequestFinished(document.toJson());
        } else if (url.endsWith(tr("comments"))) {
            auto document = QJsonDocument::fromJson(contents);
            emit pullRequestCommentsFinished(document.toJson());
        } else if (url.endsWith(tr(".diff"))) {
            emit diffFinished(new QString(QString::fromUtf8(contents)));
        }
        break;
    case QNetworkReply::ProtocolUnknownError:
        qDebug() << "protocol unknown: " << reply->errorString() << " For "
                 << requestFactory->baseUrl();
    default:
        auto document = QJsonDocument::fromJson(contents);
        qDebug() << document;
        emit errorOcurred(reply->error(), &document);
    }
}

void NetworkManager::getPullRequests()
{
    manager->get(requestFactory->createRequest(tr("/pulls")));
}

void NetworkManager::getPullRequestComments(int pullRequestNumber)
{
    manager->get(requestFactory->createRequest(tr("/issues/%1/comments").arg(pullRequestNumber)));
}

void NetworkManager::getComments(int pullRequestNumber)
{
    manager->get(requestFactory->createRequest(tr("/pulls/%1/comments").arg(pullRequestNumber)));
}

void NetworkManager::getDiff(QString diffUrl)
{
    // manager->get(requestFactory->createRequest(tr("/pull/%1.diff").arg(pullRequestNumber)));
    manager->get(QNetworkRequest(QUrl(diffUrl)));
}
