import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4
import userManager 1.0
import "../infoSetting"
import Qt.labs.platform 1.0
Item {
    width: 680
    height: 640
    property int indicatorNum: 256
    signal editIndicatorState(string networkNum)
    signal shieldIndicatorState(string extNum,string networkNum,bool curState)

    signal onlineStateSetting()
    Text {
        id: txt
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 10
        anchors.leftMargin: 5
        //anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 12
        width: parent.width
        wrapMode: Text.WordWrap
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
            id:onLineTxt
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
            id:offLineTxt
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
            id:initTxt
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
            id:networkTxt
            height: 20
            text: qsTr("   网络号:")
            verticalAlignment: Text.AlignVCenter

        }
        TextField
        {
            id:networkNumTextField
            width: 80
            height: 30
            text:"0"
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
            validator: IntValidator{bottom: 0; top: 255;}

            onTextEdited:
            {
                if(text.length>0)
                {
                    emit:editIndicatorState(String("%1").arg(parseInt(text.replace(/\s+/g,""))))
                }

            }

        }

        NaviButton
        {
            id:deviceOnlineBtn
            text: qsTr("设置")
            //enabled: UserManager.userRight() ===UserManager.Super ? true:false
            onClicked:
             {
                if(UserManager.userRight() ===UserManager.Super)
                {
                   onlineStateSetting()
                }
                else
                {
                  messageDialog.open()
                }

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
            visibledOne:false
        }

    }

    Grid
    {
        //property bool isShield: true
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
                    visible: visibledOne
                    //anchors.horizontalCenter: parent.horizontalCenter
                }
                StatusIndicator
                {
                    active: true
                    color: indicatorColor
                    height: 20
                    visible: visibledOne

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
            listModel.append({"name":String("%1%2").arg(space).arg(i),"indicatorColor":"gray","visibledOne":false})
        }



    }
    function setIndicatorState(index,colorStr)
    {
        listModel.setProperty(index,"indicatorColor",colorStr)
    }

    function resetVisibleAll()
    {
        for(var i=0;i<indicatorNum;i++)
        {
            setVisible(i,false)
        }

    }
    function resetIndicatorState()
    {
        for(var i=0;i<indicatorNum;i++)
        {
            setIndicatorState(i,"gray")
        }
    }

    function setVisible(curIndex,curState)
    {
        listModel.setProperty(curIndex,"visibledOne",curState)

    }

    function setNetworkNumValue(value)
    {
        networkNumTextField.text = String("%1").arg(value)
    }
    function networkNum()
    {
        return String("%1").arg(parseInt(networkNumTextField.text))
    }

    function retranslate()
    {

        txt.text = qsTr("主机在线状态(指示灯左边的数字代表主机号)")
        onLineTxt.text = qsTr("在线:")
        offLineTxt.text = qsTr("    离线:")
        initTxt.text = qsTr("   初始状态:")
        networkTxt.text = qsTr("   网络号:")
        deviceOnlineBtn.text = qsTr("设置")

    }

    MessageDialog {
        id: messageDialog
        title: qsTr("信息提示")
        text: qsTr("在超级用户模式下才可打开")
        //standardButtons: StandardButton.Yes
        flags:Qt.WindowStaysOnTopHint|Qt.WindowMaximizeButtonHint|Qt.MSWindowsFixedSizeDialogHint|Qt.WindowCloseButtonHint
    }
}
