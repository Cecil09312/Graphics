import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.VirtualKeyboard 2.1
import QtQuick.Window 2.3
import Qt.labs.platform 1.0

Rectangle {
    id: treeViewSettingRec
    width: 480
    height: 240
    GridLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            id: primaryArchiteTxt
            text: qsTr("名称修改:")
            Layout.row: 0
            Layout.column: 0
        }
        TextField {
            id: primaryArchiteName
            Layout.row: 0
            Layout.column: 1
            // echoMode: TextInput.Password
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
                              | Qt.ImhSensitiveData | Qt.ImhNoPredictiveText
        }
        Button {
            id: primaryArchiteBtn
            Layout.row: 0
            Layout.column: 2
            text: qsTr("确认")
            onClicked: {
                TreeView.setItemName(primaryArchiteName.text)
            }
        }

        Text {
            id: primaryImageTxt
            text: qsTr("图片路径:")
            Layout.row: 1
            Layout.column: 0
        }

        TextField {
            id: primArchImageTextField
            Layout.row: 1
            Layout.column: 1
        }
        Button {
            id: primArchImageBtn
            Layout.row: 1
            Layout.column: 2
            text: qsTr("选择图片")
            onClicked: {
                fileDialog.open()
            }
        }

        RowLayout {
            Layout.row: 2
            Layout.column: 1
            Layout.topMargin: 10
            spacing: 5

            Button {
                id: saveBtn
                text: qsTr("保存")
            }

            Button {
                id: quitBtn
                text: qsTr("退出")
                onClicked: {
                    TreeView.architeSettingViewClose()
                }
            }
        }
    }
    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)

        onAccepted: {
            console.log(currentFile.toString())
            primArchImageTextField.text = currentFile.toString()
            TreeView.insertPixmap(primArchImageTextField.text)
        }
    }

    function clearTextField() {
        primaryArchiteName.clear()
        primArchImageTextField.clear()
        primaryArchiteName.focus = false
        primArchImageTextField.focus = false
    }
}
