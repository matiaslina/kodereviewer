import QtQuick 6.7
import QtQuick.Layouts 6.7
import QtQuick.Controls 6.7 as QQC2

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.components as KirigamiComponents

Kirigami.AbstractCard {
    id: root
    required property url userImage
    required property string createdAt
    required property string username
    required property string description

    contentItem: Item {
        implicitWidth: delegateLayout.implicitWidth
        implicitHeight: delegateLayout.implicitHeight
        ColumnLayout {
            id: delegateLayout
            spacing: Kirigami.Units.largeSpacing
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            RowLayout {
                KirigamiComponents.Avatar {
                    name: root.username
                    source: root.userImage
                    Layout.preferredWidth: Kirigami.Units.iconSizes.smallMedium
                    Layout.preferredHeight: Kirigami.Units.iconSizes.smallMedium
                }
                Kirigami.Heading {
                    id: commentHeading
                    level: 3
                    text: `@${root.username}`
                }
                QQC2.Label {
                    text: root.createdAt
                }
            }
            Kirigami.Separator {
                Layout.fillWidth: true
            }
            QQC2.Label {
                Layout.fillWidth: true
                text: root.description
                textFormat: Text.MarkdownText
                wrapMode: Text.WordWrap
            }
        }
    }
}
