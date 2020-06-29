import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import Qt.labs.platform 1.0
import operatorInfo 1.0
import controller 1.0
//import QtQuick.Dialogs 1.2 as Dialog1_2
import "../infoSetting"
Rectangle {

    width: 420
    height: 320
    signal setItemValue(real value)
    property string buildName: ""
    property string personOnDuty: ""
    property string iconPath: ""
    property url filePath: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
    signal updateBuildingName(string oldName,string newName)

    GridLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            id:buildingNameTxt
            text: qsTr("建筑名称")
            Layout.row: 0
            Layout.column: 0
        }

        TextField {
            id: buildNameTextField
            Layout.row: 0
            Layout.column: 1
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }

        NaviButton {
            id: buildNameSettingBtn
            Layout.row: 0
            Layout.column: 2
            text: qsTr("设置")
            onClicked: {

                if(GlobalItemSettingView.buildNameIsExist(buildNameTextField.text))
                {
                    messageDialog.open()
                   return
                }
                GlobalItemSettingView.setCurrentBuildName(
                            buildNameTextField.text)

                OperatorInfo.insertEvent(qsTr("更改建筑物名称"),
                                         String(qsTr("建筑物名称由\"%1\"改为\"%2\"")).arg(
                                             buildName).arg(
                                             buildNameTextField.text))

               // emit:updateItemInfo(buildName,buildNameTextField.text)
                buildName = buildNameTextField.text
            }
        }

        Text {
            id:personOndutyTxt
            text: qsTr("值班人员")
            Layout.row: 1
            Layout.column: 0
        }

        TextField {
            id: personOnDutyTextField
            Layout.row: 1
            Layout.column: 1
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }

        NaviButton {
            id: personOnDutySettingBtn
            Layout.row: 1
            Layout.column: 2
            text: qsTr("设置")
            onClicked: {

                GlobalItemSettingView.setPersonOnDuty(
                            personOnDutyTextField.text)
                OperatorInfo.insertEvent(qsTr("更改值班人员"),
                                         String(qsTr("值班人员由\"%1\"更改为\"%2\"")).arg(
                                             personOnDuty).arg(
                                             personOnDutyTextField.text))
                personOnDuty = personOnDutyTextField.text
               // emit:updateItemInfo(buildName,personOnDuty)
            }
        }

        Text {
            id:iconTxt
            text: qsTr("图标")
            Layout.row: 2
            Layout.column: 0
        }

        TextField {
            id: iconTextField
            Layout.row: 2
            Layout.column: 1
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }

        NaviButton {
            id: iconSelectBtn
            Layout.row: 2
            Layout.column: 2
            text: qsTr("选择图标")
            onClicked: {
                fileDialog.open()
            }
        }

        Text {

            id:sizeTxt
            text: qsTr("大小")
            Layout.row: 3
            Layout.column: 0
        }
        SpinBox {
            id: spinBox
            Layout.row: 3
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
            }
        }
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("错误提示")
        text: qsTr("建筑物已经存在，请重新修改")
        //standardButtons: StandardButton.Yes
        flags:Qt.WindowStaysOnTopHint|Qt.WindowMaximizeButtonHint|Qt.MSWindowsFixedSizeDialogHint|Qt.WindowCloseButtonHint
    }
    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: filePath
        flags:Qt.WindowStaysOnTopHint|Qt.WindowMaximizeButtonHint|Qt.MSWindowsFixedSizeDialogHint|Qt.WindowCloseButtonHint

        onAccepted: {

            if(GlobalItemSettingView.buildNameIsExist(Controller.getFileNameFromUrl(currentFile)))
            {
                messageDialog.open()
               return
            }
            filePath = file
            GlobalItemSettingView.setCurrentItemIcon(currentFile)
            iconTextField.text = currentFile
            OperatorInfo.insertEvent(qsTr("更改建筑物图标"),
                                     String(qsTr("建筑物图标由\"%1\"更改为\"%2\"")).arg(
                                         iconPath).arg(iconTextField.text))
            iconPath = iconTextField.text

            GlobalItemSettingView.setCurrentBuildName(Controller.getFileNameFromUrl(currentFile))
            buildNameTextField.text = Controller.getFileNameFromUrl(currentFile)

        }
    }

    function setBuileName(name) {
        buildNameTextField.text = name
        buildName = name
    }

    function setGlobalItemValue(value) {
        spinBox.value = value
    }

    function setGlobalIcon(name) {

        iconTextField.text = name
        iconPath = name
    }

    function setPersonOnDuty(person) {
        personOnDutyTextField.text = person
        personOnDuty = person
    }

    function retranslate()
    {
      buildingNameTxt.text = qsTr("建筑名称")
        buildNameSettingBtn.text =qsTr("设置")
        personOndutyTxt.text = qsTr("值班人员")
        personOnDutySettingBtn.text = qsTr("设置")
        iconTxt.text = qsTr("图标")
        iconSelectBtn.text = qsTr("选择图标")
        sizeTxt.text = qsTr("大小")
        messageDialog.title=qsTr("错误提示")
        messageDialog.text=qsTr("建筑物已经存在，请重新修改")
    }
}
