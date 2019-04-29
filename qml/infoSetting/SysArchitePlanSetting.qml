import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import sysArchitePlanView 1.0
import qmlForJson 1.0
import operatorInfo 1.0

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
            height: 30
            verticalAlignment: Text.AlignVCenter
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
            height: 30
            verticalAlignment: Text.AlignVCenter
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
            height: 30
            verticalAlignment: Text.AlignVCenter
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
            height: 30
            verticalAlignment: Text.AlignVCenter
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
            height: 30
            verticalAlignment: Text.AlignVCenter
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
            height: 30
            verticalAlignment: Text.AlignVCenter
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
            height: 30
            verticalAlignment: Text.AlignVCenter
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
            height: 30
            verticalAlignment: Text.AlignVCenter
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
            height: 30
            verticalAlignment: Text.AlignVCenter
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
            height: 30
            verticalAlignment: Text.AlignVCenter
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
        nameFilters: "SVG Files(*.svg)"
        onAccepted: {
            OperatorInfo.insertEvent(qsTr(String("%1图纸变更").arg(txtName)),
                                     qsTr(String("%1变更为%2").arg(
                                              textFieldName.text).arg(
                                              currentFile.toString())))
            textFieldName.text = currentFile.toString()
            SysArchitePlanView.setSysDrawing(txtName, textFieldName.text)
        }
    }

    QmlForJson {
        id: qmlForJson
    }

    Component.onCompleted: {

        setSysArchitePlan()
    }

    function setSysArchitePlan() {
        var sysArchitePlanObj = new Object
        var jsonStr = new String
        jsonStr = qmlForJson.readFileToString()
        if (jsonStr.length <= 0)
            return
        sysArchitePlanObj = JSON.parse(jsonStr)
        var sysArchitePlanStr = new String
        sysArchitePlanStr = JSON.stringify(
                    sysArchitePlanObj["sysArchitePlan"].valueOf())
        var gasFireStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                        ))["气体灭火系统"]
        var waterSprayStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                           ))["水喷雾灭火系统"]
        var foamAndDryStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                           ))["泡沫和干粉灭火系统"]
        var fireCockStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                         ))["消火栓系统"]
        var fireEmergencyStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                              ))["消防应急照明系统"]
        var linkageStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                        ))["消防联动控制系统"]

        var autoFireAlarm = JSON.parse(sysArchitePlanStr.toLocaleString(
                                           ))["火灾自动报警系统"]
        var evacuationInsStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                              ))["疏散指示系统"]
        var autoFireExitStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                             ))["自动喷水灭火系统"]
        var smokeExhaustStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                             ))["防烟排烟系统"]
        //JSON.stringify()
        fireAlarmTextField.text = autoFireAlarm
        linkageTextField.text = linkageStr
        autoSprayWaterTextField.text = autoFireExitStr
        fireCockTextField.text = fireCockStr
        gasFireTextField.text = gasFireStr
        waterSprayTextField.text = waterSprayStr
        foamAndDryTextField.text = foamAndDryStr
        smokeControlTextField.text = smokeExhaustStr
        fireEmergencyLightingTextField.text = fireEmergencyStr
        evacuationTextField.text = evacuationInsStr
    }

    //    Component.onDestroyed: {

    //    }
}
