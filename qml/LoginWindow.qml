import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtQuick.VirtualKeyboard 2.1
import QtQuick.VirtualKeyboard.Settings 2.2

Rectangle
{
    id:loginWindow;
    width: 480;
    height:240;
    GridLayout
    {
        id:gridLayout;
        anchors.verticalCenter: parent.verticalCenter;
        anchors.horizontalCenter: parent.horizontalCenter
        columnSpacing: 5
        Text
        {
            id: userName
            text: qsTr("用户名")
            Layout.row: 0
            Layout.column: 0
        }
        ComboBox
        {
            model: ["超级用户","工程人员","员工"];
            Layout.row: 0;
            Layout.column: 1;
        }
        Text
        {

            id: password
            text: qsTr("密码")
            Layout.row: 1
            Layout.column: 0
        }
        TextField
        {
            id:passwordTextField;
            Layout.row: 1
            Layout.column: 1
            echoMode: TextInput.Password
            placeholderText: "Password field"
            inputMethodHints: Qt.ImhNoAutoUppercase|Qt.ImhPreferLowercase|
                              Qt.ImhSensitiveData|Qt.ImhNoPredictiveText

        }

        Button
        {

            text: qsTr("登陆")
            Layout.row: 2;
            Layout.column: 0;
        }

        Button
        {
            text: qsTr("取消")
            Layout.row: 2;
            Layout.column: 1;
            onClicked:
            {
                //loginWindow.visible= false;

            }
        }


    }

}
