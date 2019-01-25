import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtQuick.VirtualKeyboard 2.1
import userManager 1.0
import QtQuick.Dialogs 1.2

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
            id: userNameComboBox
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
                onClicked: {
                    var passwordStr = new String
                    var userRight = new Number
                    if (userNameComboBox.currentText === qsTr("超级用户")) {
                        userRight = UserManager.Super
                        passwordStr = UserManager.password(UserManager.Super)
                    } else if (userNameComboBox.currentText === qsTr("工程人员")) {
                        userRight = UserManager.Engineer
                        passwordStr = UserManager.password(UserManager.Engineer)
                    } else if (userNameComboBox.currentText === qsTr("员工")) {
                        userRight = UserManager.Employee
                        passwordStr = UserManager.password(UserManager.Employee)
                    }

                    if (passwordTextField.text === passwordStr) {

                        UserManager.setUserRight(userRight)
                        infoMessageDialog.open()
                    } else {
                        criticalMessageDialog.open()
                    }
                }
            }

            Button {
                text: qsTr("退出")
                onClicked: {
                    CrtWidget.logWidgetClose()
                }
            }
        }
    }

    MessageDialog {
        id: criticalMessageDialog
        title: qsTr("错误提示")
        text: qsTr("密码错误，请重新输入......")
        icon: StandardIcon.Critical
        standardButtons: StandardButton.Yes
    }

    MessageDialog {
        id: infoMessageDialog
        standardButtons: StandardButton.Yes | StandardButton.No
        title: qsTr("信息提示")
        text: qsTr("密码输入正确!")
        icon: StandardIcon.Information
        onYes: {
            CrtWidget.logWidgetClose()
        }
    }
}
