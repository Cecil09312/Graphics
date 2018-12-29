import QtQuick 2.7
import QtQuick.Controls 2.2

Button {
    id: button

    property int edge: Qt.TopEdge
    property alias imageSource: image.source

    contentItem: Image {
        id: image
        fillMode: Image.Pad
        sourceSize {
            width: 40
            height: 40
        } // ### TODO: resize the image
    }

    background: Rectangle {
        height: button.height * 4
        width: height
        radius: width / 2
        anchors.horizontalCenter: button.horizontalCenter
        anchors.top: edge === Qt.BottomEdge ? button.top : undefined
        anchors.bottom: edge === Qt.TopEdge ? button.bottom : undefined
        color: "#222840"
    }

    transform: Translate {
        Behavior on y {
            NumberAnimation {
            }
        }
        y: enabled ? 0 : edge === Qt.TopEdge ? -button.height : button.height
    }
}
