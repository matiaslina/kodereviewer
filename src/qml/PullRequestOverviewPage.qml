import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami
import org.kde.kodereviewer.models
import org.kde.kodereviewer.types


Kirigami.ScrollablePage {
    id:root
    required property PullRequest pullRequest

    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Kirigami.Theme.inherit: false

    CommentModel {
        id: commentModel
    }

    Component {
        id: commentDelegate
        Kirigami.AbstractCard {
            contentItem: Item {
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight
                ColumnLayout {
                    id: delegateLayout
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    RowLayout {
                        Kirigami.Heading {
                            level: 3
                            text: `@${username}`
                            Layout.fillWidth: true
                        }
                        Text {
                            Layout.fillWidth: false
                            text: createdAt
                        }
                    }
                    Kirigami.Separator {
                        Layout.fillWidth: true
                    }
                    Text {
                        Layout.fillWidth: true
                        text: description
                        textFormat: Text.MarkdownText
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent

        Kirigami.Heading {
            level: 1
            text: pullRequest.title
        }
        Kirigami.Separator {
            Layout.fillWidth: true
        }

        Text {
            Layout.fillHeight: false
            text: pullRequest.description
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
            model: commentModel
            delegate: commentDelegate
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

    Connections {
        target: NetworkManager

        function onPullRequestCommentsFinished(jsonResponse) {
            commentModel.loadData(jsonResponse)
        }
        function onErrorOcurred(err) {
            console.log(err);
        }
    }

    Component.onCompleted: {
        NetworkManager.getPullRequestComments(root.pullRequest.number)
    }
}
