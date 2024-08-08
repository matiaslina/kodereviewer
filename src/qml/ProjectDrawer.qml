import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami
import org.kde.kodereviewer.models
import org.kde.kodereviewer.network
import org.kde.kodereviewer.types


Kirigami.GlobalDrawer {
    id: drawer
    property string currentSearch
    property string repositoryRoot
    signal pullRequestSelected(PullRequest pullRequest)

    //collapsedSize: applicationMenuButton.implicitWidth
    modal: false
    collapsible: true
    collapsed: true

    //width: Kirigami.Units.gridUnit * 14
    //implicitWidth: Kirigami.Units.gridUnit * 14
    showHeaderWhenCollapsed: true
    collapseButtonVisible: false


    header: Kirigami.AbstractApplicationHeader {
        contentItem: RowLayout {
            anchors.fill: parent
            QQC2.ToolButton {
                id: applicationMenuButton
                Layout.fillWidth: false
                Layout.leftMargin: Kirigami.Units.smallSpacing
                icon.name: "application-menu"
                onClicked: drawer.collapsed = !drawer.collapsed
            }
            Kirigami.SearchField {
                id: searchField
                Layout.fillWidth: true
                Layout.rightMargin: Kirigami.Units.smallSpacing
                visible: !drawer.collapsed

                onTextChanged: {
                    drawer.currentSearch = text
                }
            }
        }
    }

    PullRequestModel {
        id: pullRequestModel
        onModelAboutToBeReset: {
            /* drawer.pullRequests.forEach((elem) => { */
            /*     elem.destroy() */
            /* }); */
            drawer.pullRequests = []
        }
        onModelReset: {
            const data = pullRequestModel.getPullRequests()
            data.forEach((pr) => {
                var action = prAction.createObject(pullRequestAction, {
                    pr: pr
                })
                pullRequests.push(action)
            })
        }
    }

    property list<Kirigami.Action> pullRequests: [
    ]

    actions: [
        Kirigami.Action {
            text: "Home"
            icon.name: "go-home"
        },
        Kirigami.Action {
            id: pullRequestAction
            text: "Pull requests"
            icon.name: "vcs-merge-request"
            expandible: true
            children: drawer.pullRequests
        }
    ]

    Component {
        id: prAction
        Kirigami.Action {
            property PullRequest pr
            text: `#${pr.number} - ${pr.title}`
            icon.name: "vcs-merge-request"
            icon.color: "green"
            onTriggered: {
                drawer.pullRequestSelected(pr)
            }
        }
    }

    Connections {
        target: NetworkManager

        function onPullRequestFinished(jsonResponse) {
            pullRequestModel.loadData(jsonResponse)
        }
        function onErrorOcurred(err) {
            console.log(err);
        }
    }


    function loadPullRequests() {
        NetworkManager.getPullRequests()
    }
}
