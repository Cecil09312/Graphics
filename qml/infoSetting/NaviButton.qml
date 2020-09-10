import QtQuick 2.7
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
Button {
    id: button

    font.pointSize: 11
    //font.family: "Times New Roman"
    flat:false
    background: Rectangle
    {
        id:backRect

        implicitWidth: 80
        implicitHeight: 40
        color: highlighted ? "gray" : "lightGray"
        radius:10
        border.width: 1
       // border.color:"lightGray"


    }




    onPressed: {
        highlighted = true
    }
    onReleased: {
       highlighted = false

    }

}
