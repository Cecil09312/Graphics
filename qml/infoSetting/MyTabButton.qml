import QtQuick 2.0
import QtQuick.Controls 2.2

TabButton
{
    id:tabBtn
    contentItem: Label {
        text: tabBtn.text
        font: tabBtn.font
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: tabBtn.checked ? "white":"black"
    }
    background: Rectangle {
        color:tabBtn.checked ? "steelblue" : "lightsteelblue"
        border.color: "steelblue"
        //implicitWidth: Math.max(text.width + 4, 80)
        implicitHeight: 40
        radius: 2

    }
}
