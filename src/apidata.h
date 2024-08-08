#ifndef APIDATA_H
#define APIDATA_H

#include <QJsonDocument>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

/*!
  User structure from github api.
*/
class User {
public:
    User(QJsonDocument &document);

    QJsonObject toObject();

    /// login
    QString username;
    QUrl avatarUrl;
};

/*! Pull request data from
  https://docs.github.com/en/rest/pulls/pulls?apiVersion=2022-11-28#list-pull-requests
  (/pull-request)
*/
class PullRequest : public QObject {
    Q_OBJECT

    Q_PROPERTY(int number READ number NOTIFY numberChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QUrl url READ url)
    Q_PROPERTY(QUrl htmlUrl READ htmlUrl)
    Q_PROPERTY(QUrl issuesUrl READ issuesUrl)
    Q_PROPERTY(QUrl reviewCommentsUrl READ reviewCommentsUrl)
    Q_PROPERTY(QUrl commentsUrl READ commentsUrl)
    Q_PROPERTY(State state READ state)
    Q_PROPERTY(QDateTime createdAt READ createdAt)
    Q_PROPERTY(QDateTime updatedAt READ updatedAt)

    QML_ELEMENT

public:
    /// State of the pull request
    enum State {
        OPEN,
        CLOSED
    };

    Q_ENUM(State)


    PullRequest(QObject *parent = nullptr);
    PullRequest(QJsonDocument &document, QObject *parent = nullptr);
    ~PullRequest();

    unsigned int id() const;
    QString title() const;

    QUrl url() const;
    QUrl htmlUrl() const;
    QUrl issuesUrl() const;
    QUrl reviewCommentsUrl() const;
    QUrl commentsUrl() const;

    unsigned int number() const;
    State state() const;

    User *user() const;

    /// body
    QString description() const;

    QDateTime createdAt() const;
    QDateTime updatedAt() const;

signals:
    void titleChanged(QString title);
    void numberChanged(int number);
    void descriptionChanged(QString description);

private:
    unsigned int _id;
    QString _title;
    QUrl _url;
    QUrl _htmlUrl;
    QUrl _issuesUrl;
    QUrl _reviewCommentsUrl;
    QUrl _commentsUrl;

    unsigned int _number;
    State _state;

    std::unique_ptr<User> _user;

    /// body
    QString _description;

    QDateTime _createdAt;
    QDateTime _updatedAt;

};


/*! A single comment from /issues/{prNumber}/comments */
class Comment : public QObject {
    Q_OBJECT

    Q_PROPERTY(int id READ id NOTIFY idChanged)
    Q_PROPERTY(QString nodeId READ nodeId NOTIFY nodeIdChanged)
    Q_PROPERTY(QDateTime createdAt READ createdAt NOTIFY createdAtChanged)
    Q_PROPERTY(QDateTime updatedAt READ updatedAt NOTIFY updatedAtChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QString username READ username NOTIFY usernameChanged)

    QML_ELEMENT
public:
    Comment(QObject *parent = nullptr);
    Comment(QJsonDocument &document, QObject *parent = nullptr);
    ~Comment();

    unsigned int id() const;
    QString nodeId() const;
    QDateTime createdAt() const;
    QDateTime updatedAt() const;
    QString description() const;
    QString username() const;

signals:
    void idChanged(int id);
    void nodeIdChanged(QString nodeId);
    void createdAtChanged(QDateTime createdAt);
    void updatedAtChanged(QDateTime updatedAt);
    void descriptionChanged(QString description);
    void usernameChanged(QString username);

private:
    unsigned int _id;
    QString _nodeId;

    QDateTime _createdAt;
    QDateTime _updatedAt;

    /// body
    QString _description;

    std::unique_ptr<User> _user;
};

/*! A review comment from /pulls/{prNumber}/comments */
class Review {
public:
    Review(QJsonDocument &document);
    ~Review();

    QJsonObject toObject();

    unsigned int id;
    unsigned int pullRequestReviewId;
    QString nodeId;

    QString diffHunk;
    QString path;
    unsigned int line;

    std::unique_ptr<User> user;

    /// body
    QString description;
    QDateTime createdAt;
    QDateTime updatedAt;

    /// Id of the Review comment that this affects
    unsigned int inReplyTo = 0;
};

/*! A thread for one review */
class ReviewThread {
public:
    ReviewThread();
    ~ReviewThread();

    void addReview(Review *review);

    std::vector<Review *> reviews();
    bool hasId(unsigned int id);
    int line();
private:
    std::vector<Review *> childs;
};

#endif
