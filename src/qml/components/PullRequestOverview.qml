import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami
import org.kde.kodereviewer

QQC2.ScrollView {

    id: root
    required property PullRequest pullRequest
    required property CommentModel model
    anchors.fill: parent

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: Kirigami.Units.largeSpacing

        Kirigami.Heading {
            level: 1
            text: root.pullRequest ? root.pullRequest.title : ""
        }

        Kirigami.Separator {
            Layout.fillWidth: true
        }

        QQC2.Label {
            Layout.fillHeight: false
            text: root.pullRequest ? root.pullRequest.description : ""
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
            visible: commentView.count > 0
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
