import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0

Item {
    property string fileType: ""
    Grid {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        columns: 3
        spacing: 5
        Text {
            text: qsTr("消防控制室的管理机构")
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: managInstitutionsTextField
        }
        Button {
            id: managInstitutionsBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "managInstitutions"
                fileDialog.open()
            }
        }

        Text {
            text: qsTr("系统竣工图纸")
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: sysAsbuiltDrawingsTextField
        }
        Button {
            id: sysAsbuiltDrawingsBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "sysAsbuiltDrawings"
                fileDialog.open()
            }
        }

        Text {
            text: qsTr("各分系统控制逻辑关系说明")
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: logicalThatTextField
        }
        Button {
            id: logicalThatBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "logicalThat"
                fileDialog.open()
            }
        }

        Text {
            text: qsTr("设备使用说明书")
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: equipmentInstTextField
        }
        Button {
            id: equipmentInstBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "equipmentInst"
                fileDialog.open()
            }
        }

        Text {
            text: qsTr("系统操作规程")
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: sysProceduresTextField
        }
        Button {
            id: sysProceduresBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "sysProcedures"
                fileDialog.open()
            }
        }

        Text {
            text: qsTr("应急预案")
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: emergencyPlanTextField
        }
        Button {
            id: emergencyPlanBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "emergencyPlan"
                fileDialog.open()
            }
        }

        Text {
            text: qsTr("值班制度")
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: onDutySysTextField
        }
        Button {
            id: onDutySysBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "onDutySys"
                fileDialog.open()
            }
        }

        Text {
            text: qsTr("维护保养制度及记录")
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: maintainSysTextField
        }
        Button {
            id: maintainSysBtn
            text: qsTr("选择文件")
            onClicked: {
                fileType = "maintainSys"
                fileDialog.open()
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: {
            if (fileType == "managInstitutions") {
                managInstitutionsTextField.text = currentFile
            } else if (fileType == "sysAsbuiltDrawings") {
                sysAsbuiltDrawingsTextField.text = currentFile
            } else if (fileType == "logicalThat") {
                logicalThatTextField.text = currentFile
            } else if (fileType == "equipmentInst") {
                equipmentInstTextField.text = currentFile
            } else if (fileType == "sysProcedures") {
                sysProceduresTextField.text = currentFile
            } else if (fileType == "emergencyPlan") {
                emergencyPlanTextField.text = currentFile
            } else if (fileType == "onDutySys") {
                onDutySysTextField.text = currentFile
            } else if (fileType == "maintainSys") {
                maintainSysTextField.text = currentFile
            }
        }
    }
}
