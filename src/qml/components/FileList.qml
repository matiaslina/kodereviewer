import QtQuick 6.7
import QtQuick.Controls 6 as QQC2
import QtQuick.Layouts 6.7

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.delegates as Delegates

import org.kde.kodereviewer

ColumnLayout {
    id: root
    property FileModel model
    property alias filterModel: view.model
    signal fileSelected(File file)

    ListView {
        id: view
        Layout.fillWidth: true
        Layout.fillHeight: true

        delegate: Delegates.RoundedItemDelegate {
            id: delegate

            required property string filename
            required property int index
            required property string status

            highlighted: ListView.isCurrentItem
            text: filename
            contentItem: FileDelegate {
                itemDelegate: delegate
            }
            onClicked: {
                view.currentIndex = index
                const file = root.model.get(index)
                root.fileSelected(file)
            }
        }
    }

    Item {
        visible: view.count === 0
        Layout.fillHeight: true
        Layout.fillWidth: true
        ColumnLayout {
            QQC2.Label {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                text: "Loading files..."
            }
            QQC2.BusyIndicator {
                Layout.alignment: Qt.AlignHCenter| Qt.AlignVCenter
            }
        }
    }
}
