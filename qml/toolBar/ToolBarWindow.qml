import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import crtWidget 1.0
import userManager 1.0

ToolBar {
    id: toolBar
    signal chAndEnSwitch()
    signal checkTheVersion()
    anchors.fill: parent
    background: Rectangle
    {
        border.width:1

    }

    //width: 30
    RowLayout {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        Layout.fillWidth: true
        spacing: 20
        Image {
            id: titleImage
            height: parent.height
            width: 200
            source: "qrc:/images/ldlogo.png"
            Layout.alignment: Qt.AlignLeft
        }

        Text {
            id: currentDataTimeText
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            //horizontalAlignment: Text.AlignHCenter
            font.family: "Times New Roman"
            font.pixelSize: 24
            color: "green"

            text: Qt.formatDateTime(new Date, "yyyy/MM/dd hh:mm:ss")
            //Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true
        }
        Text {
            id: titleText
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            //horizontalAlignment: Text.AlignHCenter
            font.family: "Times New Roman"
            font.pixelSize: 30
            font.italic: true
            font.bold: true
            color: "black"

            text: qsTr("消防控制室图形显示装置V6.0")
            // Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true
        }

        Text
        {
            id:programVerTxt
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            //horizontalAlignment: Text.AlignHCenter
            font.family: "Arial"
            font.pointSize:12
            color: "black"
            visible: false
            Layout.fillWidth: true

        }
        ToolButton {
            id: logInBtn
            Layout.alignment: Qt.AlignRight
            width: 30
            BorderImage {
                id: logInBtnImage
                source: "qrc:/images/userLogin.png"
                width: parent.width
                height: parent.height
            }

            ToolTip {

                id: logInBtnToolTip
                visible: logInBtn.hovered
                text: qsTr("登录")
                contentItem: Text {
                    text: logInBtnToolTip.text
                    font.family: "宋体"
                    font.bold: true
                    color: "yellow"
                    font.pointSize:11
                }

                background: Rectangle {
                    color: "transparent"
                    BorderImage {
                        source: "qrc:/images/dialog.png"
                        anchors.fill: parent
                    }
                }
                bottomMargin: 25
            }

            onClicked: {
                CrtWidget.loginWidgetShow()
            }
        }

        ToolButton {
            id: infoQueryBtn
            width: 30
            Layout.alignment: Qt.AlignRight
            BorderImage {
                id: infoQueryBtnImage
                source: "qrc:/images/search.png"
                width: parent.width
                height: parent.height
            }

            ToolTip {

                id: infoQueryToolTip
                visible: infoQueryBtn.hovered
                text: qsTr("信息查询")
                contentItem: Text {
                    text: infoQueryToolTip.text
                    font.family: "宋体"
                    font.bold: true
                    color: "yellow"
                    font.pointSize:11
                }

                background: Rectangle {
                    color: "transparent"
                    BorderImage {
                        source: "qrc:/images/dialog.png"
                        anchors.fill: parent
                    }
                }
                bottomMargin: 25
            }

            onClicked: {

                CrtWidget.queryViewShow()
                // CrtWidget.settingWindowShow()
            }
        }

        ToolButton {
            id: infoSettingBtn
            width: 30
            Layout.alignment: Qt.AlignRight
            BorderImage {
                id: infoSettingBtnImage
                source: "qrc:/images/settings.png"
                width: parent.width
                height: parent.height
            }

            ToolTip {

                id: infoSettingToolTip
                visible: infoSettingBtn.hovered
                text: qsTr("信息设置")
                contentItem: Text {
                    text: infoSettingToolTip.text
                    font.family: "宋体"
                    font.bold: true
                    color: "yellow"
                    font.pointSize:11
                }

                background: Rectangle {
                    color: "transparent"
                    BorderImage {
                        source: "qrc:/images/dialog.png"
                        anchors.fill: parent
                    }
                }
                bottomMargin: 25
            }

            onClicked: {
                CrtWidget.settingWindowShow()
            }
        }

        ToolButton {
            id: quitBtn
            Layout.alignment: Qt.AlignRight
            width: 30
            BorderImage {
                id: closeImage
                source: "qrc:/images/close.png"
                anchors.fill: parent
            }
            onClicked: {
                CrtWidget.widgetExit()
            }

            ToolTip {
                id: quitBtnToolTip
                visible: quitBtn.hovered
                text: qsTr("退出")
                contentItem: Text {
                    text: quitBtnToolTip.text
                    font.family: "宋体"
                    font.bold:  true
                    color: "yellow"
                    font.pointSize:11

                }

                background: Rectangle {
                    color: "transparent"
                    BorderImage {
                        source: "qrc:/images/dialog.png"
                        anchors.fill: parent
                    }
                }
                bottomMargin: 25
            }
        }
        Item {
            Layout.alignment: Qt.AlignRight
            width: 30

        }
    }

    MouseArea
    {
        id:mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked:
        {
            menu.open()
        }

    }

    Menu
    {
        id:menu
        //      MenuItem
        //      {
        //        text:qsTr("在线状态")
        //        onTriggered: showOnlineState()
        //      }
        MenuItem
        {
            id:checkVersionItem
            text:qsTr("通讯板版本查询")
            onTriggered: checkTheVersion()
        }
        MenuItem
        {
            id:enAndChSwitchItem
            text:qsTr("中英文切换")
            enabled:UserManager.userRight()===UserManager.Super ? true:false
            onTriggered: chAndEnSwitch()



        }
    }


    Connections
    {
        target: UserManager
        onUserRightChanged:
        {
            if(right===UserManager.Super)
            {
                enAndChSwitchItem.enabled = true;
            }
            else
            {
                enAndChSwitchItem.enabled = false;
            }

        }

    }

    function setTime()
    {

        currentDataTimeText.text = Qt.formatDateTime(new Date,"yyyy/MM/dd hh:mm:ss")
    }
    function setProgramVer(programVer)
    {
        programVerTxt.visible= true
        programVerTxt.text = programVer
    }
    function setProgramTxtVisible(isVisible)
    {
        programVerTxt.visible = isVisible
    }

    function retranslate()
    {
        if(CrtWidget.isEnglish())
        {
            titleText.font.pixelSize=28
            programVerTxt.font.pointSize =10

        }
        else
        {
            titleText.font.pixelSize = 30
            programVerTxt.font.pointSize=12
        }
        titleText.text = qsTr("消防控制室图形显示装置V6.0")
        logInBtnToolTip.text =qsTr("登录")
        infoQueryToolTip.text = qsTr("信息查询")
        infoSettingToolTip.text=qsTr("信息设置")
        quitBtnToolTip.text =qsTr("退出")
        checkVersionItem.text = qsTr("通讯板版本查询")
        enAndChSwitchItem.text = qsTr("中英文切换")


    }
}

