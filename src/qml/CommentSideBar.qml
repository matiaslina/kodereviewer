pragma ComponentBehavior: Bound
import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami

import org.kde.kodereviewer

Kirigami.OverlayDrawer {
    id: root

    property int headerHeight
    property string currentPage: ""
    required property PullRequest pullRequest
    required property File reviewFile
    property NetworkManager connection: applicationWindow().connection

        Kirigami.Theme.colorSet: Kirigami.Theme.View
    Kirigami.Theme.inherit: false

    /**
     * Holds which line it's beign reviewed
     */
    property int line: 0

    /**
     * Holds the hunk to show in the header
     */
    property string diffHunk: ""

    /**
     * Holds if there's an open review thread
     */
    property bool openReviewThread: false

    enabled: root.reviewFile && root.currentPage == "ReviewFilePage" && root.openReviewThread
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
        active: enabled
        property var model: []
        sourceComponent: ColumnLayout {
            spacing: 0

            QQC2.ToolBar {
                Layout.fillWidth: true

                Layout.preferredHeight: root.headerHeight

                contentItem: RowLayout {
                    Kirigami.Heading {
                        Layout.fillWidth: true
                        text: "File Comments"
                    }
                }
            }

            QQC2.ScrollView {
                id: scrollView
                QQC2.ScrollBar.vertical.policy: QQC2.ScrollBar.AsNeeded
                QQC2.ScrollBar.horizontal.policy: QQC2.ScrollBar.AlwaysOff
                Layout.fillWidth: true
                Layout.fillHeight: true

                Kirigami.CardsListView {
                    id: cardsView
                    model: loader.model
                    delegate: CommentDelegate {}
                    clip: true
                    headerPositioning: ListView.InlineHeader
                    header: ColumnLayout {
                        anchors.left: parent.left
                        anchors.right: parent.right

                        SourceCodeText {
                            Layout.bottomMargin: Kirigami.Units.largeSpacing
                            Layout.topMargin: Kirigami.Units.largeSpacing
                            visible: root.diffHunk != ""
                            text: root.diffHunk
                        }
                    }
                }

                Kirigami.PlaceholderMessage {
                    visible: cardsView.count === 0
                    anchors.centerIn: parent
                    text: "New comment"
                }
            }

            CommentControl {
                Layout.fillWidth: true
                Layout.fillHeight: false
                onCommentSent: comment => {
                    const id = loader.model.getThreadId()
                    if (id !== 0) {
                        root.connection.sendThreadComment(root.pullRequest.number, id, comment)
                    } else {
                        // Create a new review
                        root.connection.createThread(
                            root.pullRequest.number,
                            comment,
                            root.pullRequest.head.sha,
                            root.reviewFile.filename,
                            1
                        )
                    }
                }
            }
        }
    }

    function loadReviewThreadModel(filename, line) {
        const reviewThreadModel = root.pullRequest.reviewThreadModel(filename, line)
        loader.model = reviewThreadModel
        root.line = line
        root.openReviewThread = true
        root.diffHunk = reviewThreadModel.diffHunk()
        drawerOpen = true
    }

    Connections {
        target: root.connection
        function onSendThreadCommentFinished(response) {
            loader.model.addComment(response)
        }

        function onCreateThreadFinished(response) {
            print("Create thread finished? ")
            loader.model.addComment(response)
        }
    }

    onEnabledChanged: drawerOpen = enabled
}
