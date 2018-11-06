import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0

Rectangle {
    width: 480
    height: 320
    GridLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 5
        Text {
            id: typeLabel
            text: qsTr("类型:")
            Layout.row: 0
            Layout.column: 0
        }
        ComboBox {
            id: typeComboBox
            Layout.row: 0
            Layout.column: 1
            Layout.fillWidth: true
        }

        Text {
            id: colorLabel
            text: qsTr("颜色:")
            Layout.row: 1
            Layout.column: 0
        }
        Rectangle {
            id: colorRec
            width: 150
            height: 40
            color: "red"
            Layout.row: 1
            Layout.column: 1
            Layout.fillWidth: true
        }

        Button {
            id: colorSelectBtn
            text: qsTr("选择颜色")
            Layout.row: 1
            Layout.column: 2

            onClicked: {
                colorDialog.open()
            }
        }

        Text {
            id: sizeLabel
            text: qsTr("大小:")
            Layout.row: 2
            Layout.column: 0
        }
        SpinBox {
            id: sizeSpinBox
            Layout.row: 2
            Layout.column: 1
            Layout.fillWidth: true
        }
        CheckBox {
            id: useIconCheckBox
            text: qsTr("使用图标")
            Layout.row: 3
            Layout.column: 0
            onCheckStateChanged: {
                iconAddr.clear()
                if (checkState == Qt.Checked) {
                    iconAddr.enabled = true
                    iconSelectBtn.enabled = true
                } else {
                    iconAddr.enabled = false
                    iconSelectBtn.enabled = false
                }
            }
        }
        TextField {
            id: iconAddr
            Layout.row: 3
            Layout.column: 1
            Layout.fillWidth: true
            enabled: false
        }

        Button {
            id: iconSelectBtn
            text: qsTr("选择图标")
            Layout.row: 3
            Layout.column: 2
            enabled: false
            onClicked: {
                fileDialog.open()
            }
        }

        RowLayout {
            Layout.row: 4
            Layout.column: 1
            spacing: 5
            Layout.topMargin: 10
            Button {
                id: saveBtn
                text: qsTr("保存")
            }

            Button {
                id: quitBtn
                text: qsTr("退出")
            }
        }
    }
    ColorDialog {
        id: colorDialog
        onAccepted: {

            colorRec.color = color
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)

        onAccepted: {
            console.log(currentFile.toString())
            iconAddr.text = currentFile.toString()
        }
    }
}
