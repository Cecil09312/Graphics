import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtQuick.VirtualKeyboard 2.1

Rectangle {
    id: loginWindow
    width: 480
    height: 240
    GridLayout {
        id: gridLayout
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        columnSpacing: 5
        Text {
            id: userName
            text: qsTr("用户名")
            Layout.row: 0
            Layout.column: 0
        }
        ComboBox {
            model: ["超级用户", "工程人员", "员工"]
            Layout.row: 0
            Layout.column: 1
            Layout.fillWidth: true
        }
        Text {

            id: password
            text: qsTr("密码")
            Layout.row: 1
            Layout.column: 0
        }
        TextField {
            id: passwordTextField
            Layout.row: 1
            Layout.column: 1
            echoMode: TextInput.Password
            placeholderText: "Password field"
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
                              | Qt.ImhSensitiveData | Qt.ImhNoPredictiveText
            Layout.fillWidth: true
        }

        RowLayout {
            Layout.row: 2
            Layout.column: 1
            spacing: 5
            Layout.topMargin: 10
            Button {

                text: qsTr("登陆")
            }

            Button {
                text: qsTr("退出")
                onClicked: {
                    CrtWidget.logWidgetClose()
                }
            }
        }
    }
}
