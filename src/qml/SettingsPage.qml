import QtCore
import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

import org.kde.kodereviewer

FormCard.FormCardPage {
    id: root

    actions: [
        Kirigami.Action {
            id: saveAction
            text: "Save"
            onTriggered: {
                settings.sync()
            }
        }
    ]

    Settings {
        id: settings
        property string githubToken: githubTokenField.text
    }

    FormCard.FormHeader {
        title: "Authorization"
    }

    FormCard.FormCard {
        FormCard.FormTextFieldDelegate {
            id: githubTokenField
            text: settings.githubToken
            label: "Github Token"
        }
    }
}
