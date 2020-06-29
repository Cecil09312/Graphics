import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import transportInfo 1.0

Item {
    width: 800
    height: 640
    property string fileType: ""
    property url filePath: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
    Grid {
        id:grid
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        columns: 3
        spacing: 5
        Text {
            id:managementAgencyTxt
            text: qsTr("消防控制室的管理机构")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }
        TextField {
            id: managInstitutionsTextField
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
            id: managInstitutionsBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "managInstitutions"
                fileDialog.open()
            }
        }

        Text {
            id:completionDragram
            text: qsTr("系统竣工图纸")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }
        TextField {
            id: sysAsbuiltDrawingsTextField
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
            id: sysAsbuiltDrawingsBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "sysAsbuiltDrawings"
                fileDialog.open()
            }
        }

        Text {
            id:explanationTxt
            text: qsTr("各分系统控制逻辑关系说明")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }
        TextField {
            id: logicalThatTextField
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
            id: logicalThatBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "logicalThat"
                fileDialog.open()
            }
        }

        Text {
            id:equipmentInstTxt
            text: qsTr("设备使用说明书")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }
        TextField {
            id: equipmentInstTextField
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
            id: equipmentInstBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "equipmentInst"
                fileDialog.open()
            }
        }

        Text {
            id:sysProceduresTxt
            text: qsTr("系统操作规程")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }
        TextField {
            id: sysProceduresTextField
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
            id: sysProceduresBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "sysProcedures"
                fileDialog.open()
            }
        }

        Text {
            id:emergencyPlanTxt
            text: qsTr("应急预案")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }
        TextField {
            id: emergencyPlanTextField
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
            id: emergencyPlanBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "emergencyPlan"
                fileDialog.open()
            }
        }

        Text {
            id:onDutySysTxt
            text: qsTr("值班制度")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }
        TextField {
            id: onDutySysTextField
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        NaviButton {
            id: onDutySysBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "onDutySys"
                fileDialog.open()
            }
        }

        Text {
            id:maintainSysTxt
            text: qsTr("维护保养制度")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }
        TextField {
            id: maintainSysTextField
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }


        NaviButton {
            id: maintainSysBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "maintainSys"
                fileDialog.open()
            }
        }

        Text {
            id:maintainRecordTxt
            text: qsTr("维护保养记录")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }
        TextField {
            id: maintainRecordTextField
            readOnly: true
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }

        NaviButton {
            id: maintainRecordBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "maintainRecord"
                fileDialog.open()
            }
        }
    }



    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: filePath
        flags: Qt.WindowStaysOnTopHint|Qt.MSWindowsFixedSizeDialogHint|Qt.WindowCloseButtonHint
        //folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: {

            filePath = file
            if (fileType == "managInstitutions") {
                managInstitutionsTextField.text = currentFile
                TransportInfo.setTransportInfo(qsTr("消防控制室的管理机构"), currentFile)
            } else if (fileType == "sysAsbuiltDrawings") {
                sysAsbuiltDrawingsTextField.text = currentFile
                TransportInfo.setTransportInfo(qsTr("系统竣工图纸"), currentFile)
            } else if (fileType == "logicalThat") {
                logicalThatTextField.text = currentFile
                TransportInfo.setTransportInfo(qsTr("各分系统控制逻辑关系说明"), currentFile)
            } else if (fileType == "equipmentInst") {
                equipmentInstTextField.text = currentFile
                TransportInfo.setTransportInfo(qsTr("设备使用说明书"), currentFile)
            } else if (fileType == "sysProcedures") {
                sysProceduresTextField.text = currentFile
                TransportInfo.setTransportInfo(qsTr("系统操作规程"), currentFile)
            } else if (fileType == "emergencyPlan") {
                emergencyPlanTextField.text = currentFile
                TransportInfo.setTransportInfo(qsTr("应急预案"), currentFile)
            } else if (fileType == "onDutySys") {
                onDutySysTextField.text = currentFile
                TransportInfo.setTransportInfo(qsTr("值班制度"), currentFile)
            } else if (fileType == "maintainSys") {
                maintainSysTextField.text = currentFile
                TransportInfo.setTransportInfo(qsTr("维护保养制度"), currentFile)
            }
            else if (fileType == "maintainRecord") {
                maintainRecordTextField.text = currentFile
                TransportInfo.setTransportInfo(qsTr("维护保养记录"), currentFile)
            }
        }
    }

    Component.onCompleted: {
        managInstitutionsTextField.text = TransportInfo.transportInfo(
                    qsTr("消防控制室的管理机构"))
        sysAsbuiltDrawingsTextField.text = TransportInfo.transportInfo(
                    qsTr("系统竣工图纸"))
        logicalThatTextField.text = TransportInfo.transportInfo(
                    qsTr("各分系统控制逻辑关系说明"))
        equipmentInstTextField.text = TransportInfo.transportInfo(
                    qsTr("设备使用说明书"))
        sysProceduresTextField.text = TransportInfo.transportInfo(
                    qsTr("系统操作规程"))
        emergencyPlanTextField.text = TransportInfo.transportInfo(
                    qsTr("应急预案"))
        onDutySysTextField.text = TransportInfo.transportInfo(qsTr("值班制度"))
        maintainSysTextField.text = TransportInfo.transportInfo(
                    qsTr("维护保养制度"))
        maintainRecordTextField.text = TransportInfo.transportInfo(
                    qsTr("维护保养记录"))
        TransportInfo.setTransportInfo(qsTr("消防控制室的管理机构"),
                                       managInstitutionsTextField.text)
        TransportInfo.setTransportInfo(qsTr("系统竣工图纸"),
                                       sysAsbuiltDrawingsTextField.text)
        TransportInfo.setTransportInfo(qsTr("各分系统控制逻辑关系说明"),
                                       logicalThatTextField.text)
        TransportInfo.setTransportInfo(qsTr("设备使用说明书"), equipmentInstTextField.text)

        TransportInfo.setTransportInfo(qsTr("系统操作规程"), sysProceduresTextField.text)
        TransportInfo.setTransportInfo(qsTr("应急预案"), emergencyPlanTextField.text)
        TransportInfo.setTransportInfo(qsTr("值班制度"), onDutySysTextField.text)
        TransportInfo.setTransportInfo(qsTr("维护保养制度"), maintainSysTextField.text)
        TransportInfo.setTransportInfo(qsTr("维护保养记录"), maintainRecordTextField.text)
    }

    function retranslate()
    {
      managementAgencyTxt.text = qsTr("消防控制室的管理机构")
        managInstitutionsBtn.text = qsTr("选择文件")
        completionDragram.text =  qsTr("系统竣工图纸")
        sysAsbuiltDrawingsBtn.text = qsTr("选择文件")
        explanationTxt.text = qsTr("各分系统控制逻辑关系说明")
        logicalThatBtn.text = qsTr("选择文件")
        equipmentInstTxt.text = qsTr("设备使用说明书")
        equipmentInstBtn.text =  qsTr("选择文件")
        sysProceduresTxt.text = qsTr("系统操作规程")
        sysProceduresBtn.text = qsTr("选择文件")
        emergencyPlanTxt.text = qsTr("应急预案")
        emergencyPlanBtn.text = qsTr("选择文件")
        onDutySysTxt.text = qsTr("值班制度")
        onDutySysBtn.text = qsTr("选择文件")
        maintainSysTxt.text = qsTr("维护保养制度")
        maintainSysBtn.text = qsTr("选择文件")
        maintainRecordTxt.text = qsTr("维护保养记录")
        maintainRecordBtn.text = qsTr("选择文件")


    }
}
