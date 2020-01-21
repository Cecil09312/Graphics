import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import crtWidget 1.0
import QtQuick.Dialogs 1.2
import architePlanView 1.0
import "../infoSetting"
Item {

    width: 800
    height: 640
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    Column
    {
        anchors.fill: parent

        Column {
            id: alarmInfoQuery
            spacing: 5
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
                Text {

                    text: qsTr("时间")
                    height: 30
                    horizontalAlignment: TextEdit.AlignLeft
                    verticalAlignment: TextEdit.AlignVCenter
                }
                TextField {
                    id: alarmStartTimeTextField
                    height: 30
                    width: 150
                    placeholderText: qsTr("如:2019/01/01 00:00:00")
                }

                Text {

                    text: qsTr("到")
                    height: 30
                    horizontalAlignment: TextEdit.AlignLeft
                    verticalAlignment: TextEdit.AlignVCenter
                }
                TextField {
                    id: alarmEndTimeTextField
                    height: 30
                    width: 150
                    placeholderText: qsTr("如:2050/01/01 00:00:00")
                }
                NaviButton {
                    id: alarmInfoQueryBtn
                    text: qsTr("查询")
                    height: 30
                    width: 80
                    onClicked: {
                        alarmInfoListModel.sqlCommit(
                                    String(
                                        "select 分机号,回路号,地址号,网络号,系统,设备编码,设备,事件类型,状态,时间,建筑名称,楼层,位置,操作员,备注 from AlarmInfo where %1").arg(
                                        selectInfo()))
                    }
                }

                NaviButton {
                    id: alarmInfoQueryAllBtn
                    text: qsTr("查询所有")
                    height: 30
                    width: 100
                    onClicked: {
                        alarmInfoListModel.sqlCommit(
                                    "select 分机号,回路号,地址号,网络号,系统,设备编码,设备,事件类型,状态,时间,建筑名称,楼层,位置,操作员,备注 from AlarmInfo")
                    }
                }


                NaviButton {
                    id: alarmInfoDeleteBtn
                    text: qsTr("删除")
                    height: 30
                    width: 80
                    onClicked: {
                        var info =  new String
                        info= selectInfo();
                        if(info.length>0)
                        {
                            alarmInfoListModel.sqlCommit( String("delete  from AlarmInfo where %1").arg(selectInfo()))
                        }
                    }
                }
                NaviButton {
                    id: alarmInfoClearBtn
                    text: qsTr("清空")
                    height: 30
                    width: 80
                    onClicked: {
                        messageDialog.open()
                    }
                }
            }
        }

        Controls1_4.TableView {
            id: alarmInfoTableView

            width:parent.width
            height: 800
            anchors.topMargin: 10

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
                role: "currentAlarmState"
                title: qsTr("状态")
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

            model: alarmInfoListModel
            onDoubleClicked: {
                var curExtNum = new String
                var curLoopNum = new String
                var curAddrNum = new String
                var curNetworkNum = new String
                var currentState = new String
                curExtNum = alarmInfoListModel.getValue(row, "extNum")
                curLoopNum = alarmInfoListModel.getValue(row, "loopNum")
                curAddrNum = alarmInfoListModel.getValue(row, "addrNum")
                curNetworkNum = alarmInfoListModel.getValue(row, "networkNum")
                ArchitePlanView.toArchitePlan(curExtNum, curLoopNum, curAddrNum,
                                              curNetworkNum)
            }
        }
    }
    QmlTableModel {
        id: alarmInfoListModel
        dbDriver: qsTr("QSQLITE")
        dbName: Crt.alarmInfoDbName()
        dbConnectionName: "alarmInfoDb"
        dbPort: 888
        roleNameList: ["extNum", "loopNum", "addrNum", "networkNum", "deviceSys", "productNum", "deviceType", "alarmType", "currentAlarmState", "alarmTime", "buildName", "floor", "deviceLocation", "operator","remarks"]
        titleList: ["分机号", "回路号", "地址号", "网络号", "系统", "设备编码", "设备", "事件类型", "状态", "时间", "建筑名称", "楼层", "位置", "操作员","备注"]
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
        if (alarmInfoAlarmTypeComboBox.currentText.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("事件类型=") + "'" + alarmInfoAlarmTypeComboBox.currentText + "'")
        }

        if (alarmStartTimeTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("时间 >=") + "'" + alarmStartTimeTextField.text + "'")
        }

        if (alarmEndTimeTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("时间 <=") + "'" + alarmEndTimeTextField.text + "'")
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

        alarmInfoTableView.positionViewAtRow(alarmInfoTableView.rowCount - 1,
                                             ListView.Contain)
        alarmInfoListModel.setDbOpen(true)
        alarmInfoListModel.sqlCommit(
                    "select 分机号,回路号,地址号,网络号,系统,设备编码,设备,事件类型,状态,时间,建筑名称,楼层,位置,操作员,备注 from AlarmInfo")
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
