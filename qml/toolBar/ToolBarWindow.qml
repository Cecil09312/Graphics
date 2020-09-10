import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
//import crtWidget 1.0
import userManager 1.0
import speechObj 1.0

ToolBar {
    id: toolBar
    signal chAndEnSwitch()
    // signal checkTheVersion()
    anchors.fill: parent

    background: Rectangle
    {
        border.width:1
        color: "transparent"
    }

    //width: 30
    RowLayout {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top:parent.top
        Layout.fillWidth: true

        anchors.bottomMargin:5
        //spacing: 10


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
            //font.family: "Times New Roman"
            font.pixelSize: 24
            color: "green"

            text: Qt.formatDateTime(new Date, "yyyy/MM/dd hh:mm:ss")
            //Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true
        }
        Text {
            id: titleText
            height: parent.height
            Layout.fillWidth: true
            //            verticalAlignment: Text.AlignVCenter
            //            horizontalAlignment: Text.AlignHCenter
            //font.family: "Times New Roman"
            font.pixelSize: 30
            //font.italic: true
            font.bold: true
            color: "black"

            text: qsTr("软件版本V6.0")

            Layout.alignment: Qt.AlignLeft

        }

        Text
        {
            id:programVerTxt
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            //horizontalAlignment: Text.AlignHCenter
            //font.family: "Arial"
            font.pointSize:12
            color: "black"
            visible: false

            // Layout.fillWidth: true

        }

        Text
        {
            id:curUserInfoTxt
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            //horizontalAlignment: Text.AlignHCenter
            //font.family: "Arial"
            font.pointSize:12
            color: "green"
            Layout.rightMargin: 30
            // Layout.fillWidth: true

        }


        Row
        {
            spacing: 30
            // Layout.fillWidth: true
            ToolButton {
                id: chAndEnToolBtn
                width: 30
                Layout.alignment: Qt.AlignRight
                contentItem: Rectangle
                {
                    height: parent.height
                    Column
                    {

                        anchors.fill: parent
                        width: parent.width
                        // spacing: 5
                        Label{


                            width: 30
                            height: 30
                            id:contentText
                            anchors.left: parent.left
                            anchors.right: parent.right
                            background: Rectangle
                            {
                                color:"lightgray"
                                anchors.fill: parent

                            }

                            text:"中"
                            color: "blue"
                            font.pointSize: 14
                            font.bold: true
                            // width: parent.width
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter


                        }


                        Text
                        {
                            id:chAndEnTxt
                            font.pointSize: 11
                            //font.bold: true
                            text: qsTr("语言")
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter

                        }

                    }
                }

                onClicked:
                {
                    if(UserManager.userRight()===UserManager.Super)
                    {
                        emit:chAndEnSwitch()
                        if(CrtWidget.isEnglish())
                        {
                            contentText.text = "En"

                        }
                        else
                        {
                            contentText.text = "中"
                        }
                        //setCurrentUser()
                    }
                    else
                    {
                        messageDialog.open()

                    }
                }
            }
            ToolButton {
                id: logInBtn
                Layout.alignment: Qt.AlignRight
                width: 30
                contentItem: Rectangle {

                    height: parent.height
                    color: "transparent"
                    Column
                    {
                        anchors.fill: parent
                        BorderImage
                        {
                            id: logInBtnImage
                            source: "qrc:/images/userLogin.png"
                            width: 30
                            height: 30
                        }

                        Text
                        {
                            id:logInTxt
                            font.pointSize: 11
                            text: qsTr("登录")
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter

                        }
                    }

                }

                onClicked: {
                    CrtWidget.loginWidgetShow()
                }
            }

            ToolButton {
                id: infoQueryBtn
                width: 40
                height: parent.height
                Layout.alignment: Qt.AlignRight
                contentItem: Rectangle {

                    height: parent.height
                    color: "transparent"
                    Column
                    {
                        anchors.fill: parent
                        BorderImage
                        {
                            id: infoQueryBtnImage
                            source: "qrc:/images/search.png"
                            width: 30
                            height: 30
                        }

                        Text
                        {
                            id: infoQueryTxt
                            font.pointSize: 11
                            text: qsTr("信息查询")
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter

                        }
                    }
                }

                onClicked: {
                    CrtWidget.queryViewShow()
                }
            }

            ToolButton {
                id: infoSettingBtn
                width: 40
                Layout.alignment: Qt.AlignRight
                contentItem: Rectangle {

                    height: parent.height
                    color: "transparent"
                    Column
                    {
                        anchors.fill: parent
                        BorderImage
                        {
                            id: infoSettingImage
                            source: "qrc:/images/settings.png"
                            width: 30
                            height: 30
                        }

                        Text
                        {
                            id: infoSettingTxt
                            font.pointSize: 11
                            text: qsTr("信息设置")
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter

                        }
                    }

                }

                onClicked: {
                    CrtWidget.settingWindowShow()
                }
            }

            ToolButton {
                id: quitBtn
                Layout.alignment: Qt.AlignRight
                width: 40

                contentItem: Rectangle {

                    height: parent.height
                    color: "transparent"
                    Column
                    {
                        anchors.fill: parent
                        BorderImage
                        {
                            id: quitBtnImage
                            source: "qrc:/images/close.png"
                            width: 30
                            height: 30
                        }

                        Text
                        {
                            id: quitTxt
                            font.pointSize: 11
                            text: qsTr("退出")
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter

                        }
                    }

                }

                onClicked: {
                    CrtWidget.widgetExit()
                }


            }
        }



        Item {
            Layout.alignment: Qt.AlignRight
            width: 30

        }
    }


    MessageDialog {
        id: messageDialog
        title: qsTr("信息提示")
        text: qsTr("在超级用户模式下才可打开")
        //standardButtons: StandardButton.Yes
        flags:Qt.WindowStaysOnTopHint|Qt.WindowMaximizeButtonHint|Qt.MSWindowsFixedSizeDialogHint|Qt.WindowCloseButtonHint
    }


    Component.onCompleted:
    {
        setCurrentUser()
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
            titleText.font.pixelSize=26
            contentText.text="En"

        }
        else
        {
            titleText.font.pixelSize = 30
            contentText.text="中"
        }
        titleText.text = qsTr("软件版本V6.0")
        logInTxt.text =qsTr("登录")
        infoQueryTxt.text = qsTr("信息查询")
        infoSettingTxt.text=qsTr("信息设置")
        quitTxt.text =qsTr("退出")
        chAndEnTxt.text = qsTr("语言")
        messageDialog.title=qsTr("信息提示")
        messageDialog.text= qsTr("在超级用户模式下才可打开")
        setCurrentUser()

    }

    function  setCurrentUser()
    {
        var curUser = new String
        if(UserManager.userRight()===UserManager.Super)
        {
            curUser = qsTr("超级用户")
        }
        else if(UserManager.userRight()===UserManager.Administrator)
        {
            curUser = qsTr("管理员")
        }
        else
        {
            curUser = qsTr("普通用户")

        }

        curUserInfoTxt.text = curUser+"(%1)".arg(UserManager.userName())

    }
}

