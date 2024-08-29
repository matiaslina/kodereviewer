import QtQuick 6.7
import QtQuick.Controls 6.7 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami 2 as Kirigami

import org.kde.syntaxhighlighting 1

QQC2.Control {
    id: root
    padding: Kirigami.Units.largeSpacing
    property alias text: textEdit.text
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            scrollView.implicitWidth)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             scrollView.implicitHeight)
    Layout.fillWidth: true
    width: ListView.view ? ListView.view.width - ListView.view.leftMargin - ListView.view.rightMargin : undefined

    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Kirigami.Theme.inherit: false

    contentItem: QQC2.ScrollView {
        id: scrollView
        anchors.fill: parent
        padding: Kirigami.Units.smallSpacing

        // (https://bugreports.qt.io/browse/QTBUG-83890)
        QQC2.ScrollBar.horizontal.policy: QQC2.ScrollBar.AlwaysOff

        QQC2.TextArea {
            id: textEdit
            padding: Kirigami.Units.smallSpacing

            readOnly: true
            textFormat: TextEdit.PlainText
            wrapMode: TextEdit.Wrap
            color: Kirigami.Theme.textColor
            selectionColor: Kirigami.Theme.highlightColor
            font.family: "monospace"

            SyntaxHighlighter {
                id: highlighter
                textEdit: textEdit
                definition: Repository.definitionForFileName("a.diff")
            }
        }
    }
    onImplicitWidthChanged: print(implicitBackgroundWidth + leftInset + rightInset,
                                  textEdit.implicitWidth, implicitWidth)
}
