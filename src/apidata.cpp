#include "apidata.h"
#include <memory>
#include <qjsondocument.h>
#include <qnamespace.h>
#include <qstringliteral.h>
#include <QJsonObject>
#include <QDateTime>


/***************************
 *       User
 **************************/
User::User(QJsonDocument &document)
{
    this->username = document[QStringLiteral("login")].toString();
    this->avatarUrl = QUrl(document[QStringLiteral("avatar_url")].toString());
}

QJsonObject User::toObject()
{
    return {
        { QStringLiteral("login"), username },
        { QStringLiteral("avatarUrl"), avatarUrl.toString() }
    };
}

/***************************
 *       Pull Request
 **************************/
PullRequest::PullRequest(QObject *parent)
    : QObject(parent)
{}

PullRequest::PullRequest(QJsonDocument &document, QObject *parent)
    : QObject(parent)
{
    this->_id = document[QStringLiteral("id")].toInt();
    this->_url = QUrl(document[QStringLiteral("url")].toString());
    this->_htmlUrl =QUrl(document[QStringLiteral("html_url")].toString());
    this->_issuesUrl = QUrl(document[QStringLiteral("issue_url")].toString());
    this->_reviewCommentsUrl = QUrl(document[QStringLiteral("review_comments_url")].toString());
    this->_commentsUrl = QUrl(document[QStringLiteral("comments_url")].toString());

    this->_number = document[QStringLiteral("number")].toInt();

    QString jsonState = document[QStringLiteral("state")].toString();
    if (jsonState == QStringLiteral("open")) {
        this->_state = State::OPEN;
    } else if (jsonState == QStringLiteral("closed")) {
        this->_state = State::CLOSED;
    } else {
        qDebug() << "Unknown state: " << jsonState;
    }
    this->_title = document[QStringLiteral("title")].toString();

    auto userDocument = QJsonDocument(document[QStringLiteral("user")].toObject());
    this->_user = std::make_unique<User>(userDocument);

    this->_description = document[QStringLiteral("body")].toString();

    this->_createdAt = QDateTime::fromString(document[QStringLiteral("created_at")].toString(),
                                            Qt::ISODate);
    this->_updatedAt = QDateTime::fromString(document[QStringLiteral("updated_at")].toString(),
                                            Qt::ISODate);

    auto head = document[QStringLiteral("head")].toObject();
    _sourceRef = head[QStringLiteral("ref")].toString();

    auto base = document[QStringLiteral("base")].toObject();
    _targetRef = base[QStringLiteral("ref")].toString();
}

PullRequest::~PullRequest() = default;

QString PullRequest::title() const {
    return this->_title;
}

unsigned int PullRequest::id() const { return _id; }

QUrl PullRequest::url() const { return _url; }
QUrl PullRequest::htmlUrl() const { return _htmlUrl; }
QUrl PullRequest::issuesUrl() const { return _issuesUrl; }
QUrl PullRequest::reviewCommentsUrl() const { return _reviewCommentsUrl; }
QUrl PullRequest::commentsUrl() const { return _commentsUrl; }

unsigned int PullRequest::number() const { return _number; }
PullRequest::State PullRequest::state() const { return _state; }

User *PullRequest::user() const { return _user.get(); }

/// body
QString PullRequest::description() const { return _description; }

QDateTime PullRequest::createdAt() const { return _createdAt; }
QDateTime PullRequest::updatedAt() const { return _updatedAt; }

QString PullRequest::sourceRef() const { return _sourceRef; }
QString PullRequest::targetRef() const { return _targetRef; }

/***************************
 *       Comments
 **************************/

Comment::Comment(QObject *parent)
    : QObject(parent)
{}

Comment::Comment(QJsonDocument &doc, QObject *parent)
    : QObject(parent)
{
    this->_id = doc[QStringLiteral("id")].toInt();
    this->_nodeId = doc[QStringLiteral("node_id")].toString();
    this->_createdAt = QDateTime::fromString(doc[QStringLiteral("created_at")].toString(),
                                            Qt::ISODate);
    this->_updatedAt = QDateTime::fromString(doc[QStringLiteral("updated_at")].toString(),
                                            Qt::ISODate);
    this->_description = doc[QStringLiteral("body")].toString();
    QJsonDocument userDocument(doc[QStringLiteral("user")].toObject());
    this->_user = std::make_unique<User>(userDocument);
}

Comment::~Comment() = default;

unsigned int Comment::id() const { return _id; }
QString Comment::nodeId() const { return _nodeId; }
QDateTime Comment::createdAt() const { return _createdAt; }
QDateTime Comment::updatedAt() const { return _updatedAt; }
QString Comment::description() const { return _description; }
QString Comment::username() const { return _user.get()->username; }
QUrl Comment::userImage() const {
    return _user.get()->avatarUrl;
}


/***************************
 *       Review
 **************************/
Review::Review(QJsonDocument &doc)
{
    this->id = doc[QStringLiteral("id")].toInt();
    this->pullRequestReviewId = doc[QStringLiteral("pull_request_review_id")].toInt();
    this->nodeId = doc[QStringLiteral("node_id")].toString();
    this->diffHunk = doc[QStringLiteral("diff_hunk")].toString();
    this->path = doc[QStringLiteral("path")].toString();
    this->line = doc[QStringLiteral("line")].toInt();
    this->description = doc[QStringLiteral("body")].toString();
    this->createdAt = QDateTime::fromString(doc[QStringLiteral("created_at")].toString(), Qt::ISODate);
    this->updatedAt = QDateTime::fromString(doc[QStringLiteral("updated_at")].toString(), Qt::ISODate);

    QJsonDocument userDocument(doc[QStringLiteral("user")].toObject());
    this->user = std::make_unique<User>(userDocument);

    auto in_reply_to = doc[QStringLiteral("in_reply_to")];
    if (in_reply_to != QJsonValue::Undefined) {
        this->inReplyTo = in_reply_to.toInt();
    }
}

Review::~Review() = default;


QJsonObject Review::toObject()
{
    return {
        { QStringLiteral("id"), QJsonValue((int) id) },
        { QStringLiteral("diffHunk"), QJsonValue(diffHunk) },
        { QStringLiteral("path"), path },
        { QStringLiteral("user"), user->toObject() },
        { QStringLiteral("body"), QJsonValue(description) },
        { QStringLiteral("createdAt"), QJsonValue(createdAt.toString()) },
        { QStringLiteral("in_reply_to_id"), QJsonValue((int) inReplyTo) },
        { QStringLiteral("originalStartLine"), QJsonValue((int) line) },
        { QStringLiteral("startLine"), QJsonValue((int) line) }
    };
}


/***************************
 *       ReviewThread
 **************************/
ReviewThread::ReviewThread()
{
}

ReviewThread::~ReviewThread() = default;

void ReviewThread::addReview(Review *review)
{
    childs.push_back(review);
}

std::vector<Review*> ReviewThread::reviews()
{
    return childs;
}

bool ReviewThread::hasId(unsigned int id)
{
    for (Review *review : reviews()) {
        if (review->id == id) {
            return true;
        }
    }
    return false;
}

int ReviewThread::line()
{
    if (childs.size() > 0) {
        return childs[0]->line;
    }
    return -1;
}

File::File(QObject *parent)
    : QObject(parent)
{}

File::File(QJsonDocument &doc, QObject *parent)
    : QObject(parent)
{
    this->_sha = doc["sha"].toString();
    this->_filename = doc["filename"].toString();
    this->_status = doc["status"].toString();
    this->_additions = doc["additions"].toInt();
    this->_deletions = doc["deletions"].toInt();
    this->_changes = doc["changes"].toInt();
    this->_patch = doc["patch"].toString();
}

File::~File() = default;

QString File::sha() const { return _sha; }
QString File::filename() const { return _filename; }
QString File::status() const { return _status; }
int File::additions() const { return _additions; }
int File::deletions() const { return _deletions; }
int File::changes() const { return _changes; }
QString File::patch() const { return _patch; }
