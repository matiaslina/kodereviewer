#ifndef APIDATA_H
#define APIDATA_H

#include <QJsonDocument>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <QPointer>

#include "reviewthreadmodel.h"


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


/*! A review comment from /pulls/{prNumber}/comments */
class Review : public QObject  {
    Q_OBJECT

    Q_PROPERTY(int id READ id NOTIFY idChanged)
    Q_PROPERTY(QString nodeId READ nodeId NOTIFY nodeIdChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QDateTime createdAt READ createdAt NOTIFY createdAtChanged)
    Q_PROPERTY(QDateTime updatedAt READ updatedAt NOTIFY updatedAtChanged)

    Q_PROPERTY(QString username READ username NOTIFY usernameChanged)
    Q_PROPERTY(QUrl avatarUrl READ avatarUrl NOTIFY avatarUrlChanged)

    QML_ELEMENT

public:
    Review(QObject *parent = nullptr);
    Review(QJsonDocument &document, QObject *parent = nullptr);
    ~Review();

    QJsonObject toObject();

    int id() const;
    QString nodeId() const;
    QString description() const;
    QDateTime createdAt() const;
    QDateTime updatedAt() const;

    QString username() const;
    QUrl avatarUrl() const;

signals:
    void idChanged(int id);
    void nodeIdChanged(QString nodeId);
    void descriptionChanged(QString description);
    void createdAtChanged(QDateTime createdAt);
    void updatedAtChanged(QDateTime updatedAt);

    void usernameChanged(QString username);
    void avatarUrlChanged(QUrl avatarUrl);

private:
    unsigned int _id;
    unsigned int _pullRequestReviewId;
    QString _nodeId;

    QString _diffHunk;
    QString _path;
    unsigned int _line;

    std::unique_ptr<User> _user;

    /// body
    QString _description;
    QDateTime _createdAt;
    QDateTime _updatedAt;

    /// Id of the Review comment that this affects
    unsigned int _inReplyTo = 0;
};

/*! A thread for one review */
class ReviewThread : public QObject {
    Q_OBJECT

    /**
     * Path (file) for the review thread
     */
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

    /**
     * Line of the thread
     */
    Q_PROPERTY(int line READ line WRITE setLine NOTIFY lineChanged)

    /**
     * From where the review starts in the source file
     */
    Q_PROPERTY(unsigned int startLine READ startLine NOTIFY startLineChanged)

    /**
     * Where the thread ends in the source file
     */
    Q_PROPERTY(unsigned int endLine READ endLine NOTIFY endLineChanged)

    /**
     * Holds the comments for this review
     */
    Q_PROPERTY(QList<Review*> comments READ comments NOTIFY commentsChanged)

     /**
     * Holds the original line of the review
     */
    Q_PROPERTY(int originalLine READ originalLine NOTIFY originalLineChanged)


    QML_ELEMENT
public:
    explicit ReviewThread(QObject *parent = nullptr);
    ReviewThread(QString &path, int line, int originalLine, QObject *parent = nullptr);
    ReviewThread(QString &path, int line, int originalLine, unsigned int startLine, unsigned int endLine, QObject *parent = nullptr);
    ~ReviewThread();

    void addReview(Review *review);

    std::vector<Review *> reviews();
    bool hasId(unsigned int id);

    /**
     * Returns the path of the file.
     */
    QString path() const;

    /**
     * Sets the file path for this review
     */
    void setPath(QString path);

    /**
     * Line of the thread in the source file
     */
    int line() const;

    /**
     * Sets the line on which the threads is
     */
    void setLine(int line);

    unsigned int startLine() const;
    unsigned int endLine() const;

    QList<Review *> comments() const;

    /**
     * Original line of the thread
     */
    int originalLine() const;

    /**
     * @returns If the review is outdated
     */
    bool isOutdated() const;

signals:
    void pathChanged(QString path);
    void lineChanged(QString line);

    void startLineChanged(unsigned int startLine);
    void endLineChanged(unsigned int endLine);

    /**
     * Notifies if ::comments has changed
     */
    void commentsChanged(QList<Review*> comment);

    void originalLineChanged(int originalLine);

private:
    QString _path;

    int _line; //< Line that the review is made. 0 in case is outdated

    unsigned int _startLine;
    unsigned int _endLine;

    int _originalLine; //< Original line of the review. This doesn't dissapear

    std::vector<Review *> childs;
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
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QUrl url READ url CONSTANT)
    Q_PROPERTY(QUrl htmlUrl READ htmlUrl CONSTANT)
    Q_PROPERTY(QUrl issuesUrl READ issuesUrl CONSTANT)
    Q_PROPERTY(QUrl reviewCommentsUrl READ reviewCommentsUrl CONSTANT)
    Q_PROPERTY(QUrl commentsUrl READ commentsUrl CONSTANT)
    Q_PROPERTY(State state READ state CONSTANT)
    Q_PROPERTY(QDateTime createdAt READ createdAt CONSTANT)
    Q_PROPERTY(QDateTime updatedAt READ updatedAt CONSTANT)

    Q_PROPERTY(QString sourceRef READ sourceRef NOTIFY sourceRefChanged)
    Q_PROPERTY(QString targetRef READ targetRef NOTIFY targetRefChanged)

    Q_PROPERTY(QHash<QPair<QString, int>, ReviewThread*> fileThreads READ fileThreads NOTIFY fileThreadsChanged)

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

    QString sourceRef() const;
    QString targetRef() const;

    /**
     * A map of (filename, line) -> thread for this pull request
     */
    QHash<QPair<QString, int>, ReviewThread*> fileThreads() const;


public slots:
    /**
     * Load the threads from a response
     * @param threadsDocument The json of the response
     */
    void loadThreads(QByteArray threadsDocument);

    /**
     * Returns the lines of threads for a given path
     */
    QList<int> reviewThreadLines(QString path);

    /**
     * Return a ReviewThread for path
     * @returns reviewThread
     */
    ReviewThread *reviewThread(QString path, int line);

    ReviewThreadModel *reviewThreadModel(QString path, int line);


signals:
    void titleChanged(QString title);
    void numberChanged(int number);
    void descriptionChanged(QString description);
    void sourceRefChanged(QString ref);
    void targetRefChanged(QString ref);

    void fileThreadsChanged(QHash<QString, QList<ReviewThread*>> threads);

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

    QString _sourceRef;
    QString _targetRef;

    QHash<QPair<QString, int>, ReviewThread*> _threads;

    QHash<QPair<QString, int>, ReviewThreadModel*> _reviewThreadModels;
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
    Q_PROPERTY(QUrl userImage READ userImage NOTIFY userImageChanged)

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
    QUrl userImage() const;

signals:
    void idChanged(int id);
    void nodeIdChanged(QString nodeId);
    void createdAtChanged(QDateTime createdAt);
    void updatedAtChanged(QDateTime updatedAt);
    void descriptionChanged(QString description);
    void usernameChanged(QString username);
    void userImageChanged(QUrl userImage);

private:
    unsigned int _id;
    QString _nodeId;

    QDateTime _createdAt;
    QDateTime _updatedAt;

    /// body
    QString _description;

    std::unique_ptr<User> _user;
};

class File : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString sha READ sha CONSTANT)
    Q_PROPERTY(QString filename READ filename CONSTANT)
    Q_PROPERTY(QString status READ status CONSTANT)
    Q_PROPERTY(int additions READ additions CONSTANT)
    Q_PROPERTY(int deletions READ deletions CONSTANT)
    Q_PROPERTY(int changes READ changes CONSTANT)
    Q_PROPERTY(QString patch READ patch CONSTANT)

    QML_ELEMENT
public:
    File(QObject *parent = nullptr);
    File(QJsonDocument &document, QObject *parent = nullptr);
    ~File();

    QString sha() const;
    QString filename() const;
    QString status() const;
    int additions() const;
    int deletions() const;
    int changes() const;
    QString patch() const;

signals:
    void shaChanged(QString sha);
    void filenameChanged(QString filename);
    void statusChanged(QString status);
    void additionsChanged(int additions);
    void deletionsChanged(int deletions);
    void changesChanged(int changes);
    void patchChanged(QString patch);

private:
    QString _sha;
    QString _filename;
    QString _status;
    int _additions;
    int _deletions;
    int _changes;
    QString _patch;
};

#endif
