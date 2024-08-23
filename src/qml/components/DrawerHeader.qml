import QtQuick 2.15 // Removing version break onCurrentItemChanged
import QtQuick.Layouts
import QtQuick.Controls as Controls

import org.kde.kirigamiaddons.delegates as Delegates
import org.kde.kitemmodels 1 as KItemModels
import org.kde.kirigami as Kirigami

import org.kde.kodereviewer

RowLayout {
    id: root
    required property Kirigami.OverlayDrawer drawer
    required property Controls.ToolBar toolbar

    signal searchChanged(string text)

    anchors.fill: parent

    Controls.ToolButton {
        visible: !root.drawer.collapsed
        Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
        Controls.ToolTip.visible: hovered
        Controls.ToolTip.text: "Pull Requests"
        action: Kirigami.Action {
            id: pullRequestListAction
            icon.name: "vcs-merge-request"
            onTriggered: {
                root.drawer.switchToPullRequestList()
            }
        }
    }

    Kirigami.SearchField {
        id: searchField
        Layout.fillWidth: true
        visible: root.toolbar.isWide
        onTextChanged: {
            root.searchChanged(text)
        }
    }

    Controls.ToolButton {
        visible: root.toolbar.isWide
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
}
