import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import userManager 1.0

Item {

    GridLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            id: userName
            text: qsTr("用户名")
            Layout.row: 0
            Layout.column: 0
        }
        ComboBox {
            id: userComboBox
            model: ["超级用户", "工程人员", "员工"]
            Layout.row: 0
            Layout.column: 1
            Layout.fillWidth: true
        }
        Text {
            id: oldPassword
            text: qsTr("初始密码")
            Layout.row: 1
            Layout.column: 0
        }
        TextField {
            id: oldPasswordTextFile
            Layout.row: 1
            Layout.column: 1
            echoMode: TextInput.Password
            placeholderText: qsTr("初始密码")
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
                              | Qt.ImhSensitiveData | Qt.ImhNoPredictiveText
        }

        Text {
            Layout.row: 2
            Layout.column: 0
            id: newPassword
            text: qsTr("新密码")
        }
        TextField {
            Layout.row: 2
            Layout.column: 1
            id: newPasswordTextFiled
            echoMode: TextInput.Password
            placeholderText: qsTr("新密码")
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
                              | Qt.ImhSensitiveData | Qt.ImhNoPredictiveText
        }

        Button {
            id: confineBtn
            Layout.row: 3
            Layout.column: 1
            text: qsTr("确认")
            onClicked: {
                setUserInfo()
            }
        }
    }

    function setUserInfo() {

        var userName = userComboBox.currentText
        var oldPasswordStr = new String
        var currentUserRight
        if (userName === qsTr("超级用户")) {
            currentUserRight = UserManager.Super
            oldPasswordStr = UserManager.password(UserManager.Super)
        } else if (userName === qsTr("工程人员")) {
            currentUserRight = UserManager.Engineer
            oldPasswordStr = UserManager.password(UserManager.Engineer)
        } else if (userName === qsTr("员工")) {
            currentUserRight = UserManager.Employee
            oldPasswordStr = UserManager.password(UserManager.Employee)
        }

        if (oldPasswordTextFile.text === oldPasswordStr) {
            if (newPasswordTextFiled.text.length > 0) {
                UserManager.setPassword(currentUserRight,
                                        newPasswordTextFiled.text)
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
}
