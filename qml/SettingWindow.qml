import QtQuick 2.7
import QtQuick.Controls 2.4

ApplicationWindow {
    id: window
    visible: true
    width: 560
    height: 420
    // title: qsTr("Wearable")

    //    background: Image {
    //        source: "images/background.png"
    //    }
    header: NaviButton {
        id: homeButton
        edge: Qt.TopEdge
        enabled: stackView.depth > 1
        imageSource: "qrc:/images/home.png"
        onClicked: stackView.pop(null)
    }
    footer: NaviButton {
        id: backButton

        edge: Qt.BottomEdge
        enabled: stackView.depth > 1
        imageSource: "qrc:/images/back.png"
        onClicked: stackView.pop()
    }

    StackView {
        id: stackView
        focus: true
        anchors.fill: parent
        initialItem: SettingView {
            onLaunched: stackView.push(page)
        }
    }
}
