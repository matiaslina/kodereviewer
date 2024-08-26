import QtQuick 6.7
import QtQuick.Controls 6
import QtQuick.Layouts 6.7
import org.kde.kirigami 2 as Kirigami
import org.kde.kitemmodels 1 as KItemModels
import org.kde.syntaxhighlighting 1
import org.kde.kirigamiaddons.delegates 1

Kirigami.ApplicationWindow {
    id: root
    width: 600
    height: 400
    title: "Hello"
    required property var model

    Component {
        id: commentDelegate
        Kirigami.AbstractCard {
            contentItem: Item {
                // implicitWidth/Height define the natural width/height
                // of an item if no width or height is specified.
                // The setting below defines a component's preferred size based on its content
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight
                GridLayout {
                    id: delegateLayout
                    anchors {
                        left: parent.left
                        top: parent.top
                        right: parent.right
                    }
                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.largeSpacing
                    columns: root.wideScreen ? 4 : 2

                    ColumnLayout {
                        Kirigami.Heading {
                            Layout.fillWidth: true
                            level: 2
                            text: `@${modelData.username}`
                        }
                        Kirigami.Separator {
                            Layout.fillWidth: true
                            visible: modelData.description.length > 0
                        }
                        Text {
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            text: modelData.description
                            visible: modelData.description.length > 0
                            textFormat: Text.MarkdownText
                        }
                    }
                }
            }
        }
    }

    pageStack.initialPage: Kirigami.ScrollablePage {
        id: page
        title: "Comments"
        Kirigami.CardsListView {
            id: cardsView
            // Model contains info to be displayed
            model: root.model
            // Delegate is how the information will be presented in the ListView
            delegate: commentDelegate
        }
        footer: ColumnLayout {
            Kirigami.Separator {
                weight: Kirigami.Separator.Weight.Normal
                Layout.fillWidth: true
            }
            CommentControl {
                Layout.fillWidth: true
            }
        }
    }
}
