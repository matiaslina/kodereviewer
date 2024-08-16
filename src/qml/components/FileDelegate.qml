import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QQC2
import QtQuick.Templates 2.15 as T
import org.kde.kirigami 2.20 as Kirigami

RowLayout {
    id: root
    required property T.AbstractButton itemDelegate

    readonly property alias labelItem: labelItem

    spacing: Kirigami.Units.smallSpacing

    Rectangle {
        id: rect
        visible: true
        Layout.alignment: Qt.AlignVCenter
        color: root.itemDelegate.status == "modified" ? "orange"
            : root.itemDelegate.status == "deleted" ? "red"
            : root.itemDelegate.status == "added" ? "springgreen"
            : "?"
        Layout.preferredHeight: Kirigami.Units.iconSizes.smallMedium
        Layout.preferredWidth: Kirigami.Units.iconSizes.smallMedium
        radius: Kirigami.Units.smallSpacing
        Text {
            id: statusIcon
            anchors.centerIn: parent
            text: root.itemDelegate.status == "modified" ? "M"
                : root.itemDelegate.status == "deleted" ? "D"
                : root.itemDelegate.status == "added" ? "A"
                : "?"
        }
    }

    QQC2.Label {
        id: labelItem

        leftPadding: itemDelegate.mirrored ? (itemDelegate.indicator ? itemDelegate.indicator.width : 0) + itemDelegate.spacing : 0
        rightPadding: !itemDelegate.mirrored ? (itemDelegate.indicator ? itemDelegate.indicator.width : 0) + itemDelegate.spacing : 0

        text: root.itemDelegate.text
        font: root.itemDelegate.font
        color: root.itemDelegate.enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
        elide: Text.ElideRight
        visible: itemDelegate.text && itemDelegate.display !== QQC2.AbstractButton.IconOnly
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        Layout.alignment: Qt.AlignLeft
        Layout.fillWidth: true

        Accessible.ignored: true
    }
}
