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

    required property PullRequestModel model
    required property NetworkManager connection
    required property Project currentProject
    signal pullRequestSelected(PullRequest pullRequest)
    signal fileSelected(File file)

    // Style settings
    Kirigami.Theme.colorSet: Kirigami.Theme.Window
    Kirigami.Theme.inherit: false
    property color backgroundColor: Kirigami.Theme.backgroundColor
    readonly property int narrowWidth: Kirigami.Units.gridUnit * 3
    readonly property int largeWidth: Kirigami.Units.gridUnit * 15

    required property Kirigami.ApplicationWindow window

    modal: !enabled || Kirigami.Settings.isMobile || (window.width < Kirigami.Units.gridUnit * 50 && !collapsed)
    closePolicy: modal ? Controls.Popup.CloseOnReleaseOutside : Controls.Popup.NoAutoClose
    drawerOpen: !Kirigami.Settings.isMobile && enabled
    onModalChanged: drawerOpen = !modal
    handleClosedIcon.source: modal ? null : "sidebar-expand-left"
    handleOpenIcon.source: modal ? null : "sidebar-collapse-left"

    handleVisible: enabled

    enabled: currentProject.url != ""
    width: Kirigami.Units.gridUnit * 15

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0


    property FileModel fileListModel: FileModel {}

    KItemModels.KSortFilterProxyModel {
        id: filterModel
        sourceModel: drawer.model
        filterRoleName: "title"
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
            readonly property bool isWide: !drawer.collapsed
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

            visible: !drawer.collapsed

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
                                drawer.switchToFileList();
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
                            drawer.fileSelected(file)
                        }
                    }
                }
            }
        }
    }

    function switchToPullRequestList() {
        stackView.pop()
    }

    function switchToFileList() {
        stackView.push(fileListViewComponent)
    }

    Connections {
        target: drawer.connection
        function onPullRequestFilesFinished(jsonResponse) {
            fileListModel.loadData(jsonResponse)
        }
    }
}
