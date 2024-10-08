#include <KIconTheme>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QSettings>
#include <QStandardPaths>
#include <QUrl>
#include <QIcon>
#include <qgit2.h>
#include <qqml.h>
#include <QQmlContext>
#include <qstandardpaths.h>

#include "pullrequestmodel.h"
#include "commentsmodel.h"
#include "filemodel.h"
#include "networkmanager.h"
#include "linemodel.h"
#include "projectmodel.h"
#include "diffmodel.h"
#include "apidata.h"
#include "gitbackend.h"
#include "qgit2/qgitglobal.h"


int main(int argc, char *argv[])
{
    KIconTheme::initTheme();
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("tutorial");
    QApplication::setOrganizationName(QStringLiteral("KDE"));
    QApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QApplication::setApplicationName(QStringLiteral("Kode Reviewer"));
    QApplication::setDesktopFileName(QStringLiteral("kode-reviewer"));

    QApplication::setStyle(QStringLiteral("breeze"));
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }

    LibQGit2::initLibQGit2();

    QSettings settings;
    QQmlApplicationEngine engine;

    qmlRegisterType<NetworkManager>("org.kde.kodereviewer", 1, 0, "NetworkManager");
    qmlRegisterType<PullRequestModel>("org.kde.kodereviewer", 1, 0, "PullRequestModel");
    qmlRegisterType<CommentModel>("org.kde.kodereviewer", 1, 0, "CommentModel");
    qmlRegisterType<FileModel>("org.kde.kodereviewer", 1, 0, "FileModel");
    qmlRegisterType<LineModel>("org.kde.kodereviewer", 1, 0, "LineModel");
    qmlRegisterType<ProjectModel>("org.kde.kodereviewer", 1, 0, "ProjectModel");
    qmlRegisterType<DiffModel>("org.kde.kodereviewer", 1, 0, "DiffModel");
    qmlRegisterType<ReviewThreadModel>("org.kde.kodereviewer", 1, 0, "ReviewThreadModel");


    qmlRegisterType<PullRequest>("org.kde.kodereviewer", 1, 0, "PullRequest");
    qmlRegisterType<Project>("org.kde.kodereviewer", 1, 0, "Project");
    qmlRegisterType<File>("org.kde.kodereviewer", 1, 0, "File");

    qmlRegisterType<GitBackend>("org.kde.kodereviewer", 1, 0, "GitBackend");

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.loadFromModule("org.kde.kodereviewer", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }


    return app.exec();
}
