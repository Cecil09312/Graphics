import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import crtWidget 1.0
import QtQuick.Dialogs 1.2

Item {
    //id: alarmInfoItem
    //title: qsTr("报警记录查询")
    // anchors.fill: parent
    anchors.bottomMargin: 40
    anchors.topMargin: 20
    Row {
        id: alarmInfoQuery
        spacing: 5
        Text {
            id: alarmInfoExtNum
            text: qsTr("分机号")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: alarmInfoExtNumTextField
            width: 100
            height: 30
        }

        Text {
            id: alarmInfoLoopNum
            text: qsTr("回路号")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: alarmInfoLoopNumTextField
            width: 100
            height: 30
        }

        Text {
            id: alarmInfoAddNum
            text: qsTr("地址号")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: alarmInfoAddNumTextField
            width: 100
            height: 30
        }

        Text {
            id: alarmInfoAlarmState
            text: qsTr("报警状态")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: alarmInfoAlarmStateTextField
            height: 30
            width: 100
        }
        Button {
            id: alarmInfoQueryBtn
            text: qsTr("查询")
            height: 30
            onClicked: {
                alarmInfoListModel.sqlCommit(
                            String(
                                "select 分机号,回路号,地址号,系统,设备编码,设备,报警状态,报警时间,报警恢复时间,建筑名称,楼层,位置,操作员 from AlarmInfo where %1").arg(
                                selectInfo()))
            }
        }

        Button {
            id: alarmInfoQueryAllBtn
            text: qsTr("查询所有")
            height: 30
            onClicked: {
                alarmInfoListModel.sqlCommit(
                            "select 分机号,回路号,地址号,系统,设备编码,设备,报警状态,报警时间,报警恢复时间,建筑名称,楼层,位置,操作员 from AlarmInfo")
            }
        }

        Button {
            id: alarmInfoClearBtn
            text: qsTr("清空")
            height: 30
            onClicked: {
                messageDialog.open()
            }
        }
    }

    Controls1_4.TableView {
        id: alarmInfoTableView
        anchors.top: alarmInfoQuery.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        width: alarmInfoQuery.width
        anchors.topMargin: 5

        Controls1_4.TableViewColumn {
            role: "extNum"
            title: qsTr("分机号")
            width: 60
        }

        Controls1_4.TableViewColumn {
            role: "loopNum"
            title: qsTr("回路号")
            width: 60
        }

        Controls1_4.TableViewColumn {
            role: "addrNum"
            title: qsTr("地址号")
            width: 60
        }
        Controls1_4.TableViewColumn {
            role: "deviceSys"
            title: qsTr("系统")
            width: 60
        }

        Controls1_4.TableViewColumn {
            role: "productNum"
            title: qsTr("设备编码")
            width: 60
        }

        Controls1_4.TableViewColumn {
            role: "deviceType"
            title: qsTr("设备")
            width: 60
        }

        Controls1_4.TableViewColumn {
            role: "currentAlarmState"
            title: qsTr("报警状态")
            width: 60
        }

        Controls1_4.TableViewColumn {
            role: "alarmTime"
            title: qsTr("报警时间")
            width: 100
        }

        Controls1_4.TableViewColumn {
            role: "rebackAlarmTime"
            title: qsTr("报警恢复时间")
            width: 100
        }

        Controls1_4.TableViewColumn {
            role: "buildName"
            title: qsTr("建筑名称")
            width: 60
        }

        Controls1_4.TableViewColumn {
            role: "floor"
            title: qsTr("楼层")
            width: 60
        }

        Controls1_4.TableViewColumn {
            role: "deviceLocation"
            title: qsTr("位置")
            width: 60
        }

        Controls1_4.TableViewColumn {
            role: "operator"
            title: qsTr("操作员")
            width: 60
        }

        model: alarmInfoListModel
    }
    QmlTableModel {
        id: alarmInfoListModel
        dbDriver: qsTr("QSQLITE")
        dbName: Crt.alarmInfoDbName()
        dbConnectionName: "alarmInfoDb"
        dbPort: 888
        roleNameList: ["extNum", "loopNum", "addrNum", "deviceSys", "productNum", "deviceType", "currentAlarmState", "alarmTime", "rebackAlarmTime", "buildName", "floor", "deviceLocation", "operator"]
        titleList: ["分机号", "回路号", "地址号", "系统", "设备编码", "设备", "报警状态", "报警时间", "报警恢复时间", "建筑名称", "楼层", "位置", "操作员"]
    }
    function selectInfo() {
        var info = new String
        if (alarmInfoExtNumTextField.text.length > 0) {
            info += (qsTr("分机号=") + alarmInfoExtNumTextField.text)
        }

        if (alarmInfoLoopNumTextField.text.length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("回路号=") + alarmInfoLoopNumTextField.text)
        }
        if (alarmInfoAddNumTextField.text.length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("地址号=") + alarmInfoAddNumTextField.text)
        }

        if (alarmInfoAlarmStateTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("报警状态=") + "'" + alarmInfoAlarmStateTextField.text + "'")
        }
        return info
    }

    function saveToPdf() {
        alarmInfoListModel.saveToPdf()
    }
    function startPrint() {
        alarmInfoListModel.startPrint()
    }
    function printPreview() {
        alarmInfoListModel.printPreview()
    }

    Component.onCompleted: {

        alarmInfoListModel.setDbOpen(true)
        alarmInfoListModel.sqlCommit(
                    "select 分机号,回路号,地址号,系统,设备编码,设备,报警状态,报警时间,报警恢复时间,建筑名称,楼层,位置,操作员 from AlarmInfo")
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("警告提示")
        text: qsTr("如果信息超过1000条，或者没有正在报警的信息可以清除。注意信息的保存和备份。")
        icon: StandardIcon.Warning
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {
            alarmInfoListModel.sqlCommit("delete from AlarmInfo")
        }
        onNo: {
            close()
        }
    }
}
