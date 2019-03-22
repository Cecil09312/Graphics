import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Item {
    width: 420
    height: 320
    signal createAlarm(string extNum, string loopNum, string addrNum, string alarmState)
    signal clearAlarm
    GridLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            text: qsTr("分机号")
            Layout.row: 0
            Layout.column: 0
        }
        TextField {
            id: extNumTextField
            Layout.fillWidth: true
            Layout.row: 0
            Layout.column: 1
        }
        Text {
            text: qsTr("回路号")
            Layout.row: 1
            Layout.column: 0
        }
        TextField {
            id: loopNumTextField
            Layout.fillWidth: true
            Layout.row: 1
            Layout.column: 1
        }
        Text {
            text: qsTr("地址号")
            Layout.row: 2
            Layout.column: 0
        }
        TextField {
            id: addrNumTextField
            Layout.fillWidth: true
            Layout.row: 2
            Layout.column: 1
        }

        Text {
            text: qsTr("报警状态")
            Layout.row: 3
            Layout.column: 0
        }
        ComboBox {
            id: alarmStateComboBox
            Layout.fillWidth: true
            Layout.row: 3
            Layout.column: 1
            model: ["火警", "联动", "故障", "监管", "反馈", "屏蔽"]
        }

        Row {
            Layout.row: 4
            Layout.column: 1
            spacing: 10
            Layout.fillWidth: true
            Button {

                id: creatAlarmBtn

                text: qsTr("报警开始")
                onClicked: {
                    emit: createAlarm(extNumTextField.text,
                                      loopNumTextField.text,
                                      addrNumTextField.text,
                                      alarmStateComboBox.currentText)
                }
            }

            Button {

                id: clearAlarmBtn

                text: qsTr("报警清除")
                onClicked: {
                    emit: clearAlarm()
                }
            }
        }

        Text {
            Layout.row: 5
            Layout.column: 1
            text: qsTr("注意：请在没有任何正常报警发生的情况\n
下进行模拟；\"报警消除\"按键必须在模拟\n
状态下使用。模拟状态下不要使用\"复位\"\n
按键。")
            color: "red"
        }
    }
}
