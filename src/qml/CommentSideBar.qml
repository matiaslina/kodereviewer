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
                QQC2.ScrollBar.vertical.policy: QQC2.ScrollBar.AsNeeded
                QQC2.ScrollBar.horizontal.policy: QQC2.ScrollBar.AlwaysOff
                Layout.fillWidth: true
                Layout.fillHeight: true

                Kirigami.CardsListView {
                    id: cardsView
                    model: loader.model
                    delegate: CommentDelegate {}
                }
            }
        }
    }

    function loadReviewThreadModel(filename, line) {
        loader.model = root.pullRequest.reviewThreadModel(filename, line)
        root.openReviewThread = true
        drawerOpen = true
    }

    onEnabledChanged: drawerOpen = enabled
}
