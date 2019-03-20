import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0

Item {
    // id: operaEventItem
    anchors.bottomMargin: 40
    anchors.topMargin: 40
    Row {
        id: operaEventQuery
        spacing: 5
        anchors.topMargin: 20

        Text {
            id: equipmentNum
            text: qsTr("设备编码")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: equipmentNumTextField
            width: 100
            height: 30
        }

        Text {
            id: maintTime
            text: qsTr("维保时间")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: maintTimeTextField
            width: 100
            height: 30
        }

        Button {
            id: operaEventQueryBtn
            text: qsTr("查询")
            height: 30
            onClicked: {

            }
        }

        Button {
            id: operaEventQueryAllBtn
            text: qsTr("查询所有")
            height: 30
            onClicked: {

            }
        }

        Button {
            id: operaEventQueryClearBtn
            text: qsTr("清空")
            height: 30
            onClicked: {

            }
        }
    }

    Controls1_4.TableView {
        anchors.top: operaEventQuery.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 5

        Controls1_4.TableViewColumn {
            role: "equipmentCode"
            title: qsTr("设备编码")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "maintTime"
            title: qsTr("维保时间")
            width: 120
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "maintInstructions"
            title: qsTr("维保说明")
            width: 120
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "state"
            title: qsTr("状态现象")
            width: 120
            // resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "solution"
            title: qsTr("解决方法")
            width: 120
            // resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "contentDesc"
            title: qsTr("内容描述")
            width: 120
            // resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "maintEngineer"
            title: qsTr("维保员")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "floor"
            title: qsTr("楼层")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "position"
            title: qsTr("部位")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "system"
            title: qsTr("系统")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "buildingName"
            title: qsTr("建筑名称")
            width: 60
        }

        Controls1_4.TableViewColumn {
            role: "personOnDuty"
            title: qsTr("值班人员")
            width: 60
        }

        Controls1_4.TableViewColumn {
            role: "operator"
            title: qsTr("操作人员")
            width: 60
        }
        model: operaEventQueryModel

        QmlTableModel {
            id: operaEventQueryModel
            roleNameList: ["equipmentCode", "maintTime", "maintInstructions", "state", "solution", "contentDesc", "maintEngineer", "floor", "position", "system", "buildingName", "personOnDuty", "operator"]
            titleList: [qsTr("设备编码"), qsTr("维保时间"), qsTr(
                    "维保说明"), qsTr("状态现象"), qsTr("解决方法"), qsTr("内容描述"), qsTr(
                    "维保员"), qsTr("楼层"), qsTr("部位"), qsTr("系统"), qsTr(
                    "建筑名称"), qsTr("值班人员"), qsTr("操作人员")]
        }
    }

    function selectInfo() {
        var info = new String
        if (productNumTextField.text.length > 0) {
            info += (qsTr("产品编号=") + productNumTextField.text)
        }

        if (dateTextField.text.length > 0) {
            if (info.length > 0) {
                info += ","
            }
            info += (qsTr("维保日期=") + dateTextField.text)
        }
        return info
    }

    function saveToPdf() {
        operaEventQueryModel.saveToPdf()
    }
    function startPrint() {
        operaEventQueryModel.startPrint()
    }
    function printPreview() {
        operaEventQueryModel.printPreview()
    }
}
