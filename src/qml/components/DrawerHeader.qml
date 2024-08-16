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
    spacing: 0

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

    Controls.ToolSeparator {
        visible: root.toolbar.isWide
        orientation: Qt.Vertical
        Layout.fillHeight: true
        Layout.margins: Kirigami.Units.smallSpacing
    }

    Controls.ToolButton {
        visible: !Kirigami.Settings.isMobile
        Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
        Controls.ToolTip.visible: hovered
        Controls.ToolTip.text: (root.toolbar.isWide
                                ? i18n("Collapse sidebar")
                                : i18n("Expand sidebar")
                               ) + " (" + closeAction.shortcut + ")"
        Layout.fillWidth: !root.toolbar.isWide

        action: Kirigami.Action {
            id: closeAction
            icon.name: root.toolbar.isWide
                ? "sidebar-collapse-left-symbolic"
                : "sidebar-expand-symbolic"

            onTriggered: {
                root.drawer.close()
            }
        }
    }
}
