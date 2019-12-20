import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0

ToolBar {
    id: toolBar
    signal showOnlineState()
    signal chAndEnSwitch()
    anchors.fill: parent

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
            font.family: "宋体"
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
            font.family: "宋体"
            font.pixelSize: 30
            font.italic: true
            font.bold: true
            color: "black"

            text: qsTr("消防控制室图形显示装置V6.0")
            // Layout.alignment: Qt.AlignLeft
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
                text: qsTr("登陆")
                contentItem: Text {
                    text: logInBtnToolTip.text
                    font.family: "宋体"
                    font.bold: true
                    color: "black"
                }

                background: Rectangle {
                    color: "transparent"
                    BorderImage {
                        source: "qrc:/images/dialog.png"
                        anchors.fill: parent
                    }
                }
                bottomMargin: 30
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
                    color: "black"
                }

                background: Rectangle {
                    color: "transparent"
                    BorderImage {
                        source: "qrc:/images/dialog.png"
                        anchors.fill: parent
                    }
                }
                bottomMargin: 30
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
                    color: "black"
                }

                background: Rectangle {
                    color: "transparent"
                    BorderImage {
                        source: "qrc:/images/dialog.png"
                        anchors.fill: parent
                    }
                }
                bottomMargin: 30
            }

            onClicked: {
                CrtWidget.settingWindowShow()
            }
        }

//        ToolButton {
//            id: helpBtn
//            width: 30
//            BorderImage {
//                id: helpImage
//                source: "qrc:/images/help.png"
//                width: parent.width
//                height: parent.height
//            }
//            ToolTip {

//                id: helpBtnToolTip
//                visible: helpBtn.hovered
//                text: qsTr("帮助")
//                contentItem: Text {
//                    text: helpBtnToolTip.text
//                    font.family: "宋体"
//                    font.bold: true
//                    color: "black"
//                }

//                background: Rectangle {
//                    color: "transparent"
//                    BorderImage {
//                        source: "qrc:/images/dialog.png"
//                        anchors.fill: parent
//                    }
//                }
//                bottomMargin: 30
//            }
//            onClicked: {
//                CrtWidget.openHelpFile()
//            }
//        }
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
                text: "退出"
                contentItem: Text {
                    text: quitBtnToolTip.text
                    font.family: "宋体"
                    font.bold: true
                    color: "black"
                }

                background: Rectangle {
                    color: "transparent"
                    BorderImage {
                        source: "qrc:/images/dialog.png"
                        anchors.fill: parent
                    }
                }
                bottomMargin: 30
            }
        }
        Item {
            Layout.alignment: Qt.AlignRight
            width: 30

        }
    }


    Timer {
        id: time
        interval: 1000
        repeat: true
        onTriggered: {
            currentDataTimeText.text = Qt.formatDateTime(new Date,
                                                         "yyyy/MM/dd hh:mm:ss")
        }
    }

    Component.onCompleted: {
        time.running = true
    }
    Component.onDestruction: {
        time.running = false
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
      MenuItem
      {
        text:qsTr("在线状态")
        onTriggered: showOnlineState()
      }
//      MenuItem
//      {
//        text:qsTr("中英文切换")
//        onTriggered: chAndEnSwitch()
//      }
    }
}

