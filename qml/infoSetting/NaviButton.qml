import QtQuick 2.7
import QtQuick.Controls 2.2
//import QtGraphicalEffects 1.0
Button {
    id: button

    font.pointSize: 14
    font.family: "宋体"
    background: Rectangle
    {
        id:backRect

        implicitWidth: 100
        implicitHeight: 40
        color: highlighted ? "gray" : "lightgray"
        radius:10

    }

    onPressed: {
        highlighted = true
    }
    onReleased: {
       highlighted = false

    }

}
