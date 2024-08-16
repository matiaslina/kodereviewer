import QtQuick 6.7
import QtQuick.Controls 6.7 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami

import org.kde.syntaxhighlighting

import org.kde.kodereviewer

QQC2.Control {
    id: root
    property string filename: "A.log"
    property string text

    contentItem: QQC2.ScrollView {
        id: codeScrollView

        QQC2.TextArea {
            id: textEdit
            topPadding: Kirigami.Units.smallSpacing
            bottomPadding: Kirigami.Units.smallSpacing
            leftPadding: lineNumberColumn.width + lineNumberColumn.anchors.leftMargin + Kirigami.Units.smallSpacing * 2

            readOnly: true
            text: root.text
            textFormat: TextEdit.PlainText
            wrapMode: TextEdit.Wrap
            color: Kirigami.Theme.textColor
            font.family: "monospace"
            Kirigami.SpellCheck.enabled: false

            onWidthChanged: lineModel.resetModel()
            onHeightChanged: lineModel.resetModel()

            SyntaxHighlighter {
                id: highlighter
                textEdit: textEdit
                definition: Repository.definitionForFileName(root.filename)
            }


            Rectangle {
                //width: textEdit.width - Kirigami.Units.gridUnit
                radius: 1
                height: 300
                color: "#aceebb"
                z: -1
                anchors {
                    top: textEdit.top
                    topMargin: Kirigami.Units.smallSpacing
                    left: lineNumberColumn.right
                    leftMargin: Kirigami.Units.smallSpacing
                    right: textEdit.right
                    rightMargin: Kirigami.Units.smallSpacing
                }
            }

            ColumnLayout {
                id: lineNumberColumn
                anchors {
                    top: textEdit.top
                    topMargin: textEdit.topPadding
                    left: textEdit.left
                    leftMargin: Kirigami.Units.smallSpacing
                }
                spacing: 0
                Repeater {
                    id: repeater
                    model: LineModel {
                        id: lineModel
                        document: textEdit.textDocument
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
    }
}
