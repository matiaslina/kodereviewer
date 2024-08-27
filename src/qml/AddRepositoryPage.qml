import QtCore
import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.FormCardPage {
    id: root
    title: "Add a new repository"

    signal accepted(string url, string name, string owner, string displayName)

    FormCard.FormHeader {
        title: "Github information"
    }

    FormCard.FormCard {
        FormCard.FormTextFieldDelegate {
            id: urlField
            label: "URL"
            onTextChanged: root.fillDataFromUrl(text)
        }

        FormCard.FormDelegateSeparator {}

        FormCard.FormTextFieldDelegate {
            id: nameField
            label: "Name"
        }

        FormCard.FormDelegateSeparator {}

        FormCard.FormTextFieldDelegate {
            id: ownerField
            label: "Owner"
        }

        FormCard.FormDelegateSeparator {}
    }

    FormCard.FormHeader {
        title: "General information"
    }

    FormCard.FormCard {
        FormCard.FormTextFieldDelegate {
            id: displayNameField
            label: "Display name"
        }
    }


    FormCard.FormHeader {
        title: "Git"
    }

    FormCard.FormCard {
        FormCard.FormCheckDelegate {
            id: cloneCheck
            text: "Clone repository"
            checked: false
        }

        FormCard.FormDelegateSeparator {}

        FormCard.FormTextFieldDelegate {
            id: cloneDirectory
            label: "Clone directory"
            enabled: cloneCheck.checked
        }
    }


    footer: QQC2.ToolBar {
        contentItem: QQC2.DialogButtonBox {
            standardButtons: QQC2.Dialog.Ok | QQC2.Dialog.Cancel
            onAccepted: root.accepted(
                urlField.text,
                nameField.text,
                ownerField.text,
                displayNameField.text
            )
            onRejected: applicationWindow().pageStack.pop()
        }
    }

    function fillDataFromUrl(text) {
        const s = text.split("/")
        if (s.length > 2) {
            nameField.text = s[s.length -1]
            ownerField.text = s[s.length - 2]
            displayNameField.text = s[s.length -1]
        }
    }
}
