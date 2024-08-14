import QtQuick 2.15 // Removing version break onCurrentItemChanged
import QtQuick.Layouts
import QtQuick.Controls as Controls

import org.kde.kirigamiaddons.delegates as Delegates
import org.kde.kitemmodels 1 as KItemModels
import org.kde.kirigami as Kirigami

import org.kde.kodereviewer

Kirigami.OverlayDrawer {
    id: drawer

    Kirigami.Theme.colorSet: Kirigami.Theme.View
    property color backgroundColor: Kirigami.Theme.backgroundColor
    required property PullRequestModel model
    readonly property int narrowWidth: Kirigami.Units.gridUnit * 3
    readonly property int largeWidth: Kirigami.Units.gridUnit * 15
    signal pullRequestSelected(PullRequest pullRequest)

    property bool isWide: true
    property bool changeWidth: true
    width: isWide ? largeWidth : narrowWidth

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    modal: Kirigami.Settings.isMobile

    closePolicy: modal ? Controls.PopupCloseOnReleaseOutside : Controls.Popup.NoAutoClose

    KItemModels.KSortFilterProxyModel {
        id: filterModel
        sourceModel: model
        filterRoleName: "title"
        sortRoleName: "number"
        sortOrder: Qt.AscendingOrder
    }

    contentItem: Rectangle {
        id: column

        Kirigami.Theme.backgroundColor: drawer.backgroundColor
        color: Kirigami.Theme.backgroundColor

        Controls.ToolBar {
            id: mainToolBar
            height: pageStack.globalToolBar.preferredHeight
            readonly property bool isWide: drawer.isWide
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right


            RowLayout {
                anchors.fill: parent
                spacing: 0

                Kirigami.SearchField {
                    id: searchField
                    Layout.fillWidth: true
                    visible: drawer.isWide
                    onTextChanged: {
                        filterModel.filterString = text
                    }
                }

                Controls.ToolButton {
                    visible: mainToolBar.isWide
                    Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
                    Controls.ToolTip.visible: hovered
                    Controls.ToolTip.text: "Refresh"
                    action: Kirigami.Action {
                        id: refreshAction
                        icon.name: "view-refresh"
                        onTriggered: {
                            print("Refreshing")
                        }
                    }
                }

                Controls.ToolSeparator {
                    visible: drawer.isWide
                    orientation: Qt.Vertical
                    Layout.fillHeight: true
                    Layout.margins: Kirigami.Units.smallSpacing
                }

                Controls.ToolButton {
                    visible: !Kirigami.Settings.isMobile
                    Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
                    Controls.ToolTip.visible: hovered
                    Controls.ToolTip.text: (mainToolBar.isWide
                                            ? i18n("Collapse sidebar")
                                            : i18n("Expand sidebar")
                                           ) + " (" + closeAction.shortcut + ")"
                    Layout.fillWidth: !mainToolBar.isWide

                    action: Kirigami.Action {
                        id: closeAction
                        icon.name: mainToolBar.isWide
                            ? "sidebar-collapse-left-symbolic"
                            : "sidebar-expand-symbolic"

                        onTriggered: {
                            drawer.close()
                        }
                    }
                }


            }
        }

        ColumnLayout {
            visible: drawer.isWide

            anchors.top: mainToolBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Controls.ScrollView {
                id: scrollView
                Layout.fillWidth: true
                Layout.fillHeight: true

                ListView {
                    id: view

                    //Kirigami.Theme.backgroundColor: drawer.backgroundColor
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    model: filterModel
                    clip: true
                    delegate: Delegates.RoundedItemDelegate {
                        required property int number
                        required property string title
                        required property int index
                        highlighted: ListView.isCurrentItem

                        text: `${number} - ${title}`
                        icon.name: "vcs-merge-request"
                        onClicked: {
                            view.currentIndex = index
                            const pr = model.get(index)
                            print(pr)
                            drawer.pullRequestSelected(pr)
                        }
                    }

                    onCurrentItemChanged: {
                    }
                }
            }
        }
    }
}
