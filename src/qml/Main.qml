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

    contextDrawer: Kirigami.OverlayDrawer {
        id: contextDrawer
        enabled: root.currentReviewFile && root.currentPage == "ReviewFilePage"
        width: Kirigami.Units.gridUnit * 20
        modal: false
        edge: Qt.application.layoutDirection == Qt.RightToLeft ? Qt.LeftEdge : Qt.RightEdge

        handleClosedIcon.source: "documentinfo-symbolic"
        handleClosedToolTip: i18nc("@action:button", "Show Room Information")

        // Default icon is fine, only need to override the tooltip text
        handleOpenToolTip: i18nc("@action:button", "Close Room Information Drawer")
        handleVisible: enabled


        leftPadding: 0
        rightPadding: 0
        topPadding: 0
        bottomPadding: 0

        contentItem: Loader {
            id: loader
            active: true
            sourceComponent: ColumnLayout {
                spacing: 0

                QQC2.ToolBar {
                    Layout.fillWidth: true

                    Layout.preferredHeight: pageStack.globalToolBar.preferredHeight

                    contentItem: RowLayout {
                        Kirigami.Heading {
                            Layout.fillWidth: true
                            text: "File Comments"
                        }
                    }
                }

                QQC2.ScrollView {
                    QQC2.ScrollBar.vertical.policy: QQC2.ScrollBar.AsNeeded
                    QQC2.ScrollBar.horizontal.policy: QQC2.ScrollBar.AlwaysOff
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Kirigami.CardsListView {
                        id: cardsView
                        model: root.currentPullRequest.reviewThreadModel(root.currentReviewFile.filename)
                        delegate: CommentDelegate {}
                    }
                }
            }
        }

        onEnabledChanged: drawerOpen = enabled
    }

    pageStack.initialPage: WelcomePage {
        onProjectSelected: (project) => {
            root.project = project
            root.connection.getPullRequests();
            root.pageStack.replace(pullRequestOverviewPage)
        }
    }

    Component {
        id: pullRequestOverviewPage
        PullRequestOverviewPage {
            visible: !!root.project
            pullRequest: root.currentPullRequest
            connection: root.connection
            git: root.gitBackend
        }
    }

    Component {
        id: reviewFilePage
        ReviewFilePage {
            visible: !!root.project
            pullRequest: root.currentPullRequest
            git: root.gitBackend
            file: root.currentReviewFile
            connection: root.connection
        }
    }

    onCurrentPullRequestChanged: {
        if (currentPullRequest) {
            gitBackend.sourceRef = currentPullRequest.sourceRef
            gitBackend.targetRef = currentPullRequest.targetRef
            root.connection.getPullRequestThreads(currentPullRequest.number)
        }
        currentPage = "PullRequestOverview"
        pageStack.replace(pullRequestOverviewPage);
    }

    onCurrentReviewFileChanged: {
        if (currentReviewFile && currentPage != "ReviewFilePage") {
            currentPage = "ReviewFilePage"
            pageStack.replace(reviewFilePage)
        }
    }

    function switchToReviewFile(file) {
        currentPage = "ReviewFilePage"
        currentReviewFile = file
        pageStack.replace(reviewFilePage)
    }

    function switchToPullRequestOverview() {
        currentPage = "PullRequestOverview"
        pageStack.replace(pullRequestOverviewPage)
    }
}
