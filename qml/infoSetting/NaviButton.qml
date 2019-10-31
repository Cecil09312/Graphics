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
        color: "lightgray"
        radius:10
//        layer.enabled: button.enabled
//        layer.effect: DropShadow {
//            verticalOffset: 1
//            //color: firmAlarmBtn.visualFocus ? "#330066ff" : "#aaaaaa"
//            samples: 4
//            spread: 0.2
//        }
    }

    //                onClicked: {
    //                    ArchitePlanView.firstFireAlarm()
    //                }
    onPressed: {
        highlighted = true
    }
    onReleased: {
       highlighted = false

    }

    onHighlightedChanged:
    {
       if(highlighted)
       {
          backRect.color="gray"
       }
       else
       {
          backRect.color="lightgray"
       }
    }
}
