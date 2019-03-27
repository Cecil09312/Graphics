import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
//import QtQuick.VirtualKeyboard 2.1
import userManager 1.0
import QtQuick.Dialogs 1.2
import operatorInfo 1.0

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
            id: userRight
            text: qsTr("用户权限")
            Layout.row: 0
            Layout.column: 0
        }
        ComboBox {
            id: userRightComboBox
            model: ["超级用户", "工程人员", "员工"]
            Layout.row: 0
            Layout.column: 1
            Layout.fillWidth: true
            onCurrentTextChanged: {
                if (currentText == "超级用户") {
                    userName.visible = false
                    userNameTextField.visible = false
                } else {
                    userName.visible = true
                    userNameTextField.visible = true
                }
            }
        }
        Text {
            id: userName
            text: qsTr("用户名")
            Layout.row: 1
            Layout.column: 0
        }
        TextField {
            id: userNameTextField
            Layout.row: 1
            Layout.column: 1
            Layout.fillWidth: true
            placeholderText: qsTr("用户名")
        }

        Text {

            id: password
            text: qsTr("密码")
            Layout.row: 2
            Layout.column: 0
        }
        TextField {
            id: passwordTextField
            Layout.row: 2
            Layout.column: 1
            echoMode: TextInput.Password
            placeholderText: qsTr("密码")
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
                              | Qt.ImhSensitiveData | Qt.ImhNoPredictiveText
            Layout.fillWidth: true
        }

        RowLayout {
            Layout.row: 3
            Layout.column: 1
            spacing: 5
            Layout.topMargin: 10
            Button {
                text: qsTr("登陆")
                onClicked: {
                    var passwordStr = new String
                    var userRight = new Number
                    if (userRightComboBox.currentText === qsTr("超级用户")) {
                        userRight = UserManager.Super
                        passwordStr = UserManager.password(UserManager.Super,
                                                           "super")
                    } else if (userRightComboBox.currentText === qsTr("工程人员")) {
                        userRight = UserManager.Engineer
                        passwordStr = UserManager.password(
                                    UserManager.Engineer,
                                    userNameTextField.text)
                    } else if (userRightComboBox.currentText === qsTr("员工")) {
                        userRight = UserManager.Employee
                        passwordStr = UserManager.password(
                                    UserManager.Employee,
                                    userNameTextField.text)
                    }

                    if (passwordTextField.text === passwordStr) {

                        UserManager.setUserRight(userRight)
                        if (userRight === UserManager.Super) {
                            UserManager.setUserName("super")
                        } else {
                            UserManager.setUserName(userNameTextField.text)
                        }
                        OperatorInfo.insertEvent(qsTr("用户登陆"))
                        infoMessageDialog.open()
                    } else {
                        OperatorInfo.insertEvent(qsTr("用户登陆"), qsTr("失败"))
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
        text: qsTr("密码或者用户名错误，请重新输入......")
        icon: StandardIcon.Critical
        standardButtons: StandardButton.Yes
    }

    MessageDialog {
        id: infoMessageDialog
        standardButtons: StandardButton.Yes | StandardButton.No
        title: qsTr("信息提示")
        text: qsTr("密码输入正确!登陆成功!")
        icon: StandardIcon.Information
        onYes: {
            CrtWidget.logWidgetClose()
        }
    }

    Component.onCompleted: {
        userName.visible = false
        userNameTextField.visible = false
    }

    function clearLoginInfo() {
        userNameTextField.clear()
        passwordTextField.clear()
    }
}
