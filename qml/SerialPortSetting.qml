import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Rectangle {
    width: 420
    height: 360
    GridLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            id: port
            text: qsTr("端口")
            Layout.row: 0
            Layout.column: 0
        }
        ComboBox {
            id: portComboBox
            Layout.row: 0
            Layout.column: 1
        }

        Text {
            id: baud
            text: qsTr("波特率")
            Layout.row: 1
            Layout.column: 0
        }
        ComboBox {
            id: baudComboBox
            Layout.row: 1
            Layout.column: 1
        }

        Text {
            id: dataBits
            text: qsTr("数据位")
            Layout.row: 2
            Layout.column: 0
        }
        ComboBox {
            id: dataBitsComboBox
            Layout.row: 2
            Layout.column: 1
        }

        Text {
            id: stopBits
            text: qsTr("停止位")
            Layout.row: 3
            Layout.column: 0
        }
        ComboBox {
            id: stopBitsComboBox
            Layout.row: 3
            Layout.column: 1
        }

        Text {
            id: parity
            text: qsTr("校验")
            Layout.row: 4
            Layout.column: 0
        }
        ComboBox {
            id: parityComboBox
            Layout.row: 4
            Layout.column: 1
        }

        Text {
            id: flowContral
            text: qsTr("流控制")
            Layout.row: 5
            Layout.column: 0
        }
        ComboBox {
            id: flowContralComboBox
            Layout.row: 5
            Layout.column: 1
        }
    }
}
