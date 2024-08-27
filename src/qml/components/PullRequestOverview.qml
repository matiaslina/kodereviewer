import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.components as KirigamiComponents

import org.kde.kodereviewer

ListView {
    id: root

    required property PullRequest pullRequest
    spacing: Kirigami.Units.largeSpacing * 2
    topMargin:  Kirigami.Units.largeSpacing * 2
    rightMargin: Kirigami.Units.largeSpacing * 2
    leftMargin: Kirigami.Units.largeSpacing * 2
    bottomMargin: Kirigami.Units.largeSpacing * 2 + commentToolbar.height

    property bool loadingComments: true


    header: ColumnLayout {
        id: mainLayout
        width: ListView.view ? ListView.view.width - ListView.view.leftMargin - ListView.view.rightMargin : 0

        Kirigami.Heading {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            level: 1
            text: root.pullRequest ? root.pullRequest.title : ""
        }

        Kirigami.ListSectionHeader {
            Layout.fillWidth: true
            text: "description"
        }

        QQC2.Label {
            Layout.fillWidth: true
            Layout.fillHeight: false
            text: root.pullRequest ? root.pullRequest.description : ""
            textFormat: Text.MarkdownText
        }

        Kirigami.ListSectionHeader {
            Layout.fillWidth: true
            text: "Comments"
        }
    }

    delegate: CommentDelegate {}

    Kirigami.PlaceholderMessage {
        anchors.centerIn: parent
        width: parent.width - (Kirigami.Units.largeSpacing * 4)
        visible: root.count === 0 && !root.loadingComments
        text: "Nothing here"
    }

    Kirigami.PlaceholderMessage {
        anchors.centerIn: parent
        width: parent.width - (Kirigami.Units.largeSpacing * 4)
        visible: root.loadingComments
        text: "Loading comments..."
    }
}
