import QtQuick
import QtQuick.Effects

Item {
    id: root
    required property url source

    Image {
        id: sourceItem
        source: root.source
        visible: false
    }

    MultiEffect {
        source: sourceItem
        anchors.fill: sourceItem
        maskEnabled: true
        maskSource: mask
    }

    Item {
        id: mask
        width: sourceItem.width
        height: sourceItem.height
        layer.enabled: true
        visible: false

        Rectangle {
            width: sourceItem.width
            height: sourceItem.height
            radius: width/2
            color: "black"
        }
    }
}
