import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
Rectangle {

    width: 960
    height: 720
    signal checkedAll(int networkNum,bool isOk)
    signal checkedOne(int networkNum,int index,bool isOk)
    signal networkChanged(int networkNum)
    ListModel
    {

        id:listModel
        ListElement
        {
            name:"0"
            checkedOnce:false
        }



    }

    Text {
        id: titleTxt
        anchors.top: parent.top
        anchors.topMargin: 10
        text: qsTr("设置主机打开或关闭")
        font.pointSize: 14
    }

    Row
    {
        id:row
        anchors.top: titleTxt.bottom
        anchors.horizontalCenter:parent.horizontalCenter
        anchors.topMargin: 10
        Text {
            id: openTxt
            text: qsTr("打开:")
            height: 40
            verticalAlignment: Text.AlignVCenter

        }
        RadioButton
        {
            checkable:false
            autoExclusive:false
            checked: true
        }

        Text {
            id: closeTxt
            text: qsTr("关闭:")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }

        RadioButton
        {
            checkable:false
            autoExclusive:false
            checked: false
        }

    }

    Grid
    {
        id:grid
        //property bool isShield: true
        anchors.top: row.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10

        columns: 13
        spacing: 5
        Repeater {
            model: listModel

            RadioButton
            {
                text: name

                checkable:true
                autoExclusive:false
                checked: checkedOnce
                height: 25


                onClicked:
                {
                    checkedOnce = checked
                    emit:checkedOne(parseInt(networkTextField.text.replace(/\s+/g,"")),index,checked)

                }
                //              width: 30

            }

        }
    }

    Row
    {


        anchors.top: grid.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        spacing: 10

        RadioButton
        {
            id:allRadioBtn
            text: qsTr("全选")
            checkable:true
            autoExclusive:false
            height: 25
            onClicked:
            {
                for(var i=0;i<listModel.count;i++)
                {
                    listModel.setProperty(i,"checkedOnce",checked)
                }
                var curNetWorkValue = parseInt(networkTextField.text.replace(/\s+/g,""))
                emit:checkedAll(curNetWorkValue,checked)
            }
            // height: 25
        }

        Text {
            id: networkTxt
            // width: 30
            height: 20
            verticalAlignment: Text.AlignVCenter
            text: qsTr("网络号:")
        }

        TextField
        {
            id:networkTextField
            height: 25
            width: 80
            text: "0"
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
            validator: IntValidator{bottom: 0; top: 255;}
            onTextChanged:
            {
                if(text.length>0)
                {
                    emit:networkChanged(parseInt(text.replace(/\s+/g,"")))
                }
            }

        }



    }

    Component.onCompleted:
    {

        listModel.clear()
        for(var i=0;i<=255;i++)
        {
            listModel.insert(i,{"name":String("%1").arg(i),"checkedOnce":false})

        }
    }

    function setChecked(index,state)
    {
        listModel.setProperty(index,"checkedOnce",state)
    }
    function clearState()
    {
        for(var i=0;i<listModel.count;i++)
        {
            listModel.setProperty(i,"checkedOnce",false)
        }
    }
    function setAllRadioBtn(state)
    {
        allRadioBtn.checked = state

    }

    function retranslate()
    {
        titleTxt.text = qsTr("设置主机打开或关闭")
        openTxt.text = qsTr("打开:")
        closeTxt.text = qsTr("关闭:")
        allRadioBtn.text = qsTr("全选")
        networkTxt.text = qsTr("网络号:")
    }

}
