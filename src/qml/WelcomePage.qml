import QtCore
import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

import org.kde.kodereviewer

FormCard.FormCardPage {
    id: root

    signal projectSelected(Project project)

    ProjectModel {
        id: projectModel
    }

    title: "Welcome"

    Component {
        id: addRepositoryPage
        AddRepositoryPage {
            onAccepted: (url, name, owner, displayName) => {
                applicationWindow().pageStack.pop()
                projectModel.append(displayName, url, owner)
            }
        }
    }

    Kirigami.Heading {
        id: welcomeMessage

        text: "Welcome to Kode Reviewer"

        Layout.alignment: Qt.AlignHCenter
        Layout.topMargin: Kirigami.Units.largeSpacing
    }

    FormCard.FormHeader {
        id: existingProjectsHeader
        title: "Existing projects"
        visible: projectModel.rowCount() > 0
    }


    FormCard.FormCard {
        visible: existingProjectsHeader.visible

        Repeater {
            id: loadedProjects
            model: projectModel
            delegate: FormCard.FormButtonDelegate {
                required property string name
                required property string url
                required property int index
                text: name
                description: url
                onClicked: root.projectSelected(projectModel.get(index))
            }
        }
    }

    FormCard.FormHeader {
        title: "Add new project"
    }

    FormCard.FormCard {
        FormCard.FormButtonDelegate {
            text: "Add new project"
            onClicked: applicationWindow().pageStack.push(addRepositoryPage)
        }
    }

}
