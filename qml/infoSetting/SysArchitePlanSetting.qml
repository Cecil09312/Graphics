import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import sysArchitePlanView 1.0
import qmlForJson 1.0
import operatorInfo 1.0

Item {
    width: 800
    height: 640
    property var textFieldName: fireAlarmTxt
    property string txtName: ""
    property url filePath: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
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
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
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
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
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
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
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
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
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
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
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
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
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
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
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
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
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
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
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
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
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
        folder: filePath
        flags: Qt.WindowStaysOnTopHint|Qt.MSWindowsFixedSizeDialogHint|Qt.WindowCloseButtonHint
        //folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        nameFilters: "SVG Files(*.svg)"
        onAccepted: {
            OperatorInfo.insertEvent(qsTr(String("%1图纸变更").arg(txtName)),
                                     qsTr(String("%1变更为%2").arg(
                                              textFieldName.text).arg(
                                              currentFile.toString())))
            textFieldName.text = currentFile.toString()
            filePath = file
            SysArchitePlanView.setSysDrawing(txtName, textFieldName.text)
        }
    }

    QmlForJson {
        id: qmlForJson
    }

    Component.onCompleted: {

       // setSysArchitePlan()

    }

    function setSysArchitePlan() {
        var sysArchitePlanObj = new Object
        var jsonStr = new String
        jsonStr = qmlForJson.readFileToString()
        if (jsonStr.length <= 0)
        {
            return
        }
        sysArchitePlanObj = JSON.parse(jsonStr)



        var sysArchitePlanStr = new String


        if(typeof sysArchitePlanObj["sysArchitePlan"]=="undefined")
        {
            return
        }
        sysArchitePlanStr = JSON.stringify(
                    sysArchitePlanObj["sysArchitePlan"].valueOf())
        var gasFireStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                        ))[qsTr("气体灭火系统")]
        var waterSprayStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                           ))[qsTr("水喷雾灭火系统")]
        var foamAndDryStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                           ))[qsTr("泡沫和干粉灭火系统")]
        var fireCockStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                         ))[qsTr("消火栓系统")]
        var fireEmergencyStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                              ))[qsTr("消防应急照明系统")]
        var linkageStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                        ))[qsTr("消防联动控制系统")]

        var autoFireAlarm = JSON.parse(sysArchitePlanStr.toLocaleString(
                                           ))[qsTr("火灾自动报警系统")]
        var evacuationInsStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                              ))[qsTr("疏散指示系统")]
        var autoFireExitStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                             ))[qsTr("自动喷水灭火系统")]
        var smokeExhaustStr = JSON.parse(sysArchitePlanStr.toLocaleString(
                                             ))[qsTr("防烟排烟系统")]
        //JSON.stringify()
        if(typeof autoFireAlarm=="string")
        fireAlarmTextField.text = autoFireAlarm
        if(typeof linkageStr=="string")
        linkageTextField.text = linkageStr
        if(typeof autoFireExitStr=="string")
        autoSprayWaterTextField.text = autoFireExitStr
        if(typeof fireCockStr=="string")
        fireCockTextField.text = fireCockStr
        if(typeof gasFireStr=="string")
        gasFireTextField.text = gasFireStr
        if(typeof waterSprayStr=="string")
        waterSprayTextField.text = waterSprayStr
        if(typeof foamAndDryStr=="string")
        foamAndDryTextField.text = foamAndDryStr
        if(typeof smokeExhaustStr=="string")
        smokeControlTextField.text = smokeExhaustStr
        if(typeof fireEmergencyStr=="string")
        fireEmergencyLightingTextField.text = fireEmergencyStr
        if(typeof evacuationInsStr=="string")
        evacuationTextField.text = evacuationInsStr
    }

    //    Component.onDestroyed: {

    //    }

    function retranslate()
    {
       fireAlarmTxt.text = qsTr("火灾自动报警系统")
        fireAlarmBtn.text = qsTr("选择图纸")
        linkageTxt.text = qsTr("消防联动控制系统")
        linkageBtn.text = qsTr("选择图纸")
        autoSprayWaterTxt.text = qsTr("自动喷水灭火系统")
        autoSprayWaterBtn.text = qsTr("选择图纸")
        fireCockTxt.text = qsTr("消火栓系统")
        fireCockBtn.text = qsTr("选择图纸")
        gasFireTxt.text = qsTr("气体灭火系统")
        gasFireBtn.text = qsTr("选择图纸")
        waterSprayTxt.text =qsTr("水喷雾灭火系统")
        waterSprayBtn.text =qsTr("选择图纸")
        foamAndDryTxt.text = qsTr("泡沫和干粉灭火系统")
        foamAndDryBtn.text = qsTr("选择图纸")
        smokeControlTxt.text = qsTr("防烟排烟系统")
        smokeControlBtn.text = qsTr("选择图纸")
        fireEmergencyLightingTxt.text = qsTr("消防应急照明系统")
        fireEmergencyLightingBtn.text = qsTr("选择图纸")
        evacuationTxt.text = qsTr("疏散指示系统")
        evacuationBtn.text = qsTr("选择图纸")


    }
}
