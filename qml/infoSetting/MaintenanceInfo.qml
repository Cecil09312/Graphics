import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    anchors.fill: parent
    anchors.leftMargin: 5
    anchors.rightMargin: 5
    Row {
        id: maintenanceRow
        anchors.top: parent.top
        anchors.topMargin: 10
        spacing: 3
        Text {
            text: qsTr("设备编码:")
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: codeTextField
        }

        Text {
            text: qsTr("维保时间:")
        }

        TextField {
            id: timeTextField
        }

        Text {
            text: qsTr("维保员:")
        }

        TextField {
            id: engineerTextField
        }
    }
    Grid {
        id: maintenanceGrid
        anchors.top: maintenanceRow.bottom
        anchors.topMargin: 5
        columns: 2
        spacing: 5
        Text {
            text: qsTr("维保说明")
        }

        TextArea {

            width: maintenanceRow.width
            height: 100
            background: Rectangle {
                border.width: 1
                color: "transparent"
            }
        }

        Text {
            text: qsTr("状态现象")
        }

        TextArea {

            width: maintenanceRow.width
            height: 100
            background: Rectangle {
                border.width: 1
                color: "transparent"
            }
        }

        Text {
            text: qsTr("解决方法")
        }
        TextArea {

            width: maintenanceRow.width
            height: 100
            background: Rectangle {
                border.width: 1
                color: "transparent"
            }
        }

        Text {
            text: qsTr("内容描述")
        }

        TextArea {

            width: maintenanceRow.width
            height: 100
            background: Rectangle {
                border.width: 1
                color: "transparent"
            }
        }
    }

    Row {
        id: selectInfoRow
        spacing: 5
        anchors.top: maintenanceGrid.bottom
        anchors.topMargin: 5
        Text {
            text: qsTr("建筑名称")
        }

        TextField {
            id: buildingNameTextField
            width: 150
        }
        Text {
            text: qsTr("楼层")
        }

        TextField {
            id: floorTextField
            width: 150
        }

        Text {
            text: qsTr("部位")
        }

        TextField {
            id: locationTextField
            width: 150
        }

        Text {
            text: qsTr("系统")
        }

        TextField {
            id: sysTextField
            width: 150
        }
    }

    Button {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: selectInfoRow.bottom
        anchors.topMargin: 5
        text: qsTr("保存")
    }
}
