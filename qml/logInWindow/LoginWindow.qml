import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
//import QtQuick.VirtualKeyboard 2.1
import userManager 1.0
import Qt.labs.platform 1.0
import operatorInfo 1.0
import "../infoSetting"
Rectangle {
    id: loginWindow
    width: 480
    height: 240
    property bool loginState: false
    signal startClose()
    signal closeView()
    signal userLogin()
    signal setTipInfo()

    ColumnLayout {
        id: columnLayout
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 5
        Row
        {

            id:curRow
            spacing: 5
            Layout.row: 0
            Layout.column: 0

            Text {
                id: userRightTxt
                text: qsTr("用户权限")
                height: 40
                width: 60
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment:Text.AlignHCenter

            }
            ComboBox {
                id: userRightComboBox
                model: [ qsTr("超级用户"),qsTr("管理员"),qsTr("普通用户")]
                currentIndex: 0
                width: 150
                onCurrentTextChanged: {
                    if (currentText == qsTr("超级用户")) {
                        userName.visible = false
                        userNameTextField.visible = false
                    } else {
                        userName.visible = true
                        userNameTextField.visible = true
                    }
                }
            }

            NaviButton {
                id:loginBtn
                text: qsTr("登录")
                width: 100
                onClicked: {
                    var passwordStr = new String
                    var userRight = new Number
                    if (userRightComboBox.currentText === qsTr("超级用户")) {
                        userRight = UserManager.Super
                        passwordStr = UserManager.password(UserManager.Super,
                                                           "super")
                    } else if (userRightComboBox.currentText === qsTr("管理员")) {
                        userRight = UserManager.Administrator
                        passwordStr = UserManager.password(
                                    UserManager.Administrator,
                                    userNameTextField.text)
                    } else if (userRightComboBox.currentText === qsTr("普通用户")) {
                        userRight = UserManager.User
                        passwordStr = UserManager.password(
                                    UserManager.User,
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
                        emit:userLogin()
                        emit:setTipInfo()
                        infoMessageDialog.open()
                        loginState = true
                    } else {
                        OperatorInfo.insertEvent(qsTr("用户登陆"), qsTr("失败"))
                        criticalMessageDialog.open()
                        loginState = false
                    }
                }
            }
 }

        Row
        {

            id:userRow

            spacing: 5
            Text {
                id: userName
                text: qsTr("用户名")
                height: 40
                width: 60
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment:Text.AlignHCenter
            }
            TextField {
                id: userNameTextField
                placeholderText: qsTr("用户名")
                selectByMouse: true
                selectionColor: "blue"
                selectedTextColor: "white"
                width: 150
            }


            Text {

                id: password
                text: qsTr("密码")
                height: 40
                width: 60
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment:Text.AlignHCenter

            }
            TextField {
                id: passwordTextField
                echoMode: TextInput.Password
                width: 150
                placeholderText: qsTr("密码")
                selectByMouse: true
                selectionColor: "blue"
                selectedTextColor: "white"
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
                                  | Qt.ImhSensitiveData | Qt.ImhNoPredictiveText
                Layout.fillWidth: true
            }

        }

        //            NaviButton {
        //                id:quitBtn
        //                text: qsTr("退出")
        //                onClicked: {
        //                    emit:closeView()
        //                }
        //            }
        // }
    }

    MessageDialog {
        id: criticalMessageDialog
        title: qsTr("错误提示")
        text: qsTr("密码或者用户名错误，请重新输入......")
        //icon: StandardIcon.Critical
        //standardButtons: StandardButton.Yes
        flags:Qt.WindowStaysOnTopHint|Qt.WindowMaximizeButtonHint|Qt.MSWindowsFixedSizeDialogHint|Qt.WindowCloseButtonHint
        buttons: MessageDialog.Yes
    }

    MessageDialog {
        id: infoMessageDialog
        buttons: MessageDialog.Yes
        // standardButtons: StandardButton.Yes | StandardButton.No
        title: qsTr("信息提示")
        text: qsTr("密码输入正确!登录成功!")
        flags:Qt.WindowStaysOnTopHint|Qt.WindowMaximizeButtonHint|Qt.MSWindowsFixedSizeDialogHint|Qt.WindowCloseButtonHint
        // icon: StandardIcon.Information
        onYesClicked: {
            //CrtWidget.logWidgetClose()
            emit:closeView()
            emit:startClose()
        }
    }

    Component.onCompleted: {
        userName.visible = false
        userNameTextField.visible = false

        userRightComboBox.currentIndex=2
    }

    function clearLoginInfo() {
        userNameTextField.clear()
        passwordTextField.clear()
        loginState = false
    }
    function getLoginState()
    {
        return loginState
    }

    function initToGeneralUser()
    {
        userRightComboBox.currentIndex=2

    }

    function retranslate()
    {
        userRightTxt.text = qsTr("用户权限")
        userRightComboBox.model =[qsTr("超级用户"), qsTr("管理员"), qsTr("普通用户")]
        userName.text = qsTr("用户名  ")
        userNameTextField.placeholderText =qsTr("用户名")
        password.text = qsTr("   密码  ")
        passwordTextField.placeholderText=qsTr("密码")
        loginBtn.text = qsTr("登录")
        criticalMessageDialog.title =qsTr("错误提示")
        criticalMessageDialog.text = qsTr("密码或者用户名错误，请重新输入......")
        infoMessageDialog.title = qsTr("信息提示")
        tipInfo()
        //infoMessageDialog.text = qsTr("密码输入正确!登录成功!")
    }

    function tipInfo()
    {
        infoMessageDialog.text = qsTr("密码输入正确!登录成功!")
        if (userRightComboBox.currentText != qsTr("超级用户"))
        {
            infoMessageDialog.text += "\r\n"+qsTr("使用软件关机，过8秒以后再开机，设备才能正常启动。")
        }

    }
}
