import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ToolBar {
    id: toolBar
    anchors.fill: parent
    //width: 30
    RowLayout {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        Layout.fillWidth: true
        spacing: 20
        Text {
            id: titleText
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            //horizontalAlignment: Text.AlignHCenter
            font.family: "Times New Roman"
            font.pixelSize: 28
            color: "green"

            text: qsTr("消防控制室图形显示装置")
            Layout.alignment: Qt.AlignLeft
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
                    font.family: "Times New Roman"
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
                    font.family: "Times New Roman"
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
                    font.family: "Times New Roman"
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

        ToolButton {
            id: helpBtn
            width: 30
            BorderImage {
                id: helpImage
                source: "qrc:/images/help.png"
                width: parent.width
                height: parent.height
            }
            ToolTip {

                id: helpBtnToolTip
                visible: helpBtn.hovered
                text: qsTr("帮助")
                contentItem: Text {
                    text: helpBtnToolTip.text
                    font.family: "Times New Roman"
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
                    font.family: "Times New Roman"
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
}
