import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami
import org.kde.kodereviewer

Kirigami.ScrollablePage {
    id: root
    required property NetworkManager connection
    property PullRequest pullRequest
    property GitBackend git
    property File file
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
            /* const component = Qt.createComponent("CommentWindow.qml") */
            /* if (component.status === Component.Error) { */
            /*     console.error(component.errorString()) */
            /*     return */
            /* } */
            /* const reviewThreadModel = root.pullRequest.reviewThreadModel(path); */
            /* print(reviewThreadModel) */
            /* const instance = component.createObject(root, { */
            /*     commentId: "", */
            /*     pullRequestNumber: root.pullRequest.number, */
            /*     model: reviewThreadModel, */
            /*     connection: root.connection, */
            /*     title: `${path}:${line}` */
            /* }) */
            /* applicationWindow().pageStack.push(instance) */
        }
    }
}
