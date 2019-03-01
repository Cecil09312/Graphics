import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import "../qmlTable"
import QtQuick.Controls.Styles 1.4

Controls1_4.TabView {
    anchors.topMargin: 20
    Controls1_4.Tab {
        title: qsTr("报警信息查询")
        anchors.topMargin: 20
        AlarmInfoTableItem {
            id: alarmInfo
        }
    }

    Controls1_4.Tab {
        title: qsTr("设备状态查询")
        anchors.topMargin: 20
        DeviceStateTableItem {
            id: deviceState
        }
    }

    Controls1_4.Tab {
        title: qsTr("操作事件查询")
        anchors.topMargin: 20
        OperaEventTableItem {
            anchors.topMargin: 20
            id: operaEvent
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
            id: rec
            color: "white"
            transitions: Transition {
                PropertyAnimation {
                    target: rec
                    property: "opacity"
                    duration: 100
                    from: 0
                    to: 1
                }
            }
        }
    }
}
