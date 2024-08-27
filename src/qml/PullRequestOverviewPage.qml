import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6

import org.kde.kirigami as Kirigami
import org.kde.kodereviewer

Kirigami.ScrollablePage {
    id:root
    required property PullRequest pullRequest
    required property NetworkManager connection
    required property GitBackend git
    property int lastCommentsRequested: -1

    property alias loadingComments: overview.loadingComments

    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Kirigami.Theme.inherit: false

    title: pullRequest ? pullRequest.title : "Select a pull request"

    CommentModel {
        id: commentModel
    }

    PullRequestOverview {
        id: overview
        visible: pullRequest != undefined
        pullRequest: root.pullRequest
        model: commentModel
    }

    QQC2.Label {
        anchors.fill: parent
        visible: root.pullRequest == undefined
        text: "Select a pull request"
    }

    footer: CommentControl {
        onCommentSent: (comment) => {
            root.connection.sendComment(root.pullRequest.number, comment)
        }
    }


    Connections {
        target: root.connection

        function onPullRequestCommentsFinished(jsonResponse) {
            commentModel.loadData(jsonResponse)
            root.loadingComments = false
        }

        function onPullRequestPostCommentFinished(jsonResponse) {
            commentModel.addComment(jsonResponse)
        }

        function onPullRequestThreadsFinished(jsonResponse) {
            root.pullRequest.loadThreads(jsonResponse)
        }

        function onErrorOcurred(err) {
            console.log(err);
        }
    }

    Connections {
        target: root.pullRequest
        function onNumberChanged() {
            lastCommentsRequested = pullRequest.number
            root.connection.getPullRequestThreads(root.pullRequest.number)

            root.connection.getPullRequestComments(root.pullRequest.number)
            root.loadingComments = true
        }
    }
}
