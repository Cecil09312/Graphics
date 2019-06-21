import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import mySqlManager 1.0
import architePlanView 1.0
import operatorInfo 1.0
import speechObj 1.0

Item {
    width: 480
    height: 560

    GroupBox {
        title: qsTr("MySql数据库设置")
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        Column {
            spacing: 10
            Grid {
                id: infoSettingGrid
                columns: 2
                spacing: 10

                Text {
                    text: qsTr("主机")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    height: 30
                }
                TextField {
                    id: hostTextField
                }

                Text {

                    text: qsTr("端口")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    height: 30
                }
                TextField {
                    id: portTextField
                }

                Text {

                    text: qsTr("用户名")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    height: 30
                }
                TextField {
                    id: userNameTextField
                }

                Text {

                    text: qsTr("密码")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    height: 30
                }
                TextField {
                    id: passwordTextField
                    echoMode: TextInput.Password
                }

                Text {

                    text: qsTr("数据库名")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    height: 30
                }
                TextField {
                    id: databaseNameTextField
                }
            }

            Row {

                spacing: 5
                Button {
                    id: saveBtn
                    text: qsTr("保存")
                    onClicked: {
                        ArchitePlanView.saveMySqlInfo(
                                    hostTextField.text, userNameTextField.text,
                                    passwordTextField.text,
                                    databaseNameTextField.text,
                                    parseInt(portTextField.text))
                    }
                }

                Button {
                    id: connectBtn
                    text: qsTr("连接")

                    onClicked: {
                        MySqlManager.hostName = hostTextField.text
                        MySqlManager.userName = userNameTextField.text
                        MySqlManager.password = passwordTextField.text
                        MySqlManager.databaseName = databaseNameTextField.text
                        MySqlManager.port = parseInt(portTextField.text)
                        MySqlManager.close()
                        MySqlManager.open()
                        console.log(MySqlManager.isOpen())
                        if (!MySqlManager.isOpen()) {
                            OperatorInfo.insertEvent(qsTr("MySql数据库连接"),
                                                     qsTr("失败"))
                            if (!SpeechObj.alarmTextExist(
                                        qsTr("MySql数据库连接失败"))) {
                                SpeechObj.insertAlarmText(qsTr("MySql数据库连接失败"))
                            }

                            connectStateText.text = qsTr("数据库连接:失败")
                            connectStateText.color = "red"
                        } else {
                            OperatorInfo.insertEvent(qsTr("MySql数据库连接"),
                                                     qsTr("成功"))
                            SpeechObj.removeAlarmText(qsTr("MySql数据库连接失败"))

                            if (!MySqlManager.tableIsExist("sys_status")) {
                                MySqlManager.executeQuery(
                                            "create table sys_status ( sys_name text,main_power text,prepare_power text,hand_auto_state text,run_state text);")
                            }

                            if (!MySqlManager.tableIsExist("alarm_info")) {
                                MySqlManager.executeQuery(
                                            "create table alarm_info ( sys_name text,device_num text,alarm_type text,current_state text,alarm_time text);")
                            }

                            if (!MySqlManager.tableIsExist("fault_state")) {
                                MySqlManager.executeQuery(
                                            "create table fault_state ( sys_name text,fault_type text,fault_state text,run_state text);")
                            }

                            connectStateText.text = qsTr("数据库连接:成功")
                            connectStateText.color = "green"
                        }
                    }
                }

                Button {
                    id: closeBtn
                    text: qsTr("关闭")
                    onClicked: {
                        MySqlManager.close()
                        if (!SpeechObj.alarmTextExist(qsTr("MySql数据库连接失败"))) {
                            SpeechObj.insertAlarmText(qsTr("MySql数据库连接失败"))
                        }
                        connectStateText.text = qsTr("数据库连接:失败")
                        connectStateText.color = "red"
                    }
                }
            }

            Text {

                id: connectStateText
                text: qsTr("数据库连接:")
                font.pointSize: 12
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    Component.onCompleted: {
        hostTextField.text = MySqlManager.hostName
        userNameTextField.text = MySqlManager.userName
        passwordTextField.text = MySqlManager.password
        databaseNameTextField.text = MySqlManager.databaseName
        portTextField.text = String("%1").arg(MySqlManager.port)
    }
}
