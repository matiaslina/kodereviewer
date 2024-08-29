pragma ComponentBehavior: Bound
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

    property Project project : Project {}
    property string username: project.owner
    property string repositoryName: project.name
    property string repositoryRoot: project.path
    property string token: settings.githubToken
    property PullRequest currentPullRequest
    property File currentReviewFile: File {}
    property string currentPage: ""

    property NetworkManager connection: NetworkManager {
        owner: root.project.owner
        repo: root.project.name

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

    Component {
        id: settingsPage
        SettingsPage {}
    }

    PullRequestModel {
        id: pullRequestModel
    }

    property GitBackend gitBackend: GitBackend {
        path: root.repositoryRoot
        sourceRef: root.currentPullRequest ? root.currentPullRequest.sourceRef : ""
        targetRef: root.currentPullRequest ? root.currentPullRequest.targetRef : ""
    }

    Kirigami.PagePool {
        id: pagePool
    }

    globalDrawer: ProjectDrawer {
        id: drawer
        window: root
        currentProject: root.project
        model: pullRequestModel ? pullRequestModel : undefined
        connection: root.connection
        onPullRequestSelected: pr => {
            if (!root.currentPullRequest || root.currentPullRequest.number != pr.number) {
                root.currentPullRequest = pr
            }
        }

        onFileSelected: file => {
            root.currentReviewFile = file
        }
    }

    contextDrawer: CommentSideBar {
        id: commentSideBar
        currentPage: root.currentPage
        headerHeight: root.pageStack.globalToolBar.preferredHeight
        pullRequest: root.currentPullRequest
        reviewFile: root.currentReviewFile
    }

    pageStack.initialPage: WelcomePage {
        onProjectSelected: (project) => {
            root.project = project
            root.connection.getPullRequests()
            switchToPullRequestOverview()
        }
    }
    onCurrentPullRequestChanged: {
        if (currentPullRequest) {
            gitBackend.sourceRef = currentPullRequest.sourceRef
            gitBackend.targetRef = currentPullRequest.targetRef
            root.connection.getPullRequestThreads(currentPullRequest.number)
        }
    }

    onCurrentReviewFileChanged: {
        if (currentReviewFile && currentPage != "ReviewFilePage") {
            currentPage = "ReviewFilePage"
            switchToReviewFile(currentReviewFile)
        }
    }

    function switchToReviewFile(file) {
        currentPage = "ReviewFilePage"
        currentReviewFile = file

        const page = pagePool.loadPage("qrc:qml/ReviewFilePage.qml")
        pageStack.replace(page)
    }

    function switchToPullRequestOverview() {
        currentPage = "PullRequestOverview"
        const page = pagePool.loadPage("qrc:qml/PullRequestOverviewPage.qml")
        pageStack.replace(page);
    }

    function openCommentSideBar(path, line) {
        commentSideBar.loadReviewThreadModel(path, line)
    }
}
