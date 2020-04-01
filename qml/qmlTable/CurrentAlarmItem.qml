import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import crtWidget 1.0
import QtQuick.Dialogs 1.2
import architePlanView 1.0
import "../infoSetting"

Item
{
    Column {
        id: alarmInfoQuery
        spacing: 10
        Row {
            spacing: 5
            Text {
                id: alarmInfoExtNum
                text: qsTr("分机号")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
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
                horizontalAlignment: TextEdit.AlignLeft
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
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }
            TextField {
                id: alarmInfoAddNumTextField
                width: 100
                height: 30
            }

            Text {
                text: qsTr("网络号")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }
            TextField {
                id: networkNumTextField
                width: 100
                height: 30
            }

            Text {
                text: qsTr("电源地址")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }
            TextField {
                id: powerAddrTextField
                width: 100
                height: 30
            }

            Text {
                id: alarmInfoAlarmType
                text: qsTr("事件类型")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }
            ComboBox {
                id: alarmInfoAlarmTypeComboBox
                height: 30
                width: 100
                model: [qsTr("火警"),qsTr("监管"),qsTr("启动"),qsTr("反馈"),qsTr("故障"),qsTr("屏蔽"),qsTr("停止"),qsTr("反馈消除"),qsTr("故障恢复"),qsTr("屏蔽解除")]
            }
        }
        Row {

            spacing: 5



            NaviButton {
                id: alarmInfoQueryBtn
                text: qsTr("查询")
                height: 30
                width: 80
                onClicked: {
                    currentAlarmListModel.sqlCommit(
                                String(
                                    "select 分机号,回路号,地址号,网络号,电源地址,系统,设备编码,设备,事件类型,时间,建筑名称,楼层,位置,操作员,备注 from AlarmInfo where %1").arg(
                                    selectInfo()))
                }
            }

            NaviButton {
                id: alarmInfoQueryAllBtn
                text: qsTr("查询所有")
                height: 30
                width: 100
                onClicked: {
                    currentAlarmListModel.sqlCommit(
                                "select 分机号,回路号,地址号,网络号,电源地址,系统,设备编码,设备,事件类型,时间,建筑名称,楼层,位置,操作员,备注 from AlarmInfo where 状态!='正常'")
                }
            }
        }
    }

    Controls1_4.TableView {
        id: alarmInfoTableView
        width: parent.width
        anchors.topMargin: 10
        clip: true
        anchors.bottom: parent.bottom
        anchors.top: alarmInfoQuery.bottom

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
            role: "networkNum"
            title: qsTr("网络号")
            width: 60
        }

        Controls1_4.TableViewColumn {
            role: "powerAddr"
            title: qsTr("电源地址")
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
            role: "alarmType"
            title: qsTr("事件类型")
            width: 60
        }

        Controls1_4.TableViewColumn {
            role: "alarmTime"
            title: qsTr("时间")
            width: 150
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

        Controls1_4.TableViewColumn {
            role: "remarks"
            title: qsTr("备注")
            width: 60
        }

        model: currentAlarmListModel
        onDoubleClicked: {
            var curExtNum = new String
            var curLoopNum = new String
            var curAddrNum = new String
            var curNetworkNum = new String
            var curPowerAddr = new String
            curExtNum = currentAlarmListModel.getValue(row, "extNum")
            curLoopNum = currentAlarmListModel.getValue(row, "loopNum")
            curAddrNum = currentAlarmListModel.getValue(row, "addrNum")
            curNetworkNum = currentAlarmListModel.getValue(row, "networkNum")
            curPowerAddr = currentAlarmListModel.getValue(row, "powerAddr")
            ArchitePlanView.toArchitePlan(curExtNum, curLoopNum, curAddrNum,
                                          curNetworkNum,curPowerAddr)
        }
    }
    QmlTableModel {
        id: currentAlarmListModel
        dbDriver: qsTr("QSQLITE")
        dbName: Crt.alarmInfoDbName()
        dbConnectionName: "curAlarmInfoDb"
        dbPort: 6688
        roleNameList: ["extNum", "loopNum", "addrNum", "networkNum", "powerAddr","deviceSys", "productNum", "deviceType", "alarmType",  "alarmTime", "buildName", "floor", "deviceLocation", "operator","remarks"]
        titleList: ["分机号", "回路号", "地址号", "网络号","电源地址", "系统", "设备编码", "设备", "事件类型", "时间", "建筑名称", "楼层", "位置", "操作员","备注"]
    }
    function selectInfo() {
        var info = new String

        if (alarmInfoExtNumTextField.text.length > 0) {
            info += (qsTr("分机号=") + "'" + alarmInfoExtNumTextField.text + "'")
        }

        if (alarmInfoLoopNumTextField.text.length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("回路号=") + "'" + alarmInfoLoopNumTextField.text + "'")
        }
        if (alarmInfoAddNumTextField.text.length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("地址号=") + "'" + alarmInfoAddNumTextField.text + "'")
        }

        if (networkNumTextField.text.length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("网络号=") + "'" + networkNumTextField.text + "'")
        }

        if (powerAddrTextField.text.length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("电源地址=") + "'" +powerAddrTextField.text + "'")
        }

        if (alarmInfoAlarmTypeComboBox.currentText.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("事件类型=") + "'" + alarmInfoAlarmTypeComboBox.currentText + "'")
        }

        if (info.length > 0) {
            info += " and "
        }
        info += (qsTr("状态!='正常'"))


        return info
    }

    function saveToPdf() {
        currentAlarmListModel.saveToPdf()
    }
    function startPrint() {
        currentAlarmListModel.startPrint()
    }
    function printPreview() {
        currentAlarmListModel.printPreview()
    }

    Component.onCompleted: {

        alarmInfoTableView.positionViewAtRow(alarmInfoTableView.rowCount - 1,
                                             ListView.Contain)
        currentAlarmListModel.setDbOpen(true)
        currentAlarmListModel.sqlCommit(
                    "select 分机号,回路号,地址号,网络号,电源地址,系统,设备编码,设备,事件类型,时间,建筑名称,楼层,位置,操作员,备注 from AlarmInfo where 状态!='正常'")
    }


}

