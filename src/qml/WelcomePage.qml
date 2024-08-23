import QtCore
import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami

import org.kde.kodereviewer

Kirigami.ScrollablePage {
    id: root

    signal projectSelected(Project project)

    ProjectModel {
        id: projectModel
    }

    title: "Welcome"

    actions: [
        Kirigami.Action {
            id: addRepositoryAction
            icon.name: "list-add"
            text: "Add repository"
            onTriggered: {
                addRepository.open()
            }
        }
    ]

    Kirigami.CardsListView {
        id: view
        model: projectModel
        delegate: Kirigami.AbstractCard {
            //NOTE: never put a Layout as contentItem as it will cause binding loops
            //SEE: https://bugreports.qt.io/browse/QTBUG-66826
            contentItem: Item {
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight
                GridLayout {
                    id: delegateLayout
                    anchors {
                        left: parent.left
                        top: parent.top
                        right: parent.right
                        //IMPORTANT: never put the bottom margin
                    }
                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.largeSpacing
                    columns: width > Kirigami.Units.gridUnit * 20 ? 4 : 2
                    Kirigami.Icon {
                        source: "git"
                        Layout.fillHeight: true
                        Layout.maximumHeight: Kirigami.Units.iconSizes.huge
                        Layout.preferredWidth: height
                    }
                    ColumnLayout {
                        Kirigami.Heading {
                            level: 2
                            text: name
                        }
                        Kirigami.Separator {
                            Layout.fillWidth: true
                        }
                        QQC2.Label {
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            text: url
                        }
                    }
                    QQC2.Button {
                        Layout.alignment: Qt.AlignRight|Qt.AlignVCenter
                        Layout.columnSpan: 2
                        text: qsTr("Open")
                        onClicked: {
                            root.projectSelected(projectModel.get(index))
                        } // showPassiveNotification("Install for Product " + name + " clicked");
                    }
                }
            }
        }

        QQC2.Label {
            visible: view.count == 0
            anchors.centerIn: parent
            text: "No repositories!"
        }
    }


    Kirigami.Dialog {
        id: addRepository
        title: "Add repository"
        standardButtons: Kirigami.Dialog.Ok | Kirigami.Dialog.Cancel
        padding: Kirigami.Units.largeSpacing
        preferredWidth: Kirigami.Units.gridUnit * 20

        Kirigami.FormLayout {
            QQC2.TextField {
                id: urlField
                Kirigami.FormData.label: "Url"
                onAccepted: nameField.forceActiveFocus()
                onTextChanged: {
                    if (text.length > 0) {
                        const s = text.split("/")
                        nameField.text = s[s.length -1]
                    }
                }
            }
            QQC2.TextField {
                id: nameField
                Kirigami.FormData.label: "Name"
            }
        }

        onAccepted: {
            const url = urlField.text
            const name = nameField.text

            const splittedUrl = url.split("/")
            const owner = splittedUrl[splittedUrl.length - 2]
            const repoName = splittedUrl[splittedUrl.length - 1]

            print(`url: ${url}`)
            print(`name: ${name}`)
            print(`owner: ${owner}`)
            print(`repoName: ${repoName}`)
            projectModel.append(name, url, owner)
        }
    }
}
