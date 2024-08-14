import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.delegates as Delegates

ColumnLayout {
    id: root
    property alias model: view.model
    signal fileSelected(string file)

    QQC2.ScrollView {
        spacing: 0
        Layout.fillWidth: true
        Layout.fillHeight: true

        QQC2.ScrollBar.horizontal.policy: QQC2.ScrollBar.AlwaysOff


        clip: true

        ListView {
            id: view
            Layout.fillWidth: true
            Layout.fillHeight: true

            model: root.model
            delegate: Delegates.RoundedItemDelegate {
                id: delegate
                highlighted: ListView.isCurrentItem
                text: modelData
                onClicked: {
                    view.currentIndex = index
                    root.fileSelected(modelData)
                }
            }
        }

    }
}
