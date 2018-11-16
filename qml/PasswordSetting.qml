import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    GridLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        //columns: 2
        //Layout.spacing: 5
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

            //            horizontalAlignment: Text.AlignHCenter
            //            verticalAlignment: Text.AlignBottom
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
            id: newPasswordTextFile
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
        }
        //        TextField {
        //            id: newPasswordTextFile
        //            echoMode: TextInput.Password
        //            placeholderText: qsTr("新密码")
        //            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
        //                              | Qt.ImhSensitiveData | Qt.ImhNoPredictiveText
        //        }
    }
}
