#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <QQuickStyle>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <KIconTheme>
#include <qqml.h>

#include "pullrequestmodel.h"
#include "commentsmodel.h"
#include "networkmanager.h"
#include "linemodel.h"
#include "apidata.h"

int main(int argc, char *argv[])
{
    KIconTheme::initTheme();
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("tutorial");
    QApplication::setOrganizationName(QStringLiteral("KDE"));
    QApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QApplication::setApplicationName(QStringLiteral("Kode Reviwere"));
    QApplication::setDesktopFileName(QStringLiteral("org.kde.kode-reviewer"));

    QApplication::setStyle(QStringLiteral("breeze"));
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }

    QSettings settings;
    QQmlApplicationEngine engine;

    qmlRegisterType<NetworkManager>("org.kde.kodereviewer.network", 1, 0, "NetworkManager");
    engine.singletonInstance<NetworkManager*>("org.kde.kodereviewer.network", "NetworkManager");

    qmlRegisterType<PullRequestModel>("org.kde.kodereviewer.models", 1, 0, "PullRequestModel");
    qmlRegisterType<CommentModel>("org.kde.kodereviewer.models", 1, 0, "CommentModel");
    qmlRegisterType<LineModel>("org.kde.kodereviewer.models", 1, 0, "LineModel");
    qmlRegisterType<PullRequest>("org.kde.kodereviewer.types", 1, 0, "PullRequest");

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.loadFromModule("org.kde.kodereviewer", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
