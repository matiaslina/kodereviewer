#include "apidata.h"
#include "reviewthreadmodel.h"
#include <memory>
#include <qjsondocument.h>
#include <qnamespace.h>
#include <qstringliteral.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
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
 *       PullRequestHead
 **************************/

PullRequestHead::PullRequestHead(QObject *parent)
    : QObject(parent)
    , _label("")
    , _ref("")
    , _sha("")
{
}

PullRequestHead::PullRequestHead(QJsonDocument &document, QObject *parent)
    : QObject(parent)
{
    this->_label = document["label"].toString();
    this->_ref = document["ref"].toString();
    this->_sha = document["sha"].toString();
}

PullRequestHead::~PullRequestHead() = default;

QString PullRequestHead::label() const { return _label; }
QString PullRequestHead::ref() const { return _ref; }
QString PullRequestHead::sha() const { return _sha; }

/***************************
 *       Pull Request
 **************************/
PullRequest::PullRequest(QObject *parent)
    : QObject(parent)
{
}

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
    QJsonDocument _d = QJsonDocument(head);
    this->_head = std::make_unique<PullRequestHead>(_d);

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
PullRequestHead *PullRequest::head() const
{
    return _head.get();
}

QString PullRequest::sourceRef() const { return _sourceRef; }
QString PullRequest::targetRef() const { return _targetRef; }

QHash<QPair<QString, int>, ReviewThread*> PullRequest::fileThreads() const
{
    return _threads;
}

ReviewThreadModel *PullRequest::reviewThreadModel(QString path, int line)
{
    QPair<QString, int> key(path, line);
    ReviewThreadModel *model = _reviewThreadModels[key];

    if (model == nullptr) {
        ReviewThread *rt = new ReviewThread(path, line, line);
        model = new ReviewThreadModel(rt);
        _reviewThreadModels[key] = model;
    }

    return model;
}

// slots

void PullRequest::loadThreads(QByteArray threadsDocument)
{
    QJsonDocument json = QJsonDocument::fromJson(threadsDocument);

    if (!json.isArray()) {
        qDebug() << "loadThreads: json is not an array";
        return;
    }

    for (auto rt : std::as_const(_threads)) {
        delete rt;
        rt = nullptr;
    }
    _threads.clear();

    QJsonArray threads = json.array();
    for (auto ref : std::as_const(threads)) {
        auto document = QJsonDocument(ref.toObject());
        ReviewThread *rt = nullptr;
        QString path = document["path"].toString();
        int line = document["line"].toInt();
        int startLine = document["start_line"].toInt();
        int originalLine = document["original_line"].toInt();

        if (startLine == 0) {
            startLine = line;
        }

        QPair<QString, int> key(path, line);

        if (!_threads.contains(key)) {
            rt = new ReviewThread(path, line, originalLine, startLine, line);
            qDebug() << "Adding review thread model";
            _reviewThreadModels[key] = new ReviewThreadModel(rt);
        } else {
            rt = _threads[key];
        }
        Review *r = new Review(document);
        rt->addReview(r);

        _threads[key] = rt;
    }
}

ReviewThread *PullRequest::reviewThread(QString path, int line)
{
    QPair<QString, int> key(path, line);
    if (_threads.contains(key)) {
        return _threads[key];
    }
    qDebug() << "Unknown path " << path;
    return nullptr;
}

QList<int> PullRequest::reviewThreadLines(QString path)
{
    QList<int> retval;

    QList<QPair<QString, int>> keys = _threads.keys();
    for(const auto &pair : std::as_const(keys)) {
        if (pair.first == path) {
            ReviewThread *thread = _threads[pair];
            if (!thread->isOutdated()) {
                retval << pair.second;
            }
        }
    }
    return retval;
}

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

Review::Review(QObject *parent)
    : QObject(parent)
{}

Review::Review(QJsonDocument &doc, QObject *parent)
    : QObject(parent)
{
    this->_id = doc[QStringLiteral("id")].toInt();
    this->_pullRequestReviewId = doc[QStringLiteral("pull_request_review_id")].toInt();
    this->_nodeId = doc[QStringLiteral("node_id")].toString();
    this->_diffHunk = doc[QStringLiteral("diff_hunk")].toString();
    this->_path = doc[QStringLiteral("path")].toString();
    this->_line = doc[QStringLiteral("line")].toInt();
    this->_description = doc[QStringLiteral("body")].toString();
    this->_createdAt = QDateTime::fromString(doc[QStringLiteral("created_at")].toString(), Qt::ISODate);
    this->_updatedAt = QDateTime::fromString(doc[QStringLiteral("updated_at")].toString(), Qt::ISODate);
    this->_commitId = doc["commit_id"].toString();

    QJsonDocument userDocument(doc[QStringLiteral("user")].toObject());
    this->_user = std::make_unique<User>(userDocument);

    auto in_reply_to = doc[QStringLiteral("in_reply_to")];
    if (in_reply_to != QJsonValue::Undefined) {
        this->_inReplyTo = in_reply_to.toInt();
    }
}

Review::~Review() = default;

int Review::id() const { return _id; }
QString Review::nodeId() const { return _nodeId; }
QString Review::description() const { return _description; }
QDateTime Review::createdAt() const { return _createdAt; }
QDateTime Review::updatedAt() const { return _updatedAt; }

QString Review::username() const { return _user->username; }
QUrl Review::avatarUrl() const { return _user->avatarUrl; }

QString Review::commitId() const
{
    return _commitId;
}

QString Review::diffHunk() const
{
    return _diffHunk;
}

QJsonObject Review::toObject()
{
    return {
        { QStringLiteral("id"), QJsonValue((int) _id) },
        { QStringLiteral("diffHunk"), QJsonValue(_diffHunk) },
        { QStringLiteral("path"), _path },
        { QStringLiteral("user"), _user->toObject() },
        { QStringLiteral("body"), QJsonValue(_description) },
        { QStringLiteral("createdAt"), QJsonValue(_createdAt.toString()) },
        { QStringLiteral("in_reply_to_id"), QJsonValue((int) _inReplyTo) },
        { QStringLiteral("originalStartLine"), QJsonValue((int) _line) },
        { QStringLiteral("startLine"), QJsonValue((int) _line) }
    };
}


/***************************
 *       ReviewThread
 **************************/
ReviewThread::ReviewThread(QObject *parent)
    : QObject(parent)
{
}

ReviewThread::ReviewThread(QString &path, int line, int originalLine, QObject *parent)
    : QObject(parent)
    , _path(path)
    , _line(line)
    , _startLine(line)
    , _endLine(line)
    , _originalLine(originalLine)
{}

ReviewThread::ReviewThread(QString &path, int line, int originalLine, unsigned int startLine, unsigned int endLine, QObject *parent)
    : QObject(parent)
    , _path(path)
    , _line(line)
    , _startLine(startLine)
    , _endLine(endLine)
    , _originalLine(originalLine)
{}

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
        if (review->id() == (int) id) {
            return true;
        }
    }
    return false;
}

void ReviewThread::setPath(QString path)
{
    _path = path;
}

QString ReviewThread::path() const
{
    return _path;
}

void ReviewThread::setLine(int line)
{
    _line = line;
}

int ReviewThread::line() const
{
    return _line;
}


unsigned int ReviewThread::startLine() const
{
    return _startLine;
}

unsigned int ReviewThread::endLine() const
{
    return _endLine;
}


QList<Review *> ReviewThread::comments() const
{
    QList<Review *> retval;

    for (auto r : childs) {
        retval << r;
    }

    return retval;
}

int ReviewThread::originalLine() const
{
    return _originalLine;
}

QString ReviewThread::diffHunk() const
{
    if (childs.size() > 0) {
        return childs[0]->diffHunk();
    }
    return "";
}

bool ReviewThread::isOutdated() const
{
    return line() == 0;
}

/***************************
 *       File
 **************************/

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
