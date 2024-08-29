#include "networkmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSettings>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <QRegularExpression>

NetworkManager::NetworkManager(QObject* parent)
    : QObject(parent)
    , _pending(false)
    , _lastPendingRequest("")
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


/***************
 * SLOTS
 ***************/

QString NetworkManager::owner() const
{
    return _owner;
}

void NetworkManager::setOwner(QString owner)
{
    _owner = owner;
    setUrl();
}

QString NetworkManager::repo() const
{
    return _repo;
}

void NetworkManager::setRepo(QString repo)
{
    _repo = repo;
    setUrl();
}

QUrl NetworkManager::baseUrl() const
{
    return requestFactory->baseUrl();
}

/**
 * Set the url to the api endpoint for `owner` and `repo`
 */
void NetworkManager::setUrl()
{
    if (!_owner.isEmpty() && !_repo.isEmpty()) {
        setUrl(_owner, _repo);
        // qDebug() << "Setting url" << baseUrl();
        emit baseUrlChanged(baseUrl());
    }
}

void NetworkManager::setUrl(QString owner, QString repo)
{
    base = QUrl(QString("https://api.github.com/repos/%1/%2").arg(owner, repo));
    requestFactory->setBaseUrl(base);
}

bool NetworkManager::pending() const
{
    return _pending;
}

void NetworkManager::setPending(bool pending)
{
    _pending = pending;
    emit pendingChanged(pending);
}

QString NetworkManager::lastPendingRequest() const
{
    return _lastPendingRequest;
}

void NetworkManager::setLastPendingRequest(QString lastPendingRequest)
{
    _lastPendingRequest = lastPendingRequest;
    emit lastPendingRequestChanged(_lastPendingRequest);
}

bool matchUrl(QString pattern, QString url)
{
    return QRegularExpression(pattern).match(url).hasMatch();
}

void NetworkManager::replyFinished(QNetworkReply* reply)
{
    QString url = reply->url().toString();
    auto contents = reply->readAll();
    // qDebug() << "<----" << url;

    // auto headers = reply->rawHeaderPairs();
    // for (auto header : headers) {
    //     // qDebug() << header.first << ": " << header.second;
    // }

    switch (reply->error()) {
    case QNetworkReply::NoError:
        if (matchUrl("/pulls$", url)) {
            auto document = QJsonDocument::fromJson(contents);
            // qDebug() << "Emiting pull request finished";
            emit pullRequestFinished(document.toJson());
        } else if (matchUrl("/issues/\\d+/comments$", url)) {
            auto document = QJsonDocument::fromJson(contents);
            if (reply->operation() == QNetworkAccessManager::Operation::GetOperation) {
                // qDebug() << "Emiting GET comments finished";
                emit pullRequestCommentsFinished(document.toJson());
            } else if (reply->operation() == QNetworkAccessManager::Operation::PostOperation){
                // qDebug() << "Emiting POST comments finished";
                emit pullRequestPostCommentFinished(document.toJson());
            }
        } else if (matchUrl("/pulls/\\d+/files$", url)) {
            auto document = QJsonDocument::fromJson(contents);
            // qDebug() << "Emiting files finished";
            emit pullRequestFilesFinished(document.toJson());
        } else if (matchUrl("/pulls/\\d+/comments$", url)) {
            auto document = QJsonDocument::fromJson(contents);
            if (reply->operation() == QNetworkAccessManager::Operation::GetOperation) {
            // qDebug() << "Emiting pulls comments finished";
                emit pullRequestThreadsFinished(document.toJson());
            } else if (reply->operation() == QNetworkAccessManager::Operation::PostOperation) {
                qDebug() << "Create thead finished";
                emit createThreadFinished(document.toJson());
            }
        } else if (matchUrl("/pulls/\\d+/comments/.+/replies$", url)) {
            auto document = QJsonDocument::fromJson(contents);
            // qDebug() << "Emiting threads finished";
            emit sendThreadCommentFinished(document.toJson());
        } else {
            // qDebug() << "Not matching url: " << url;
        }
        break;
    case QNetworkReply::ProtocolUnknownError:
        // qDebug() << "Protocol unknown error";
        {};
    default:
        auto document = QJsonDocument::fromJson(contents);
        qDebug() << "<----" << url;

        qDebug() << document;
        emit errorOcurred(&document);
    }
    setPending(false);
}

void NetworkManager::getPullRequests()
{
    QString requestUrl = QString("/pulls");
    setPending(true);
    setLastPendingRequest(requestUrl);
    manager->get(requestFactory->createRequest(requestUrl));
}

void NetworkManager::getPullRequestComments(int pullRequestNumber)
{
    QString requestUrl = QString("/issues/%1/comments").arg(pullRequestNumber);
    // qDebug() << "---->" << requestUrl;

    setPending(true);
    setLastPendingRequest(requestUrl);
    manager->get(requestFactory->createRequest(requestUrl));
}

void NetworkManager::getPullRequestFiles(int pullRequestNumber)
{
    QString requestUrl = QString("/pulls/%1/files").arg(pullRequestNumber);
    // qDebug() << "---->" << requestUrl;

    setPending(true);
    setLastPendingRequest(requestUrl);
    manager->get(requestFactory->createRequest(requestUrl));
}

void NetworkManager::getPullRequestThreads(int pullRequestNumber)
{
    QString requestUrl = QString("/pulls/%1/comments").arg(pullRequestNumber);
    // qDebug() << "---->" << requestUrl;
    setPending(true);
    setLastPendingRequest(requestUrl);
    manager->get(requestFactory->createRequest(requestUrl));
}

void NetworkManager::createThread(int pullRequestNumber, QString comment, QString commitId, QString path, int line)
{
    QString requestUrl = QString("/pulls/%1/comments").arg(pullRequestNumber);
    setPending(true);
    setLastPendingRequest(requestUrl);
    QJsonDocument body = QJsonDocument(QJsonObject({
                { "body", comment },
                { "commit_id", commitId },
                { "path", path },
                // For file is not needed
                //                { "line", line },
                { "side", "RIGHT" },
                {"subject_type", "file" } // Could be file or line
            }));
    qDebug() << body;
    manager->post(requestFactory->createRequest(requestUrl), body.toJson());
}

void NetworkManager::sendComment(int pullRequestNumber, QString comment)
{
    QJsonDocument json = QJsonDocument(QJsonObject({
            {"body", comment}
            })
        );
    QString requestUrl = QString("/issues/%1/comments").arg(pullRequestNumber);
    // qDebug() << "---->" << requestUrl;
    setPending(true);
    setLastPendingRequest(requestUrl);
    manager->post(requestFactory->createRequest(requestUrl), json.toJson());
}

void NetworkManager::sendThreadComment(int pullRequestNumber, int commentId, QString comment)
{
    QJsonDocument json = QJsonDocument(QJsonObject({
                { "body", comment }
            }));
    QString requestUrl = QString("/pulls/%1/comments/%2/replies").arg(pullRequestNumber).arg(commentId);
    // qDebug() << "---->" << requestUrl;

    setPending(true);
    setLastPendingRequest(requestUrl);
    manager->post(requestFactory->createRequest(requestUrl), json.toJson());
}
