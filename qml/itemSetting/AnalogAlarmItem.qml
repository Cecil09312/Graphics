import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import "../infoSetting"
Rectangle {
    width: 300
    height: 360

    Column
    {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 5
    Grid {

        spacing: 5
        columns: 2
        columnSpacing: 10

        Text {
            id:extTxt
            text: qsTr("分机号")
            height:  40
           verticalAlignment:Text.AlignVCenter
        }
        TextField {
            id: extNumTextField
            width: 180
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"

        }
        Text {
            id:loopTxt
            text: qsTr("回路号")
            height:  40
           verticalAlignment:Text.AlignVCenter
        }
        TextField {
            id: loopNumTextField
            width: 180
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        Text {
            id:addrTxt
            text: qsTr("地址号")
            height:  40
           verticalAlignment:Text.AlignVCenter
        }
        TextField {
            id: addrNumTextField

            width: 180
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }

        Text {
            id:networkNumTxt
            text: qsTr("网络号")
            height:  40
           verticalAlignment:Text.AlignVCenter
        }
        TextField {
            id: networkNumTextField

            width: 180
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }

        Text {
            id:stateTxt
            text: qsTr("状态")
            height:  40
           verticalAlignment:Text.AlignVCenter

        }
        ComboBox {
            id: alarmStateComboBox
            width: 180

            model: [qsTr("火警"), qsTr("监管"),qsTr("启动"), qsTr("反馈"),qsTr("故障"),qsTr("屏蔽")]
        }





    }

    Grid {

        spacing: 5
        columns: 3
        Layout.fillWidth: true
        NaviButton {

            id: creatAlarmBtn

            text: qsTr("开始")
            font.pointSize: 10
            width: 80
            onClicked: {

                ArchitePlanView.createAlarm(extNumTextField.text.replace(/\s+/g,""),
                                            loopNumTextField.text.replace(/\s+/g,""),
                                            addrNumTextField.text.replace(/\s+/g,""),
                                            networkNumTextField.text.replace(/\s+/g,""),
                                            alarmStateComboBox.currentText.replace(/\s+/g,""),
                                            qsTr("模拟")+alarmStateComboBox.currentText)

            }
        }

        NaviButton {

            id: deleteAlarmBtn
            text: qsTr("恢复")
            font.pointSize: 10
            width: 80
            onClicked: {

                ArchitePlanView.eliminateAlarm(extNumTextField.text,
                                               loopNumTextField.text, addrNumTextField.text,
                                               networkNumTextField.text,qsTr(alarmStateComboBox.currentText))
            }
        }

        NaviButton {

            id: clearAlarmBtn

            text: qsTr("清除")
            font.pointSize: 10
            width: 80
            onClicked: {

                ArchitePlanView.clearAlarm(true)
            }
        }
    }

    Text {
        id:warningTxt

        font.pointSize: 10

        color: "red"
        text:qsTr("注意：请在没有任何报警的情况下进行模拟。")

    }
    }

    function retranslate()
    {
        extTxt.text = qsTr("分机号")
        loopTxt.text = qsTr("回路号")
        addrTxt.text = qsTr("地址号")
        networkNumTxt.text = qsTr("网络号")
        stateTxt.text = qsTr("状态")
        alarmStateComboBox.model =[qsTr("火警"), qsTr("监管"),qsTr("启动"), qsTr("反馈"),qsTr("故障"),qsTr("屏蔽")]
        creatAlarmBtn.text = qsTr("开始")
        deleteAlarmBtn.text = qsTr("恢复")
        clearAlarmBtn.text =qsTr("清除")
        warningTxt.text = qsTr("注意：请在没有任何报警的情况下进行模拟。")

    }
}
