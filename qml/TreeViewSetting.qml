import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.VirtualKeyboard 2.1
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.2
Rectangle
{
    id:treeViewSettingRec
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
            onClicked:
            {
                TreeView.setItemName(primaryArchiteName.text);
            }
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
            onClicked:
            {

                fileDialog.open();
            }
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
            onClicked:
            {
              TreeView.architeSettingViewClose();
            }
        }


    }
    FileDialog
    {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        //folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted:
        {
            console.log(fileDialog.fileUrl.toString());

           primArchImageTextField.text = fileDialog.fileUrl.toString();
            TreeView.insertPixmap(primArchImageTextField.text);
        }
    }

    function clearTextField()
    {
        primaryArchiteName.clear();
        primArchImageTextField.clear();
        primaryArchiteName.focus=false;
        primArchImageTextField.focus = false;
    }
}
