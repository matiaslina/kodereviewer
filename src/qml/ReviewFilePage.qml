import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami
import org.kde.kodereviewer

Kirigami.Page {
    id: root
    property PullRequest pullRequest
    property GitBackend git
    property string file

    actions: [
        Kirigami.Action {
            icon.name: "go-home"
            onTriggered: {
                applicationWindow().switchToPullRequestOverview()
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


    header: QQC2.TabBar {
        id: headerBar
        currentIndex: swipeView.currentIndex
        QQC2.TabButton {
            text: "Source branch"
        }
        QQC2.TabButton {
            text: "Target branch"
        }
        QQC2.TabButton {
            text: "Diff"
        }
    }

    QQC2.SwipeView {
        id: swipeView
        currentIndex: headerBar.currentIndex
        clip: true
        anchors.fill: parent
        Editor {
            id: sourceEditor
            text: root.git.sourceFileContents(root.file)
        }

        Editor {
            id: targetEditor
            text: root.git.targetFileContents(root.file)
        }

        Editor {
            text: `Diff ${root.file}`
        }
    }
}
