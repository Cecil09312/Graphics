import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import crtWidget 1.0
import QtQuick.Dialogs 1.2
import architePlanView 1.0
import "../infoSetting"

Rectangle
{
    width: 960
    height: 480
    anchors.fill: parent
    signal setCurAlarm(string alarmType)
    Column {
        id: alarmInfoQueryColumn
        spacing: 10
        anchors.top: parent.top
        anchors.topMargin: 20
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
                selectionColor: "blue"
                selectedTextColor: "white"
                selectByMouse: true
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
                selectionColor: "blue"
                selectedTextColor: "white"
                selectByMouse: true
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
                selectionColor: "blue"
                selectedTextColor: "white"
                selectByMouse: true
            }

            Text {
                id:alarmInfoNetworkNum
                text: qsTr("网络号")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }
            TextField {
                id: networkNumTextField
                width: 100
                height: 30
                selectionColor: "blue"
                selectedTextColor: "white"
                selectByMouse: true
            }

            Text {
                id:alarmInfoPowerAddr
                text: qsTr("电源地址")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }
            TextField {
                id: powerAddrTextField
                width: 100
                height: 30
                selectionColor: "blue"
                selectedTextColor: "white"
                selectByMouse: true
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
                model: [qsTr("火警"),qsTr("监管"),qsTr("启动"),qsTr("反馈"),qsTr("故障"),qsTr("屏蔽")]
                onCurrentTextChanged:
                {
                   // if(currentText===qsTr("火警")||currentText===qsTr("监管")||currentText===qsTr("启动")||currentText===qsTr("反馈")||currentText===qsTr("故障")||currentText===qsTr("屏蔽"))
                   // {
                        emit:setCurAlarm(currentText)
                   // }
//                    else
//                    {
//                        emit:setCurAlarm(qsTr("火警"))
//                    }


                }
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
                                    "select 分机号,回路号,地址号,网络号,电源地址,系统,设备,事件类型,时间,建筑名称,楼层,位置,操作员,备注 from AlarmInfo where %1").arg(
                                    selectInfo()))

                    alarmInfoTableView.resizeColumnsToContents()
                }
            }

            NaviButton {
                id: alarmInfoQueryAllBtn
                text: qsTr("查询所有")
                height: 30
                width: 100
                onClicked: {
                    selectAll()
                }
            }
        }
    }

    Controls1_4.TableView {
        id: alarmInfoTableView
        //anchors.fill: parent
        width: parent.width
        anchors.topMargin: 10
        clip: true
        anchors.bottom: parent.bottom
        anchors.top: alarmInfoQueryColumn.bottom



        Controls1_4.TableViewColumn {
            id:extNum
            role: "extNum"
            title: qsTr("分机号")
            width: 60
        }

        Controls1_4.TableViewColumn {
            id:loopNum
            role: "loopNum"
            title: qsTr("回路号")
            width: 60
        }

        Controls1_4.TableViewColumn {
            id:addrNum
            role: "addrNum"
            title: qsTr("地址号")
            width: 60
        }

        Controls1_4.TableViewColumn {
            id:networkNum
            role: "networkNum"
            title: qsTr("网络号")
            width: 60
        }

        Controls1_4.TableViewColumn {
            id:powerAddr
            role: "powerAddr"
            title: qsTr("电源地址")
            width: 60
        }
        Controls1_4.TableViewColumn {
            id:deviceSys
            role: "deviceSys"
            title: qsTr("系统")
            width: 60
        }



        Controls1_4.TableViewColumn {
            id:deviceType
            role: "deviceType"
            title: qsTr("设备")
            width: 60
        }

        Controls1_4.TableViewColumn {
            id:alarmType
            role: "alarmType"
            title: qsTr("事件类型")
            width: 60
        }

        Controls1_4.TableViewColumn {
            id:alarmTime
            role: "alarmTime"
            title: qsTr("时间")
            width: 150
        }

        Controls1_4.TableViewColumn {
            id:buildingName
            role: "buildName"
            title: qsTr("建筑名称")
            width: 60
        }

        Controls1_4.TableViewColumn {
            id:floor
            role: "floor"
            title: qsTr("楼层")
            width: 60
        }

        Controls1_4.TableViewColumn {
            id:location
            role: "deviceLocation"
            title: qsTr("位置")
            width: 60
        }

        Controls1_4.TableViewColumn {
            id:operator
            role: "operator"
            title: qsTr("操作员")
            width: 60
        }

        Controls1_4.TableViewColumn {
            id:remarks
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
        dbDriver: "QSQLITE"
        dbName: Crt.alarmInfoDbName()
        dbConnectionName: "oneAlarmInfoDb"
        dbPort: 77980
        roleNameList: ["extNum", "loopNum", "addrNum", "networkNum", "powerAddr","deviceSys",  "deviceType", "alarmType",  "alarmTime", "buildName", "floor", "deviceLocation", "operator","remarks"]
        titleList: [qsTr("分机号"), qsTr("回路号"), qsTr("地址号"), qsTr("网络号"),qsTr("电源地址"), qsTr("系统"),  qsTr("设备"), qsTr("事件类型"), qsTr("时间"), qsTr("建筑名称"), qsTr("楼层"), qsTr("位置"), qsTr("操作员"),qsTr("备注")]
    }
    function selectInfo() {
        var info = new String

        if (alarmInfoExtNumTextField.text.replace(/\s+/g,"").length > 0) {
            info += ("分机号=" + "'" + alarmInfoExtNumTextField.text.replace(/\s+/g,"") + "'")
        }

        if (alarmInfoLoopNumTextField.text.replace(/\s+/g,"").length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += ("回路号=" + "'" + alarmInfoLoopNumTextField.text.replace(/\s+/g,"") + "'")
        }
        if (alarmInfoAddNumTextField.text.replace(/\s+/g,"").length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += ("地址号=" + "'" + alarmInfoAddNumTextField.text.replace(/\s+/g,"") + "'")
        }

        if (networkNumTextField.text.replace(/\s+/g,"").length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += ("网络号=" + "'" + networkNumTextField.text.replace(/\s+/g,"") + "'")
        }

        if (powerAddrTextField.text.replace(/\s+/g,"").length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += ("电源地址=" + "'" +powerAddrTextField.text.replace(/\s+/g,"") + "'")
        }

        if (alarmInfoAlarmTypeComboBox.currentText.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += ("事件类型=" + "'" + alarmInfoAlarmTypeComboBox.currentText + "'")
        }

        if (info.length > 0) {
            info += " and "
        }
        info += String("状态!='%1'").arg("OK")


        return info
    }

    function saveToPdf(fileName) {
        currentAlarmListModel.saveToPdf(fileName)
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

    }

    function setCurrentAlarm(curAlarm)
    {

        currentAlarmListModel.clearData()
        if(alarmInfoAlarmTypeComboBox.contains(curAlarm))
        {
          var index=  alarmInfoAlarmTypeComboBox.find(curAlarm)
            alarmInfoAlarmTypeComboBox.currentIndex=index
        }

    }

    function retranslate()
    {
        alarmInfoExtNum.text = qsTr("分机号")
        alarmInfoLoopNum.text =qsTr("回路号")
        alarmInfoAddNum.text = qsTr("地址号")
        alarmInfoNetworkNum.text = qsTr("网络号")
        alarmInfoPowerAddr.text = qsTr("电源地址")
        alarmInfoAlarmType.text=qsTr("事件类型")
        alarmInfoAlarmTypeComboBox.model =[qsTr("火警"),qsTr("监管"),qsTr("启动"),qsTr("反馈"),qsTr("故障"),qsTr("屏蔽")]
        alarmInfoQueryBtn.text = qsTr("查询")
        alarmInfoQueryAllBtn.text = qsTr("查询所有")
        extNum.title = qsTr("分机号")
        loopNum.title = qsTr("回路号")
        addrNum.title = qsTr("地址号")
        networkNum.title = qsTr("网络号")
        powerAddr.title = qsTr("电源地址")
        deviceSys.title = qsTr("系统")
        deviceType.title = qsTr("设备")
        alarmType.title = qsTr("事件类型")
        alarmTime.title = qsTr("时间")
        buildingName.title = qsTr("建筑名称")
        floor.title = qsTr("楼层")
        location.title = qsTr("位置")
        operator.title = qsTr("操作员")
        remarks.title = qsTr("备注")


    }


   function selectAll()
   {
       currentAlarmListModel.sqlCommit(
                   String("select 分机号,回路号,地址号,网络号,电源地址,系统,设备,事件类型,时间,建筑名称,楼层,位置,操作员,备注 from AlarmInfo where 状态!='%1' and 事件类型='%2' ").arg("OK").arg(alarmInfoAlarmTypeComboBox.currentText))

       alarmInfoTableView.resizeColumnsToContents()

   }

}

