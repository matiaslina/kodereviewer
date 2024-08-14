import QtCore
import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami

import org.kde.kodereviewer


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

    property NetworkManager connection: NetworkManager {
        owner: root.username
        repo: root.repositoryName

        onPullRequestFinished: (jsonResponse) => {
            pullRequestModel.loadData(jsonResponse)
        }
        onErrorOcurred: (err) => {
            console.log(err);
        }
    }

    /* Size settings */
    width: 600
    height: 400

    title: "Kode Reviewer"

    PullRequestModel {
        id: pullRequestModel
    }

    GitBackend {
        id: gitBackend
        path: root.repositoryRoot
        sourceRef: root.currentPullRequest ? root.currentPullRequest.sourceRef : ""
        targetRef: root.currentPullRequest ? root.currentPullRequest.targetRef : ""
    }

    globalDrawer: ProjectDrawer {
        id: drawer
        model: pullRequestModel ? pullRequestModel : undefined
        onPullRequestSelected: pr => {
            print(pr.title);
            root.currentPullRequest = pr;
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
            pullRequest: root.currentPullRequest
            connection: root.connection
            git: gitBackend
        }
    }

    onCurrentPullRequestChanged: {
        if (currentPullRequest) {
            gitBackend.sourceRef = currentPullRequest.sourceRef
            gitBackend.targetRef = currentPullRequest.targetRef
        }
        pageStack.replace(pullRequestOverviewPage);
    }

    Component.onCompleted: {
        connection.getPullRequests();
        console.info(gitBackend.path);
    }
}
