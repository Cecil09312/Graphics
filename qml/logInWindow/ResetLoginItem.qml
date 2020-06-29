import QtQuick 2.0
import QtQuick.Controls 2.2
import "../infoSetting"
Rectangle
{
    id:root
    width: 320
    height: 180
    signal curLoginState(bool loginState)
    Column
    {
        id:column
        spacing: 10
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        Row
        {
            spacing: 10

            Text {
                id:passwordTxt
                text: qsTr("密码")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                height: 30

            }
            TextField
            {
                id:passwordTextField
                echoMode:TextInput.Password
                width: 150
                selectByMouse: true
                selectionColor: "blue"
                selectedTextColor: "white"

            }
        }

        Row
        {
            id:btnRow
            spacing: 10
            NaviButton
            {
                id:confirmBtn
                width: 80
                text: qsTr("确认")
                onClicked:
                {
                    if(passwordTextField.text=="123123")
                    {
                        loginState.text=qsTr("密码正确")
                        loginState.color = "green"
                        emit:curLoginState(true)
                    }
                    else
                    {
                        loginState.text=qsTr("密码错误")
                        loginState.color = "red"
                       // emit:curLoginState(false)
                    }
                }
            }
            NaviButton
            {
                id:cancelBtn
                width: 80
                text: qsTr("退出")
                onClicked:
                {
                   emit:curLoginState(false)
                }
            }
        }

        Text {
            id: loginState
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            width: 150
            font.family: "Times New Roman"
            font.pointSize: 12
        }
    }

    Component.onCompleted:
    {
        clearLoginInfo()
    }

    function setLoginState(state)
    {
       loginState.text = state
    }
    function setLoginTextColor(color)
    {
       loginState.color = color
    }

    function clearLoginInfo()
    {
       loginState.text = ""
       passwordTextField.clear()
    }

    function retranslate()
    {
       passwordTxt.text = qsTr("密码")
       confirmBtn.text = qsTr("确认")
       cancelBtn.text = qsTr("退出")
    }
}
