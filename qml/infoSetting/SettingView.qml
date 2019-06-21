import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import "qrc:/qml/infoSetting"
import "qrc:/qml/databaseSetting"
import QtQuick.Controls.Styles 1.4

Controls1_4.TabView {

    anchors.fill: parent
    Controls1_4.Tab {
        title: qsTr("串口设置")
        id: serialPortSettingTab
        SerialPortSetting {
            id: serialPortSetting
        }
    }

    Controls1_4.Tab {
        title: qsTr("建筑平面图设置")
        GraphicsViewSetting {
            id: graphicsViewSetting
        }
    }

    Controls1_4.Tab {
        title: qsTr("密码设置")
        PasswordSetting {
            id: passwordSetting
        }
    }

    //    Controls1_4.Tab {
    //        title: qsTr("信息查询")
    //        anchors.topMargin: 20

    //        InfoQuery {
    //            id: infoQuery
    //        }
    //    }
    Controls1_4.Tab {
        title: qsTr("系统图设置")
        SysArchitePlanSetting {
            id: sysArchitePlanSetting
        }
    }

    Controls1_4.Tab {
        title: qsTr("语音设置")
        SpeechSetting {
            id: speechSetting
        }
    }

    Controls1_4.Tab {
        title: qsTr("维保信息")
        MaintenanceInfo {
            id: maintenanceInfo
        }
    }

    Controls1_4.Tab {
        title: qsTr("消防控制室管理信息")
        InfoTransport {
            id: infoTransport
        }
    }

    Controls1_4.Tab {
        title: qsTr("网络设置")
        NetworkSetting {
            id: networkSetting
        }
    }

    Controls1_4.Tab {
        title: qsTr("数据库设置")
        MySqlSetting {
            id: mySqlSettinng
        }
    }
    style: TabViewStyle {
        frameOverlap: 1
        tab: Rectangle {
            color: styleData.selected ? "steelblue" : "lightsteelblue"
            border.color: "steelblue"
            implicitWidth: Math.max(text.width + 4, 80)
            implicitHeight: 40
            radius: 2
            Text {
                id: text
                anchors.centerIn: parent
                text: styleData.title
                color: styleData.selected ? "white" : "black"
            }
        }
        frame: Rectangle {
            color: "white"
        }
    }
}
