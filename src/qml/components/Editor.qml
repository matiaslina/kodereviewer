import QtQuick 6.7
import QtQuick.Controls 6.7 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami

import org.kde.syntaxhighlighting

import org.kde.kodereviewer

TextEdit {

    id: root

    property string filename: "A.log"
    property DiffModel diffModel
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

    Repeater {
        visible: diffModel
        model: diffModel
        delegate: Rectangle {
            //width: textEdit.width - Kirigami.Units.gridUnit
            radius: 1
            height: root.lineHeight * newLines
            y: (newStart) * root.lineHeight
            color: "#aceebb"
            z: -1
            anchors {
                top: root.top
                topMargin: Kirigami.Units.smallSpacing
                left: lineNumberColumn.right
                leftMargin: Kirigami.Units.smallSpacing
                right: root.right
                rightMargin: Kirigami.Units.smallSpacing
            }
            onYChanged: { print(newStart); print(y) }
        }
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
}
