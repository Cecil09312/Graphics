import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import operatorInfo 1.0
import "../infoSetting"
Item {

    width: 800
    height: 640
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    Column
    {
        anchors.fill: parent
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
            width: 80
            height: 30
        }

        Text {
            text: qsTr("维保员")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: maintEngineerTextField
            width: 80
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
            id: maintStartTimeTextField
            width: 150
            height: 30
            placeholderText: qsTr("如:2019/01/01 00:00:00")
        }

        Text {
            text: qsTr("到")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: maintEndTimeTextField
            width: 150
            height: 30
            placeholderText: qsTr("如:2050/01/01 00:00:00")
        }

        NaviButton {
            id: operaEventQueryBtn
            text: qsTr("查询")
            height: 30
            width: 80
            onClicked: {
                maintInfoQueryModel.sqlCommit(
                            String("select * from maintenance where %1").arg(
                                selectInfo()))
            }
        }

        NaviButton {
            id: operaEventQueryAllBtn
            text: qsTr("查询所有")
            height: 30
            width: 100
            onClicked: {
                maintInfoQueryModel.sqlCommit("select * from maintenance")
            }
        }


        NaviButton {
            id: operaEventQueryDeleteBtn
            text: qsTr("删除")
            height: 30
            width: 80
            onClicked: {
                var info = new String
                info = selectInfo()
                if(info.length>0)
                {
                    maintInfoQueryModel.sqlCommit(String("delete from maintenance where %1").arg(info))
                }
            }
        }

        NaviButton {
            id: operaEventQueryClearBtn
            text: qsTr("清空")
            height: 30
            width: 80
            onClicked: {
                maintInfoQueryModel.sqlCommit("delete from maintenance")
            }
        }
    }

    Controls1_4.TableView {

        width: parent.width
        height: 800
        anchors.topMargin: 10

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
            role: "state"
            title: qsTr("状态现象")
            width: 120
            // resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "methods"
            title: qsTr("维保方法")
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
        model: maintInfoQueryModel


    }
    }
    QmlTableModel {
        id: maintInfoQueryModel
        dbDriver: qsTr("QSQLITE")
        dbName: OperatorInfo.operatorInfoDbPath()
        dbConnectionName: "maintInfo"
        dbPort: 6688
        roleNameList: ["equipmentCode", "maintTime", "state", "methods", "contentDesc", "maintEngineer", "floor", "position", "system", "buildingName", "personOnDuty", "operator"]
        titleList: [qsTr("设备编码"), qsTr("维保时间"), qsTr("状态现象"), qsTr(
                "维保方法"), qsTr("内容描述"), qsTr("维保员"), qsTr("楼层"), qsTr(
                "部位"), qsTr("系统"), qsTr("建筑名称"), qsTr("值班人员"), qsTr("操作人员")]
    }

    Component.onCompleted: {
        maintInfoQueryModel.setDbOpen(true)
        maintInfoQueryModel.sqlCommit("select * from maintenance")
    }

    function selectInfo() {
        var info = new String
        if (equipmentNumTextField.text.length > 0) {
            info += (qsTr("设备编码=") + "'" + equipmentNumTextField.text + "'")
        }

        if (maintEngineerTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("维保员 =") + "'" + maintEngineerTextField.text + "'")
        }

        if (maintStartTimeTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("维保时间 >=") + "'" + maintStartTimeTextField.text + "'")
        }

        if (maintEndTimeTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("维保时间 <=") + "'" + maintEndTimeTextField.text + "'")
        }
        return info
    }

    function saveToPdf() {
        maintInfoQueryModel.saveToPdf()
    }
    function startPrint() {
        maintInfoQueryModel.startPrint()
    }
    function printPreview() {
        maintInfoQueryModel.printPreview()
    }
}
