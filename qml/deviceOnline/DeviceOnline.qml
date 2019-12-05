import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4

Item {
    width: 540
    height: 360
    property int indicatorNum: 64
    Row
    {
        id:legendRow
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
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


    }

    ListModel
    {
        id:listModel
        ListElement
        {
            name:"   0号主机"
            indicatorColor:"gray"
        }

    }

    Grid
    {
        anchors.top: legendRow.bottom
        anchors.topMargin: 20
        columns: 6
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
            listModel.append({"name":String("%1%2号主机").arg(space).arg(i),"indicatorColor":"gray"})
        }

    }
    function setIndicatorState(index,colorStr)
    {
        listModel.setProperty(index,"indicatorColor",colorStr)
    }
}
