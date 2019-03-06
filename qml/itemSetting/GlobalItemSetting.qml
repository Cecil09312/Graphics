import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import Qt.labs.platform 1.0

Rectangle {

    width: 480
    height: 360
    signal setItemValue(real value)

    GridLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            text: qsTr("建筑物名称")
            Layout.row: 0
            Layout.column: 0
        }

        TextField {
            id: buildNameTextField
            Layout.row: 0
            Layout.column: 1
        }

        Button {
            id: buildNameSettingBtn
            Layout.row: 0
            Layout.column: 2
            text: qsTr("设置")
            onClicked: {
                GlobalItemSettingView.setCurrentBuildName(
                            buildNameTextField.text)
            }
        }

        Text {
            text: qsTr("图标")
            Layout.row: 1
            Layout.column: 0
        }

        TextField {
            id: iconTextField
            Layout.row: 1
            Layout.column: 1
        }

        Button {
            id: iconSelectBtn
            Layout.row: 1
            Layout.column: 2
            text: qsTr("选择图标")
            onClicked: {
                fileDialog.open()
            }
        }

        Text {

            text: qsTr("大小")
            Layout.row: 2
            Layout.column: 0
        }
        SpinBox {
            id: spinBox
            Layout.row: 2
            Layout.column: 1

            Layout.fillWidth: true
            from: 1
            to: 100

            property int decimals: 1
            property real realValue: value

            validator: DoubleValidator {
                bottom: Math.min(spinBox.from, spinBox.to)
                top: Math.max(spinBox.from, spinBox.to)
            }

            textFromValue: function (value, locale) {
                return Number(value).toLocaleString(locale, 'f',
                                                    spinBox.decimals)
            }

            valueFromText: function (text, locale) {
                return Number.fromLocaleString(locale, text)
            }
            onValueChanged: {
                emit: setItemValue(value)
                //GlobalItemSettingView.setCurrentItemSize(value / 10.0)
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: {
            GlobalItemSettingView.setCurrentItemIcon(currentFile)
            iconTextField.text = currentFile
        }
    }

    function setBuileName(name) {
        buildNameTextField.text = name
    }

    function setGlobalItemValue(value) {
        spinBox.value = value
    }

    function setGlobalIcon(name) {

        iconTextField.text = name
    }
    //    Component.onCompleted: {
    //        buildNameTextField.text = GlobalItemSettingView.currentBuildName()
    //        spinBox.value = GlobalItemSettingView.currentItemSize()
    //        iconTextField.text = GlobalItemSettingView.currentItemIcon()
    //    }
}
