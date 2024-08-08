import QtCore
import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami
import org.kde.kodereviewer.types


Kirigami.ApplicationWindow {
    id: root

    Settings {
        id: settings
        property alias width: root.width
        property alias height: root.height
        property alias drawerCollapsed: drawer.collapsed
        property string githubToken: ""
    }

    property string username: "matiaslina"
    property string repositoryName: "private-tests"
    property string repositoryRoot: "/home/matias/Workspace/miniaudio-test"
    property string token: settings.githubToken
    property PullRequest currentPullRequest

    /* Size settings */
    width: 600
    height: 400

    title: "Kode Reviewer"

    globalDrawer: ProjectDrawer {
        id: drawer
        repositoryRoot: root.repositoryRoot

        onPullRequestSelected: (pr) => {
            root.currentPullRequest = pr
        }
    }

    pageStack.initialPage: Kirigami.Page {
        QQC2.Label {
            anchors.centerIn: parent
            text: "Select a pull request :)"
        }
    }

    Component {
        id: pullRequestOverviewPage
        PullRequestOverviewPage {
            pullRequest: currentPullRequest
        }
    }

    onCurrentPullRequestChanged: {
        print("Pushing pull request!!")
        pageStack.replace(pullRequestOverviewPage)
    }

    Component.onCompleted: {
        NetworkManager.setUrl(root.username, root.repositoryName)
        drawer.loadPullRequests()
    }
}
