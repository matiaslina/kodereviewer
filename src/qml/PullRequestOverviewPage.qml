import QtQuick 6.7
import QtQuick.Controls 6 as QQC2

import org.kde.kirigami as Kirigami
import org.kde.kodereviewer

Kirigami.Page {
    id:root
    required property PullRequest pullRequest
    required property NetworkManager connection
    required property GitBackend git

    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Kirigami.Theme.inherit: false

    title: pullRequest.title

    CommentModel {
        id: commentModel
    }

    contentItem: PullRequestOverview {
        QQC2.SplitView.fillHeight: true
        QQC2.SplitView.fillWidth: true
        pullRequest: root.pullRequest
        model: commentModel
    }

    Connections {
        target: root.connection

        function onPullRequestCommentsFinished(jsonResponse) {
            commentModel.loadData(jsonResponse)
        }

        function onErrorOcurred(err) {
            console.log(err);
        }
    }

    Component.onCompleted: {
        root.connection.getPullRequestComments(root.pullRequest.number)
    }
}
