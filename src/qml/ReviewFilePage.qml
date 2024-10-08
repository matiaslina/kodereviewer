import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami
import org.kde.kodereviewer

Kirigami.ScrollablePage {
    id: root

    property PullRequest pullRequest: applicationWindow().currentPullRequest
    property NetworkManager connection: applicationWindow().connection
    property GitBackend git: applicationWindow().gitBackend
    property File file: applicationWindow().currentReviewFile
    padding: Kirigami.Units.smallSpacing

    title: `${pullRequest.title} - ${file.filename}`

    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Kirigami.Theme.inherit: false

    actions: [
        Kirigami.Action {
            icon.name: "go-home"
            text: "Pull request overview"
            onTriggered: {
                applicationWindow().switchToPullRequestOverview()
            }
        },
        Kirigami.Action {
            icon.name: "edit-comment"
            text: "Add comment"
            onTriggered: {
                print("Adding comment")
                applicationWindow().openCommentSideBar(root.file.filename, 0)
            }
        }
    ]

    Connections {
        target: root.git
        function onSourceRefChanged() {
            print("S")
            sourceEditor.text = root.git.sourceFileContents()
        }

        function onTargetRefChanged() {
            print("T")
            targetEditor.text = root.git.targetFileContents()
        }
    }

    Editor {
        id: diffEditor
        connection: root.connection

        anchors.fill: parent
        text: root.file.patch
        filename: "a.patch"
        file: root.file.filename
        pullRequest: root.pullRequest

        onCommentClicked: (path, line) => {
            applicationWindow().openCommentSideBar(path, line)
        }
    }
}
