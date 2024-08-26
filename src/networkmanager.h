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

/*!
 * @brief Backend class to make requests to a single github repository
 */
class NetworkManager : public QObject {
    Q_OBJECT

    /**
     * Github owner
     */
    Q_PROPERTY(QString owner READ owner WRITE setOwner NOTIFY ownerChanged)

    /**
     * Github repository
     */
    Q_PROPERTY(QString repo READ repo WRITE setRepo NOTIFY repoChanged)

    /**
     * Base url for github api (it includes the ::owner and ::repo)
     */
    Q_PROPERTY(QUrl baseUrl READ baseUrl NOTIFY baseUrlChanged)

    /**
     * Tells if there's a pending request
     */
    Q_PROPERTY(bool pending READ pending NOTIFY pendingChanged)

    /**
     * Path of request that is pending
     *
     * ```
     *     if (networkManager.pending) {
     *         qDebug() << networkManager.lastPendingRequest;
     *         // /pulls/1/comments
     *     }
     * ```
     */
    Q_PROPERTY(QString lastPendingRequest READ lastPendingRequest NOTIFY lastPendingRequestChanged)

    QML_ELEMENT

public:
    /**
     * Construct a new NetworkManager
     */
    explicit NetworkManager(QObject* parent = nullptr);
    ~NetworkManager() override;

public slots:
    /** Holds the owner of the github account. */
    QString owner() const;

    /** Sets the owner of the github account
     *
     * Triggers the signal ownerChanged(QString owner)
     */
    void setOwner(QString owner);

    /** Holds the github repository to make requests */
    QString repo() const;

    /**
     * Sets the repository to make requests.
     *
     * Triggers the signal repoChanged(QString repo)
     */
    void setRepo(QString repo);

    /**
     * Holds the base url
     *
     * Example: QUrl("https://api.github.com/repos/matiaslina/kode-reviewer")
     */
    QUrl baseUrl() const;

    /**
     * Sets the base url updating the owner and the repo
     *
     * ```
     * networkManager.setUrl("owner", "repo");
     * qDebug() << networkManager.baseUrl(); // "https://api.github.com/repos/owner/repo
     * ```
     */
    void setUrl(QString owner, QString repo);

    /**
     * Holds if there's a network request pending
     */
    bool pending() const;

    /**
     * Holds the last request that was requested
     */
    QString lastPendingRequest() const;

    /**
     * Do a GET request to `<base-url>/pulls`
     *
     * When finished the signal ::pullRequestFinished(QByteArray doc)
     * is triggered with the body of the request in `doc`
     */
    void getPullRequests(void);

    /**
     * Retrieve the pull request comments for `pullRequestNumber`.
     *
     * This does a GET request to `<base-url>/issues/<pullRequestNumber>/comments`
     *
     * When finished the signal
     * ::pullRequestCommentsFinished(QByteArray doc) is triggered with
     * the body of the request in `doc`
     */
    void getPullRequestComments(int pullRequestNumber);

    /**
     * Retrieve the files changes in the pull request `pullRequestNumber`.
     *
     * This does a GET request to `<base-url>/pulls/<pullRequestNumber>/files`
     *
     * When finished the signal
     * ::pullRequestFilesFinished(QByteArray doc) is triggered with
     * the body of the request in `doc`
     */
    void getPullRequestFiles(int pullRequestNumber);

    /**
     * Retrieve the review threads of pull request `pullRequestNumber`
     *
     * This does a GET request to `<base-url>/pulls/<pullRequestNumber>/comments`
     *
     * When finished the signal
     * ::pullRequestThreadsFinished(QByteArray doc) is triggered with
     * the body of the request in `doc`
     */
    void getPullRequestThreads(int pullRequestNumber);

    /**
     * Send a comment to the github's base discussion of the pull request
     *
     * This does a POST request to `<base-url>/issues/<pullRequestNumber>/comments`
     *
     * When finished the signal
     * ::pullRequestPostCommentFinished(QByteArray doc) is triggered
     * with the body of the request in `doc`
     */
    void sendComment(int pullRequestNumber, QString comment);

signals:
    /**
     * This signal is emmited whenever the ::owner of the network manager changes.
     */
    void ownerChanged(QString owner);

    /**
     * This signal is emmited whenever the ::repo of the network manager changes.
     */
    void repoChanged(QString repo);

    /**
     * This signal is emmited whenever the ::baseUrl of the network manager changes.
     */
    void baseUrlChanged(QUrl baseUrl);

    /**
     * This signal is emmited whenever the ::pending of the network manager changes.
     */
    void pendingChanged(bool pending);

    /**
     * This signal is emmited whenever the ::lastPendingRequest of the network manager changes.
     */
    void lastPendingRequestChanged(QString lastPendingRequest);

    /**
     * Signal emmited when getPullRequest() finishes.
     */
    void pullRequestFinished(QByteArray document);

    /**
     * Signal emmited when getPullRequestComments() finishes.
     */
    void pullRequestCommentsFinished(QByteArray document);

    /**
     * Signal emmited when getPullRequestPostComment() finishes.
     */
    void pullRequestPostCommentFinished(QByteArray document);

    /**
     * Signal emmited when getPullRequestFiles() finishes.
     */
    void pullRequestFilesFinished(QByteArray document);

    /**
     * Signal emmited when getPullRequestThreads() finishes.
     */
    void pullRequestThreadsFinished(QByteArray document);

    /**
     * Signal emmited when there's an error different from QNetworkReply::NoError
     *
     * @param error The error body
     */
    void errorOcurred(QJsonDocument* error);

private slots:
    void replyFinished(QNetworkReply* reply);

private:
    QString _owner;
    QString _repo;
    QUrl base;
    QNetworkAccessManager* manager;
    QNetworkRequestFactory* requestFactory;

    // Last network request
    bool _pending;
    QString _lastPendingRequest;
    void setPending(bool pending);
    void setLastPendingRequest(QString lastPendingRequest);

    void setUrl();
};

#endif
