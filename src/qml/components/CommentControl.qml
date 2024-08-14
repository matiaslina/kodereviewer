import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7
import org.kde.kirigami 2 as Kirigami
import org.kde.syntaxhighlighting 1

QQC2.Control {
    id: root

    onActiveFocusChanged: textField.forceActiveFocus()
    signal commentSent(string comment)

    Kirigami.Action {
        id: sendAction
        icon.name: "document-send"
        text: "Send message"
        displayHint: Kirigami.DisplayHint.IconOnly
        checkable: true

        onTriggered: {
            print("Sending text" + textField.text)
            root.commentSent(textField.text)
            textField.text = ""
        }
        tooltip: text
    }
    spacing: 0

    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Kirigami.Theme.inherit: false
    leftPadding: rightPadding
    rightPadding: Kirigami.Units.gridUnit
    topPadding: 0
    bottomPadding: 0

    contentItem: ColumnLayout {
        spacing: 0
        Item {
            // Required to adjust for the top separator
            Layout.preferredHeight: 1
            Layout.fillWidth: true
        }
        Kirigami.Separator {
            Layout.fillWidth: true
        }
        RowLayout {
            QQC2.ScrollView {
                id: chatBarScrollView

                Layout.fillWidth: true

                Layout.topMargin: Kirigami.Units.smallSpacing
                Layout.bottomMargin: Kirigami.Units.smallSpacing
                Layout.minimumHeight: Kirigami.Units.gridUnit * 2

                // HACK: This is to stop the ScrollBar flickering on and off as the height is increased
                QQC2.ScrollBar.vertical.policy: QQC2.ScrollBar.AsNeeded

                QQC2.TextArea {
                    id: textField

                    placeholderText: "Comment..."
                    verticalAlignment: TextEdit.AlignVCenter
                    wrapMode: TextEdit.Wrap

                    Accessible.description: placeholderText

                    Kirigami.SpellCheck.enabled: false

                    Keys.onReturnPressed: event => {
                        textField.insert(cursorPosition, "\n");
                    }
                    background: MouseArea {
                       acceptedButtons: Qt.NoButton
                      cursorShape: Qt.IBeamCursor
                       z: 1
                    }
                    SyntaxHighlighter {
                        id: hightlighter
                        textEdit: textField
                        repository: Repository
                        definition: Repository.definitionForFileName("la.md")
                    }

                }
            }
            QQC2.ToolButton {
                spacing: 0
                Layout.bottomMargin: Kirigami.Units.smallSpacing
                Layout.alignment: Qt.AlignBottom
                icon.name: sendAction.icon.name
                icon.height: 22
                icon.width: 22
                onClicked: {
                    sendAction.trigger()
                }

                QQC2.ToolTip.visible: hovered
                QQC2.ToolTip.text: sendAction.tooltip
                QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
            }
        }
    }
}
