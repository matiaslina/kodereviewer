pragma ComponentBehavior: Bound
import QtQuick 6.7
import QtQuick.Controls 6.7 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami

import org.kde.syntaxhighlighting

import org.kde.kodereviewer

TextEdit {

    id: root

    required property PullRequest pullRequest
    required property string file
    required property NetworkManager connection

    signal commentClicked(string path, int line)

    property string filename: "A.log"

    leftPadding: lineNumberColumn.width + lineNumberColumn.anchors.leftMargin + Kirigami.Units.smallSpacing * 2

    readOnly: true
    textFormat: TextEdit.PlainText
    wrapMode: TextEdit.Wrap
    color: Kirigami.Theme.textColor
    selectionColor: Kirigami.Theme.highlightColor
    font.family: "monospace"
    Kirigami.SpellCheck.enabled: false
    property int lineHeight: contentHeight / lineCount

    onWidthChanged: lineModel.resetModel()
    onHeightChanged: lineModel.resetModel()

    SyntaxHighlighter {
        id: highlighter
        textEdit: root
        definition: Repository.definitionForFileName(root.filename)
    }

    Kirigami.Separator {
        anchors {
            top: root.top
            bottom: root.bottom
            left: lineNumberColumn.right
            leftMargin: Kirigami.Units.smallSpacing
        }
    }

    ColumnLayout {
        id: lineNumberColumn
        anchors {
            top: root.top
            topMargin: root.topPadding
            left: root.left
            leftMargin: Kirigami.Units.smallSpacing
        }
        spacing: 0
        Repeater {
            id: repeater
            model: LineModel {
                id: lineModel
                document: root.textDocument
            }
            delegate: QQC2.Label {
                id: label
                required property int index
                required property int docLineHeight
                Layout.fillWidth: true
                Layout.preferredHeight: docLineHeight
                horizontalAlignment: Text.AlignRight
                text: index + 1
                color: Kirigami.Theme.disabledTextColor

                font.family: "monospace"
            }
        }
    }

    Repeater {
        id: threadsRepeater
        model: root.pullRequest.reviewThreadLines(root.file)

        QQC2.Button {
            required property int modelData
            icon.name: "edit-comment"
            y: root.lineHeight * modelData
            anchors.right: root.right

            onClicked: {
                root.commentClicked(root.file, modelData)
            }
        }
    }

    Repeater {
        id: hightlightRepeater
        model: root.pullRequest.reviewThreadLines(root.file)
        delegate: Rectangle {
            required property int modelData
            radius: 1
            height: root.lineHeight * modelData + 1 // (modelData.endLine - modelData.startLine + 1)
            y: modelData * root.lineHeight
            color: "peachpuff"
            z: -1
            anchors {
                left: lineNumberColumn.right
                leftMargin: Kirigami.Units.smallSpacing
                right: root.right
                rightMargin: Kirigami.Units.smallSpacing
            }
        }
    }

    onFileChanged: {
        repeater.model.resetModel()
        threadsRepeater.model = root.pullRequest.reviewThreadLines(root.file)
        hightlightRepeater.model = root.pullRequest.reviewThreadLines(root.file)
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: mouse => {
            if (mouse.button === Qt.RightButton)
                contextMenu.popup()
        }
        QQC2.Menu {
            id: contextMenu
            QQC2.MenuItem { text: "Add comment" }
        }
    }
}
