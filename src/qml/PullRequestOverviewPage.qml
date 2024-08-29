import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6

import org.kde.kirigami as Kirigami
import org.kde.kodereviewer

Kirigami.ScrollablePage {
    id:root
    property PullRequest pullRequest: applicationWindow().currentPullRequest
    property NetworkManager connection: applicationWindow().connection
    property GitBackend git: applicationWindow().gitBackend
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
        pullRequest: root.pullRequest
        model: commentModel
    }

    footer: CommentControl {
        visible: pullRequest
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

    onPullRequestChanged: {
        if  (pullRequest) {
            lastCommentsRequested = pullRequest.number
            root.connection.getPullRequestThreads(root.pullRequest.number)
            root.connection.getPullRequestComments(root.pullRequest.number)
            root.loadingComments = true
        }
    }

}
