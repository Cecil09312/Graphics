import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import userManager 1.0
import operatorInfo 1.0

Item {

    width: 800
    height: 640
    ListModel {
        id: addUserModel
        ListElement {
            value: qsTr("管理员")
        }

        ListElement {
            value: qsTr("普通用户")
        }
    }

    ListModel {
        id: managerRightModel
        ListElement {
            value: qsTr("超级用户")
        }

        ListElement {
            value: qsTr("管理员")
        }
    }

    ListModel {
        id: changePasswordModel
        ListElement {
            value: qsTr("超级用户")
        }
        ListElement {
            value: qsTr("管理员")
        }

        ListElement {
            value: qsTr("普通用户")
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
                text: qsTr("增加或删除用户")
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
                model: changePasswordModel
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

            NaviButton {
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
                model: managerRightModel
                Layout.row: 0
                Layout.column: 1
                Layout.fillWidth: true
                onCurrentTextChanged: {
                    if (currentText === qsTr("超级用户")) {
                        addUserModel.clear()
                        addUserModel.append({
                                                value: qsTr("管理员")
                                            })
                        addUserModel.append({
                                                value: qsTr("普通用户")
                                            })
                        if (addUserModel.count > 0) {
                            newUserComboBox.currentIndex = 0
                        }
                    } else {
                        for (var i = 0; i < addUserModel.count; i++) {
                            var userRightObj = new Object
                            userRightObj = addUserModel.get(i)
                            if (userRightObj["value"] === qsTr("管理员")) {
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
                text: qsTr("操作用户权限")
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

            Row
            {
                Layout.row: 4
                Layout.column: 1
                spacing: 10
                NaviButton {
                    id: addNewUserconfineBtn

                    text: qsTr("增加")
                    onClicked: {
                        if(newUserPasswordTextField.text.length<=0)
                        {
                            warningMessageDialog.open()
                        }
                        else
                        {
                            if (newUserComboBox.currentText === qsTr("管理员")) {

                                if(!UserManager.userIsExist(newUserTextField.text,UserManager.Administrator))
                                {
                                    UserManager.addUser(newUserTextField.text,
                                                        UserManager.Administrator,
                                                        newUserPasswordTextField.text)
                                    addNewUserSuccessDialog.open()
                                    OperatorInfo.insertEvent(qsTr("添加用户成功"))

                                }
                                else
                                {
                                    addNewUserFailureDialog.open()
                                    OperatorInfo.insertEvent(qsTr("添加用户失败"),
                                                             qsTr("添加用户失败,或者用户已经存在"))

                                }

                            } else if (newUserComboBox.currentText === qsTr("普通用户")) {
                                if(!UserManager.userIsExist(newUserTextField.text,UserManager.User))

                                {
                                    UserManager.addUser(newUserTextField.text,
                                                        UserManager.User,
                                                        newUserPasswordTextField.text)
                                    addNewUserSuccessDialog.open()
                                    OperatorInfo.insertEvent(qsTr("添加用户成功"))

                                }
                                else
                                {
                                    addNewUserFailureDialog.open()
                                    OperatorInfo.insertEvent(qsTr("添加用户失败"),
                                                             qsTr("添加用户失败,或者用户已经存在"))

                                }

                            }
                        }
                    }

                }


                NaviButton
                {
                    id:removeBtn
                    text: qsTr("删除")
                    onClicked:
                    {
                        if(newUserPasswordTextField.text.length<=0)
                        {
                            warningMessageDialog.open()
                        }
                        else
                        {
                            if (newUserComboBox.currentText === qsTr("管理员")) {



                                if (UserManager.password(
                                            UserManager.Administrator,
                                            newUserTextField.text) === newUserPasswordTextField.text) {
                                    UserManager.removeUser(newUserTextField.text,
                                                           UserManager.Administrator,
                                                           newUserPasswordTextField.text)
                                    removeUserSuccessDialog.open()
                                    OperatorInfo.insertEvent(qsTr("删除用户成功"))
                                } else {
                                    removeUserFailureDialog.open()
                                    OperatorInfo.insertEvent(qsTr("删除失败(密码错误)或用户不存在"), qsTr("删除失败或用户不存在"))
                                }
                            } else if (newUserComboBox.currentText === qsTr("普通用户")) {


                                if (UserManager.password(
                                            UserManager.User,
                                            newUserTextField.text) === newUserPasswordTextField.text) {
                                    UserManager.removeUser(newUserTextField.text,
                                                           UserManager.User,
                                                           newUserPasswordTextField.text)
                                    removeUserSuccessDialog.open()
                                    OperatorInfo.insertEvent(qsTr("删除用户成功"))
                                } else {
                                    removeUserFailureDialog.open()
                                    OperatorInfo.insertEvent(qsTr("删除失败(密码错误)或用户不存在"), qsTr("删除失败或用户不存在"))
                                }
                            }
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
        } else if (userRight === qsTr("管理员")) {
            currentUserRight = UserManager.Administrator
            oldPasswordStr = UserManager.password(UserManager.Administrator,
                                                  currentUserName)
        } else if (userRight === qsTr("普通用户")) {
            currentUserRight = UserManager.User
            oldPasswordStr = UserManager.password(UserManager.User,
                                                  currentUserName)
        }


        if(oldPasswordTextFile.text.length<=0)
        {
            OperatorInfo.insertEvent(qsTr("修改密码失败"), qsTr("旧密码为空"))
            warningMessageDialog.open()
        }
        else
        {
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
                    OperatorInfo.insertEvent(qsTr("修改密码成功"))
                    infoMessageDialog.open()
                } else {
                    OperatorInfo.insertEvent(qsTr("修改密码失败"), qsTr("新密码为空"))
                    warningMessageDialog.open()
                }
            } else {
                OperatorInfo.insertEvent(qsTr("修改密码失败"), qsTr("初始密码错误"))
                criticalMessageDialog.open()
            }
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
        text: qsTr("有密码为空，请重新设置......")
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
        title: qsTr("错误信息提示")
        text: qsTr("添加用户失败,或者用户已经存在")
    }


    MessageDialog {
        id: removeUserSuccessDialog
        icon: StandardIcon.Information
        title: qsTr("信息提示")
        text: qsTr("删除用户成功")
    }

    MessageDialog {
        id: removeUserFailureDialog
        icon: StandardIcon.Critical
        title: qsTr("错误信息提示")
        text: qsTr("删除失败或用户不存在")
    }

    Component.onCompleted: {
        userName.visible = false
        userNameTextField.visible = false
        initSetting()

    }

    function initSetting()
    {
        addUserModel.clear()
        changePasswordModel.clear()
        managerRightModel.clear()
        if(UserManager.userRight()===UserManager.Super)
        {
            addUserModel.append({value:qsTr("管理员")})
            addUserModel.append({value:qsTr("普通用户")})
            changePasswordModel.append({value:qsTr("超级用户")})
            changePasswordModel.append({value:qsTr("管理员")})
            changePasswordModel.append({value:qsTr("普通用户")})
            managerRightModel.append({value:qsTr("超级用户")})
            managerRightModel.append({value:qsTr("管理员")})
        }
        else if(UserManager.userRight()===UserManager.Administrator)
        {
            addUserModel.append({value:qsTr("普通用户")})
            changePasswordModel.append({value:qsTr("管理员")})
            changePasswordModel.append({value:qsTr("普通用户")})
            managerRightModel.append({value:qsTr("管理员")})
        }

        userRightComboBox.currentIndex=0
        managerRightComboBox.currentIndex=0

    }
}
