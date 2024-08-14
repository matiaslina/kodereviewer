import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Effects
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami
import org.kde.kodereviewer



Kirigami.Page {
    id:root
    required property PullRequest pullRequest
    required property NetworkManager connection
    required property GitBackend git

    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Kirigami.Theme.inherit: false

    title: `${pullRequest.number} - ${pullRequest.title}`

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
                        Image {
                            source: userImage
                            sourceSize.width: 22
                            sourceSize.height: 22
                        }
                        Kirigami.Heading {
                            id: commentHeading
                            level: 3
                            text: `@${username}`
                        }
                        Text {
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
        anchors.fill: parent

        QQC2.ScrollView {
            spacing: 0
            Layout.fillWidth: true
            Layout.fillHeight: true
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
                    text: "Files changed"
                }

                Column {
                    Repeater {
                        model: root.git.filesChanged()
                        delegate: Text {
                            text: modelData
                        }
                    }
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
        }
    }

    footer: CommentControl {

    }

    Connections {
        target: root.connection

        function onPullRequestCommentsFinished(jsonResponse) {
            commentModel.loadData(jsonResponse)
        }
        function onErrorOcurred(err) {
            console.log(err);
        }
    }

    Component.onCompleted: {
        root.connection.getPullRequestComments(root.pullRequest.number)
    }
}
