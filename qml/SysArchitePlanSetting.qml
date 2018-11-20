import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import sysArchitePlanView 1.0

Item {
    property var textFieldName: fireAlarmTxt
    property string txtName: ""
    Grid {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        columns: 3
        columnSpacing: 5
        rowSpacing: 5
        Text {
            id: fireAlarmTxt
            text: qsTr("火灾自动报警系统")
        }

        TextField {
            id: fireAlarmTextField
        }
        Button {
            id: fireAlarmBtn
            text: qsTr("选择图纸")
            onClicked: {
                textFieldName = fireAlarmTextField
                txtName = fireAlarmTxt.text
                chooseFileDialog.open()
            }
        }

        Text {
            id: linkageTxt
            text: qsTr("消防联动控制系统")
        }

        TextField {
            id: linkageTextField
        }
        Button {
            id: linkageBtn
            text: qsTr("选择图纸")
            onClicked: {
                textFieldName = linkageTextField
                txtName = linkageTxt.text
                chooseFileDialog.open()
            }
        }

        Text {
            id: autoSprayWaterTxt
            text: qsTr("自动喷水灭火系统")
        }

        TextField {
            id: autoSprayWaterTextField
        }
        Button {
            id: autoSprayWaterBtn
            text: qsTr("选择图纸")
            onClicked: {
                textFieldName = autoSprayWaterTextField
                txtName = autoSprayWaterTxt.text
                chooseFileDialog.open()
            }
        }

        Text {
            id: fireCockTxt
            text: qsTr("消火栓系统")
        }

        TextField {
            id: fireCockTextField
        }
        Button {
            id: fireCockBtn
            text: qsTr("选择图纸")
            onClicked: {
                textFieldName = fireCockTextField
                txtName = fireCockTxt.text
                chooseFileDialog.open()
            }
        }

        Text {
            id: gasFireTxt
            text: qsTr("气体灭火系统")
        }

        TextField {
            id: gasFireTextField
        }
        Button {
            id: gasFireBtn
            text: qsTr("选择图纸")
            onClicked: {
                textFieldName = gasFireTextField
                txtName = gasFireTxt.text
                chooseFileDialog.open()
            }
        }

        Text {
            id: waterSprayTxt
            text: qsTr("水喷雾灭火系统")
        }

        TextField {
            id: waterSprayTextField
        }
        Button {
            id: waterSprayBtn
            text: qsTr("选择图纸")
            onClicked: {
                textFieldName = waterSprayTextField
                txtName = waterSprayTxt.text
                chooseFileDialog.open()
            }
        }

        Text {
            id: foamAndDryTxt
            text: qsTr("泡沫和干粉灭火系统")
        }

        TextField {
            id: foamAndDryTextField
        }
        Button {
            id: foamAndDryBtn
            text: qsTr("选择图纸")
            onClicked: {
                textFieldName = foamAndDryTextField
                txtName = foamAndDryTxt.text
                chooseFileDialog.open()
            }
        }

        Text {
            id: smokeControlTxt
            text: qsTr("防烟排烟系统")
        }

        TextField {
            id: smokeControlTextField
        }
        Button {
            id: smokeControlBtn
            text: qsTr("选择图纸")
            onClicked: {
                textFieldName = smokeControlTextField
                txtName = smokeControlTxt.text
                chooseFileDialog.open()
            }
        }

        Text {
            id: fireEmergencyLightingTxt
            text: qsTr("消防应急照明系统")
        }

        TextField {
            id: fireEmergencyLightingTextField
        }
        Button {
            id: fireEmergencyLightingBtn
            text: qsTr("选择图纸")
            onClicked: {
                textFieldName = fireEmergencyLightingTextField
                txtName = fireEmergencyLightingTxt.text
                chooseFileDialog.open()
            }
        }

        Text {
            id: evacuationTxt
            text: qsTr("疏散指示系统")
        }

        TextField {
            id: evacuationTextField
        }
        Button {
            id: evacuationBtn
            text: qsTr("选择图纸")
            onClicked: {
                textFieldName = evacuationTextField
                txtName = evacuationTxt.text
                chooseFileDialog.open()
            }
        }
    }

    FileDialog {
        id: chooseFileDialog
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: {
            textFieldName.text = currentFile.toString()
            SysArchitePlanView.setSysDrawing(txtName, textFieldName.text)
        }
    }
}
