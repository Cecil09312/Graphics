import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Contral_1_4

ApplicationWindow {
    id: window
    visible: true
    width: 840
    height: 640

    header: Item {
        height: 40
        NaviButton {
            id: homeButton
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            edge: Qt.TopEdge
            enabled: stackView.depth > 1
            imageSource: "qrc:/images/home.png"
            onClicked: {
                //settingView.currentIndex = 0
                // stackView.pop()
                stackView.pop()
            }
        }
    }

    footer: Item {
        height: 40
        NaviButton {
            id: backButton
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            edge: Qt.BottomEdge
            enabled: stackView.depth > 1
            imageSource: "qrc:/images/back.png"
            onClicked: {
                //settingView.currentIndex = 0
                // stackView.pop()
                stackView.pop()
            }
        }
    }
    Contral_1_4.StackView {
        id: stackView
        focus: true
        anchors.fill: parent
        delegate: Contral_1_4.StackViewDelegate {
            function transitionFinished(properties) {
                properties.exitItem.opacity = 1
            }

            pushTransition: Contral_1_4.StackViewTransition {
                PropertyAnimation {
                    target: enterItem
                    property: "opacity"
                    from: 0
                    to: 1
                }
                PropertyAnimation {
                    target: exitItem
                    property: "opacity"
                    from: 1
                    to: 0
                }
            }
        }

        initialItem: SettingView {
            id: settingView
            onLaunched: stackView.push(page)
        }
    }
}
