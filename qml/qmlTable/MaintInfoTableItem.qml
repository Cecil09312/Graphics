import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import operatorInfo 1.0
import "../infoSetting"

Item
{
    // anchors.fill: parent
    Column {
        id: operaEventQuery
        spacing: 5
        //columns: 18
        anchors.topMargin: 20
        Row
        {
            spacing: 5
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
                text: qsTr(" 维保时间:")
                height: 30
                horizontalAlignment: TextEdit.AlignHCenter
                verticalAlignment: TextEdit.AlignVCenter
            }
            Text {

                text: qsTr("年:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }

            Controls1_4.SpinBox
            {
                id:maintStartYearSpinBox

                maximumValue: 2100
                minimumValue: 1970
                value: Qt.formatDate(new Date,"yyyy")
                width: 60
                height:25

            }
            Text {

                text: qsTr("月:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }

            Controls1_4.SpinBox
            {
                id:maintStartMonthSpinBox

                maximumValue: 12
                minimumValue: 1
                value: Qt.formatDate(new Date,"M")
                width: 50
                height:25

            }
            Text {

                text: qsTr("日:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }
            Controls1_4.SpinBox
            {
                id:maintStartDateSpinBox

                maximumValue: 31
                minimumValue: 1
                value: Qt.formatDate(new Date,"d")
                width: 50
                height: 25

            }

            Text {
                text: qsTr(" 到:")
                height: 30
                horizontalAlignment: TextEdit.AlignHCenter
                verticalAlignment: TextEdit.AlignVCenter
            }
            Text {

                text: qsTr("年:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }

            Controls1_4.SpinBox
            {
                id:maintEndYearSpinBox

                maximumValue: 2100
                minimumValue: 1970
                value: Qt.formatDate(new Date,"yyyy")
                width: 60
                height:25

            }
            Text {

                text: qsTr("月:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }

            Controls1_4.SpinBox
            {
                id:maintEndMonthSpinBox

                maximumValue: 12
                minimumValue: 1
                value: Qt.formatDate(new Date,"M")
                width: 50
                height:25

            }
            Text {

                text: qsTr("日:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }
            Controls1_4.SpinBox
            {
                id:maintEndDateSpinBox

                maximumValue: 31
                minimumValue: 1
                value: Qt.formatDate(new Date,"d")
                width: 50
                height: 25

            }
        }

        Row
        {
            spacing: 5
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
    }


    Controls1_4.TableView {

        width: parent.width
        anchors.topMargin: 10
        clip: true
        anchors.bottom: parent.bottom
        anchors.top: operaEventQuery.bottom

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


        var startDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3 0:0:0").arg(maintStartYearSpinBox.value).arg(maintStartMonthSpinBox.value).arg(maintStartDateSpinBox.value),"yyyy/M/d h:m:s");
        var startDate = new Date(startDateValue)
        var startDateStr=Qt.formatDateTime(startDate,"yyyy/MM/dd hh:mm:ss");

        if (startDateStr.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("维保时间 >=") + "'" + startDateStr + "'")
        }

        var endDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3 23:59:59").arg(maintEndYearSpinBox.value).arg(maintEndMonthSpinBox.value).arg(maintEndDateSpinBox.value),"yyyy/M/d h:m:s");
        var endDate = new Date(endDateValue)
        var endDateStr=Qt.formatDateTime(endDate,"yyyy/MM/dd hh:mm:ss");

        if (endDateStr.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("维保时间 <=") + "'" + endDateStr + "'")
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
