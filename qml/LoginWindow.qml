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
          model: ["超级用户","工程人员","员工"]
          Layout.row: 0
          Layout.column: 1
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

//   VirtualKeyboardSettings
//   {
//   }
//   Binding {
//       target: VirtualKeyboardSettings
//       property: "fullScreenMode"
//       value: (Screen.width / Screen.height) > (60.0 / 9.0)
//   }

//   InputPanel {
//       id: inputPanel
//       z: 99
//       //更改x,y即可更改键盘位置
//       x: 0
//       y: parent.height
//       //更改width即可更改键盘大小
//       width:loginWindow.width

//       states: State {
//           name: "visible"
//           when: inputPanel.active
//           PropertyChanges {
//               target: inputPanel
//               y: parent.height - inputPanel.height
//           }
//       }
//       transitions: Transition {
//           from: ""
//           to: "visible"
//           reversible: true
//           ParallelAnimation {
//               NumberAnimation {
//                   properties: "y"
//                   duration: 250
//                   easing.type: Easing.InOutQuad
//               }
//           }
//       }
//   }

}
