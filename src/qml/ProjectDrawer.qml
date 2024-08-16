pragma ComponentBehavior: Bound
import QtQuick 2.15 // Removing version break onCurrentItemChanged
import QtQuick.Layouts
import QtQuick.Controls as Controls

import org.kde.kirigamiaddons.delegates as Delegates
import org.kde.kitemmodels 1 as KItemModels
import org.kde.kirigami as Kirigami

import org.kde.kodereviewer

Kirigami.OverlayDrawer {
    id: drawer

    Kirigami.Theme.colorSet: Kirigami.Theme.View
    property color backgroundColor: Kirigami.Theme.backgroundColor
    readonly property int narrowWidth: Kirigami.Units.gridUnit * 3
    readonly property int largeWidth: Kirigami.Units.gridUnit * 15

    required property PullRequestModel model
    required property NetworkManager connection
    signal pullRequestSelected(PullRequest pullRequest)
    signal fileSelected(File file)

    property bool isWide: true
    property bool changeWidth: true
    width: isWide ? largeWidth : narrowWidth

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    modal: Kirigami.Settings.isMobile

    closePolicy: modal ? Controls.PopupCloseOnReleaseOutside : Controls.Popup.NoAutoClose



    property FileModel fileListModel: FileModel {}

    KItemModels.KSortFilterProxyModel {
        id: filterModel
        sourceModel: drawer.model
        filterRoleName: "title"
        sortRoleName: "number"
        sortOrder: Qt.AscendingOrder
    }

    KItemModels.KSortFilterProxyModel {
        id: fileFilterModel
        sourceModel: drawer.fileListModel
        filterRoleName: "filename"
        sortRoleName: "filename"
        sortOrder: Qt.AscendingOrder
    }

    contentItem: Rectangle {
        id: column

        Kirigami.Theme.backgroundColor: drawer.backgroundColor
        color: Kirigami.Theme.backgroundColor

        Controls.ToolBar {
            id: mainToolBar
            height: pageStack.globalToolBar.preferredHeight
            readonly property bool isWide: drawer.isWide
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right


            DrawerHeader {
                drawer: drawer
                toolbar: mainToolBar
                onSearchChanged: (text) => {
                    filterModel.filterString = text
                    fileFilterModel.filterString = text
                }
            }
        }

        Controls.ScrollView {
            id: listView
            Kirigami.Theme.inherit: true

            visible: drawer.isWide

            anchors.top: mainToolBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Flickable {
                id: mainFlickable
                anchors.fill: parent
                Controls.StackView {
                    id: stackView
                    anchors.fill: parent
                    initialItem: pullRequestListView
                }

                Component {
                    id: pullRequestListView
                    ListView {
                        id: view
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: filterModel
                        clip: true
                        delegate: Delegates.RoundedItemDelegate {
                            required property int number
                            required property string title
                            required property int index
                            highlighted: ListView.isCurrentItem

                            text: `${number} - ${title}`
                            icon.name: "vcs-merge-request"
                            onClicked: {
                                view.currentIndex = index
                                const pr = drawer.model.get(index)
                                root.connection.getPullRequestFiles(pr.number)
                                drawer.pullRequestSelected(pr)
                                stackView.push(fileListViewComponent)
                            }
                        }
                    }
                }

                Component {
                    id: fileListViewComponent
                    FileList {
                        model: fileListModel
                        filterModel: fileFilterModel

                        onFileSelected: (file) => {
                            print("Selected File! " + file.filename)
                            drawer.fileSelected(file)
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: drawer.connection
        function onPullRequestFilesFinished(jsonResponse) {
            fileListModel.loadData(jsonResponse)
        }
    }
}
