import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4

Item {
    width: 660
    height: 640
    property int indicatorNum: 256
    signal editIndicatorState(string networkNum)
    Text {
        id: txt
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 14
        color: "blue"
        text: qsTr("主机在线状态(指示灯左边的数字代表主机号)")
    }
    Row
    {
        id:legendRow
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: txt.bottom
        anchors.topMargin: 10
        spacing: 5
        Text
        {
            text: qsTr("在线:")
            height: 20
            verticalAlignment: Text.AlignVCenter

        }

        StatusIndicator
        {
            color:"green"
            active: true
        }


        Text {
            height: 20
            text: qsTr("    离线:")
            verticalAlignment: Text.AlignVCenter
        }
        StatusIndicator
        {
            color: "yellow"
            active: true
        }


        Text {
            height: 20
            text: qsTr("   初始状态:")
            verticalAlignment: Text.AlignVCenter

        }
        StatusIndicator
        {
            color: "gray"
            active: true
        }


        Text {
            height: 20
            text: qsTr("   网络号:")
            verticalAlignment: Text.AlignVCenter

        }
       TextField
       {
         id:networkNumTextField
         width: 100
         height: 30
         text:"0"

         validator: IntValidator{bottom: 0; top: 255;}

         onTextEdited:
         {
            emit:editIndicatorState(text)
          // console.log(text)
         }

       }


    }

    ListModel
    {
        id:listModel
        ListElement
        {
            name:"   0"
            indicatorColor:"gray"
        }

    }

    Grid
    {
        anchors.top: legendRow.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10

        columns: 12
        spacing: 5
        Repeater {
            model: listModel

            Row
            {
                Text {

                    text:name
                    height: 20
                    verticalAlignment: Text.AlignVCenter
                    //anchors.horizontalCenter: parent.horizontalCenter
                }
                StatusIndicator
                {
                    active: true
                    color: indicatorColor
                    height: 20
                    //anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }

    Component.onCompleted:
    {
        for(var i=1;i<indicatorNum;i++)
        {
            var space = new String
            if(i>=1&&i<10)
            {
                space ="   ";
            }
            else if(i>=10&&i<100)
            {
                space ="  ";
            }
            else if(i>=100&&i<1000)
            {

                space =" ";
            }
            listModel.append({"name":String("%1%2").arg(space).arg(i),"indicatorColor":"gray"})
        }

    }
    function setIndicatorState(index,colorStr)
    {
        listModel.setProperty(index,"indicatorColor",colorStr)
    }
    function resetIndicatorState()
    {
        for(var i=0;i<indicatorNum;i++)
        {
            listModel.setProperty(i,"indicatorColor","gray")
        }
    }

    function setNetworkNumValue(value)
    {
      networkNumTextField.text = String("%1").arg(value)
    }
    function networkNum()
    {
       return networkNumTextField.text
    }
}
