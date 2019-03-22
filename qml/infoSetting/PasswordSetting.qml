import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import userManager 1.0

Item {

    ListModel {
        id: addUserModel
        ListElement {
            value: qsTr("工程人员")
        }

        ListElement {
            value: qsTr("员工")
        }
    }
    Column {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 5
        Row {
            RadioButton {
                id: passwordChangeRadionBtn
                text: qsTr("密码修改")
                checked: true
                autoExclusive: true
                onClicked: {

                    if (checked == true) {

                        changePasswordGridLayout.visible = true
                        addUserGridLayout.visible = false
                    } else {
                        changePasswordGridLayout.visible = false
                        addUserGridLayout.visible = true
                    }
                }
            }
            RadioButton {
                id: addUserRadionBtn
                text: qsTr("增加用户")
                Layout.fillWidth: true
                autoExclusive: true
                onClicked: {

                    if (checked == true) {

                        changePasswordGridLayout.visible = false
                        addUserGridLayout.visible = true
                    } else {
                        changePasswordGridLayout.visible = true
                        addUserGridLayout.visible = false
                    }
                }
            }
        }
        GridLayout {
            id: changePasswordGridLayout
            visible: true
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
                id: oldPassword
                text: qsTr("初始密码")
                Layout.row: 2
                Layout.column: 0
            }
            TextField {
                id: oldPasswordTextFile
                Layout.row: 2
                Layout.column: 1
                echoMode: TextInput.Password
                placeholderText: qsTr("初始密码")
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
                                  | Qt.ImhSensitiveData | Qt.ImhNoPredictiveText
            }

            Text {
                Layout.row: 3
                Layout.column: 0
                id: newPassword
                text: qsTr("新密码")
            }
            TextField {
                Layout.row: 3
                Layout.column: 1
                id: newPasswordTextFiled
                echoMode: TextInput.Password
                placeholderText: qsTr("新密码")
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
                                  | Qt.ImhSensitiveData | Qt.ImhNoPredictiveText
            }

            Button {
                id: confineBtn
                Layout.row: 4
                Layout.column: 1
                text: qsTr("确认")
                onClicked: {
                    setUserInfo()
                }
            }
        }

        GridLayout {
            id: addUserGridLayout
            visible: false
            Text {
                id: managerRight
                text: qsTr("管理员权限")
                Layout.row: 0
                Layout.column: 0
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            ComboBox {
                id: managerRightComboBox
                model: ["超级用户", "工程人员"]
                Layout.row: 0
                Layout.column: 1
                Layout.fillWidth: true
                onCurrentTextChanged: {
                    if (currentText === qsTr("超级用户")) {
                        addUserModel.clear()
                        addUserModel.append({
                                                value: qsTr("工程人员")
                                            })
                        addUserModel.append({
                                                value: qsTr("员工")
                                            })
                        if (addUserModel.count > 0) {
                            newUserComboBox.currentIndex = 0
                        }
                    } else {
                        for (var i = 0; i < addUserModel.count; i++) {
                            var userRightObj = new Object
                            userRightObj = addUserModel.get(i)
                            if (userRightObj["value"] === qsTr("工程人员")) {
                                addUserModel.remove(i)
                            }
                        }
                        if (addUserModel.count > 0) {
                            newUserComboBox.currentIndex = 0
                        }
                    }
                }
            }

            Text {
                id: newUserRight
                text: qsTr("新用户权限")
                Layout.row: 1
                Layout.column: 0
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            ComboBox {
                id: newUserComboBox
                model: addUserModel
                Layout.row: 1
                Layout.column: 1
                Layout.fillWidth: true
            }

            Text {
                id: newUserName
                text: qsTr("用户名")
                Layout.row: 2
                Layout.column: 0
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            TextField {
                id: newUserTextField
                Layout.row: 2
                Layout.column: 1
                Layout.fillWidth: true
            }

            Text {
                id: newUserPassword
                text: qsTr("密码")
                Layout.row: 3
                Layout.column: 0
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            TextField {
                id: newUserPasswordTextField
                Layout.row: 3
                Layout.column: 1
                Layout.fillWidth: true
                echoMode: TextInput.Password
            }

            Button {
                id: addNewUserconfineBtn
                Layout.row: 4
                Layout.column: 1
                text: qsTr("确认")
                onClicked: {
                    if (newUserComboBox.currentText === qsTr("工程人员")) {
                        UserManager.addUser(newUserTextField.text,
                                            UserManager.Engineer,
                                            newUserPasswordTextField.text)

                        if (UserManager.password(
                                    UserManager.Engineer,
                                    newUserTextField.text) === newUserPasswordTextField.text) {
                            addNewUserSuccessDialog.open()
                        } else {
                            addNewUserFailureDialog.open()
                        }
                    } else if (newUserComboBox.currentText === qsTr("员工")) {
                        UserManager.addUser(newUserTextField.text,
                                            UserManager.Employee,
                                            newUserPasswordTextField.text)

                        if (UserManager.password(
                                    UserManager.Employee,
                                    newUserTextField.text) === newUserPasswordTextField.text) {
                            addNewUserSuccessDialog.open()
                        } else {
                            addNewUserFailureDialog.open()
                        }
                    }
                }
            }
        }
    }

    function setUserInfo() {

        var userRight = userRightComboBox.currentText
        var oldPasswordStr = new String
        var currentUserName = new String
        currentUserName = userNameTextField.text
        var currentUserRight
        if (userRight === qsTr("超级用户")) {
            currentUserRight = UserManager.Super
            oldPasswordStr = UserManager.password(UserManager.Super, "super")
        } else if (userRight === qsTr("工程人员")) {
            currentUserRight = UserManager.Engineer
            oldPasswordStr = UserManager.password(UserManager.Engineer,
                                                  currentUserName)
        } else if (userRight === qsTr("员工")) {
            currentUserRight = UserManager.Employee
            oldPasswordStr = UserManager.password(UserManager.Employee,
                                                  currentUserName)
        }

        if (oldPasswordTextFile.text === oldPasswordStr) {
            if (newPasswordTextFiled.text.length > 0) {
                if (currentUserRight === UserManager.Super) {
                    UserManager.setPassword(currentUserRight,
                                            newPasswordTextFiled.text, "super")
                } else {
                    UserManager.setPassword(currentUserRight,
                                            newPasswordTextFiled.text,
                                            currentUserName)
                }
                infoMessageDialog.open()
            } else {
                warningMessageDialog.open()
            }
        } else {
            criticalMessageDialog.open()
        }
    }

    MessageDialog {
        id: criticalMessageDialog
        icon: StandardIcon.Critical
        title: qsTr("错误提示")
        text: qsTr("初始密码错误，请重新输入......")
    }

    MessageDialog {
        id: warningMessageDialog
        icon: StandardIcon.Warning
        title: qsTr("警告信息")
        text: qsTr("新密码为空，请重新设置......")
    }

    MessageDialog {
        id: infoMessageDialog
        icon: StandardIcon.Information
        title: qsTr("信息提示")
        text: qsTr("密码修改成功")
    }

    MessageDialog {
        id: addNewUserSuccessDialog
        icon: StandardIcon.Information
        title: qsTr("信息提示")
        text: qsTr("添加用户成功")
    }

    MessageDialog {
        id: addNewUserFailureDialog
        icon: StandardIcon.Critical
        title: qsTr("警告信息")
        text: qsTr("添加用户失败,或者用户已经存在")
    }

    Component.onCompleted: {
        userName.visible = false
        userNameTextField.visible = false
    }
}
