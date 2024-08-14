import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami
import org.kde.kodereviewer

ColumnLayout {
    id: root
    required property PullRequest pullRequest
    required property CommentModel model

    QQC2.ScrollView {
        Layout.fillWidth: true
        Layout.fillHeight: true
        ColumnLayout {
            id: mainLayout
            anchors.fill: parent
            anchors.margins: Kirigami.Units.largeSpacing

            Kirigami.Heading {
                level: 1
                text: root.pullRequest.title
            }
            Kirigami.Separator {
                Layout.fillWidth: true
            }

            Text {
                Layout.fillHeight: false
                text: root.pullRequest.description
                textFormat: Text.MarkdownText
            }

            Kirigami.Separator {
                Layout.fillWidth: true
            }

            Kirigami.Heading {
                level: 2
                text: "Comments"
            }


            Repeater {
                id: commentView
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: root.model
                delegate: CommentDelegate {}
            }


            Item {
                visible: commentView.count === 0
                Layout.fillHeight: true
                Layout.fillWidth: true
                ColumnLayout {
                    anchors.centerIn: parent
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "Loading comments..."
                    }
                    QQC2.BusyIndicator {
                        Layout.alignment: Qt.AlignHCenter
                    }
                }
            }

        }
    }

    CommentControl {
        Layout.fillWidth: true
        Layout.fillHeight: false
    }
}
