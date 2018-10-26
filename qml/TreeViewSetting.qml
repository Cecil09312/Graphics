import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.VirtualKeyboard 2.1
import QtQuick.Window 2.3
Rectangle
{
   width: 480;
   height:240;
   GridLayout
   {
       anchors.verticalCenter: parent.verticalCenter;
       anchors.horizontalCenter: parent.horizontalCenter;

       Text
       {
           id: primaryArchiteTxt
           text: qsTr("名称修改:")
           Layout.row: 0
           Layout.column: 0
       }
       TextField
       {
        id:primaryArchiteName
        Layout.row: 0
        Layout.column: 1
       // echoMode: TextInput.Password
        inputMethodHints: Qt.ImhNoAutoUppercase|Qt.ImhPreferLowercase|
                          Qt.ImhSensitiveData|Qt.ImhNoPredictiveText
       }
       Button
       {
           id:primaryArchiteBtn
           Layout.row: 0
           Layout.column: 2
           text: qsTr("确认")
       }


       Text
       {
           id: primaryImageTxt
           text: qsTr("图片路径:")
           Layout.row: 1
           Layout.column: 0
       }

       TextField
       {
        id:primArchImageTextField
        Layout.row: 1
        Layout.column: 1
       }
       Button
       {
           id:primArchImageBtn
           Layout.row: 1
           Layout.column: 2
           text: qsTr("选择图片")
       }

       Button
       {
           id:saveBtn
           Layout.row: 2
           Layout.column: 1
           text: qsTr("保存")
       }

       Button
       {
           id:quitBtn
           Layout.row: 2
           Layout.column: 2
           text: qsTr("退出")
       }


   }
//   InputPanel {
//       id: inputPanel
//       z: 99
//       //更改x,y即可更改键盘位置
//       x: 0
//       y: parent.height
//       //更改width即可更改键盘大小
//       width:parent.width

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
