import QtQuick 6.7
import QtQuick.Controls 6
import QtQuick.Layouts 6.7

import org.kde.kirigami 2 as Kirigami
import org.kde.kitemmodels 1 as KItemModels
import org.kde.syntaxhighlighting 1
import org.kde.kirigamiaddons.delegates 1

import org.kde.kodereviewer

Kirigami.ScrollablePage {
    id: root

    property int commentId
    required property int pullRequestNumber
    property alias model: cardsView.model
    required property NetworkManager connection

    actions: [
        Kirigami.Action {
            icon.name: "view-close"
            text: "Close"
            onTriggered: {
                applicationWindow().pageStack.pop()
            }
        }
    ]

    Kirigami.CardsListView {
        id: cardsView
        // Model contains info to be displayed
        // Delegate is how the information will be presented in the ListView
        delegate: CommentDelegate {}
    }

    footer: ColumnLayout {
        Kirigami.Separator {
            weight: Kirigami.Separator.Weight.Normal
            Layout.fillWidth: true
        }
        CommentControl {
            Layout.fillWidth: true
            onCommentSent: (comment) => {
                root.connection.sendThreadComment(root.pullRequestNumber, root.commentId, comment)
            }
        }
    }

    Connections {
        target: root.connection
        function onSendThreadCommentFinished(jsonResponse) {

        }
    }

    onModelChanged: commentId = root.model.getThreadId()
}
