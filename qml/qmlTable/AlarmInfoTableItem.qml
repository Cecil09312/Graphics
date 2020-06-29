import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import crtWidget 1.0
import Qt.labs.platform 1.0
import architePlanView 1.0
import "../infoSetting"
import "qrc:/jsFile/JsDateTime.js" as JsDateTime

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
                selectByMouse: true
                selectionColor: "blue"
                selectedTextColor: "white"
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
                id:networkNumTxt
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
                id:powerAddrTxt
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
                model: [qsTr("火警"),qsTr("监管"),qsTr("启动"),qsTr("反馈"),qsTr("故障"),qsTr("屏蔽"),qsTr("停止"),qsTr("反馈消除"),qsTr("故障恢复"),qsTr("屏蔽解除")]
            }
        }
        Row {

            spacing: 5
            Text {

                id:timeTxt
                text: qsTr("时间:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }

            Text {

                id:yearTxt
                text: qsTr("年:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }

            Controls1_4.SpinBox
            {
                id:alarmStartYearSpinBox

                maximumValue: 2200
                minimumValue: 1990
                value: Qt.formatDate(new Date,"yyyy")
                width: 60
                height: 25
                onValueChanged:
                {
                    setStartDate()
                }

            }
            Text {

                id:monthTxt
                text: qsTr("月:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }

            Controls1_4.SpinBox
            {
                id:alarmStartMonthSpinBox

                maximumValue: 12
                minimumValue: 1
                value: Qt.formatDate(new Date,"M")
                width: 50
                height: 25
                onValueChanged:
                {
                    setStartDate()
                }

            }
            Text {

                id:dateTxt
                text: qsTr("日:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }
            Controls1_4.SpinBox
            {
                id:alarmStartDateSpinBox

                maximumValue: 31
                minimumValue: 1
                value: Qt.formatDate(new Date,"d")
                width: 50
                height: 25

            }

            Text {

                id:toTxt
                text: qsTr(" 到:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }

            Text {

                id:alarmEndYearTxt
                text: qsTr("年:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }

            Controls1_4.SpinBox
            {
                id:alarmEndYearSpinBox

                maximumValue: 2200
                minimumValue: 1990
                value: Qt.formatDate(new Date,"yyyy")
                width: 60
                height:25

                onValueChanged:
                {
                    setEndDate()
                }

            }
            Text {

                id:alarmEndMonthTxt
                text: qsTr("月:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }

            Controls1_4.SpinBox
            {
                id:alarmEndMonthSpinBox

                maximumValue: 12
                minimumValue: 1
                value: Qt.formatDate(new Date,"M")
                width: 50
                height:25

                onValueChanged:
                {
                    setEndDate()
                }

            }
            Text {

                id:alarmEndDateTxt
                text: qsTr("日:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }
            Controls1_4.SpinBox
            {
                id:alarmEndDateSpinBox

                maximumValue: 31
                minimumValue: 1
                value: Qt.formatDate(new Date,"d")
                width: 50
                height: 25

            }

            NaviButton {
                id: alarmInfoQueryBtn
                text: qsTr("查询")
                height: 30
                width: 80
                onClicked: {
                    alarmInfoListModel.sqlCommit(
                                String(
                                    "select 分机号,回路号,地址号,网络号,电源地址,系统,设备编码,设备,事件类型,状态,时间,建筑名称,楼层,位置,操作员,备注 from AlarmInfo where %1 ").arg(
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
                    alarmInfoListModel.sqlCommit(
                                String("select 分机号,回路号,地址号,网络号,电源地址,系统,设备编码,设备,事件类型,状态,时间,建筑名称,楼层,位置,操作员,备注 from AlarmInfo where 状态='%1'").arg(qsTr("正常")))
                    alarmInfoTableView.resizeColumnsToContents()
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
            id:productNum
            role: "productNum"
            title: qsTr("设备编码")
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
            id:currentState
            role: "currentAlarmState"
            title: qsTr("状态")
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

        model: alarmInfoListModel
        onClicked: {
            var curExtNum = new String
            var curLoopNum = new String
            var curAddrNum = new String
            var curNetworkNum = new String
            var curPowerAddr = new String
            curExtNum = alarmInfoListModel.getValue(row, "extNum")
            curLoopNum = alarmInfoListModel.getValue(row, "loopNum")
            curAddrNum = alarmInfoListModel.getValue(row, "addrNum")
            curNetworkNum = alarmInfoListModel.getValue(row, "networkNum")
            curPowerAddr = alarmInfoListModel.getValue(row, "powerAddr")
            ArchitePlanView.toArchitePlan(curExtNum, curLoopNum, curAddrNum,
                                          curNetworkNum,curPowerAddr)
        }
    }
    QmlTableModel {
        id: alarmInfoListModel
        dbDriver: "QSQLITE"
        dbName: Crt.alarmInfoDbName()
        dbConnectionName: "alarmInfoDb"
        dbPort: 888
        roleNameList: ["extNum", "loopNum", "addrNum", "networkNum", "powerAddr","deviceSys", "productNum", "deviceType", "alarmType", "currentAlarmState", "alarmTime", "buildName", "floor", "deviceLocation", "operator","remarks"]
        titleList: [qsTr("分机号"), qsTr("回路号"), qsTr("地址号"), qsTr("网络号"),qsTr("电源地址"), qsTr("系统"), qsTr("设备编码"), qsTr("设备"), qsTr("事件类型"), qsTr("状态"), qsTr("时间"), qsTr("建筑名称"), qsTr("楼层"), qsTr("位置"), qsTr("操作员"),qsTr("备注")]
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
            info += ("回路号="+ "'" + alarmInfoLoopNumTextField.text.replace(/\s+/g,"") + "'")
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

        var startDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3 0:0:0").arg(alarmStartYearSpinBox.value).arg(alarmStartMonthSpinBox.value).arg(alarmStartDateSpinBox.value),"yyyy/M/d h:m:s");
        var startDate = new Date(startDateValue)
        var startDateStr=Qt.formatDateTime(startDate,"yyyy/MM/dd hh:mm:ss");


        if (startDateStr.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += ("时间 >=" + "'" + startDateStr + "'")
        }

        var endDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3 23:59:59").arg(alarmEndYearSpinBox.value).arg(alarmEndMonthSpinBox.value).arg(alarmEndDateSpinBox.value),"yyyy/M/d h:m:s");
        var endDate = new Date(endDateValue)
        var endDateStr=Qt.formatDateTime(endDate,"yyyy/MM/dd hh:mm:ss");

        if (endDateStr.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += ("时间 <=" + "'" + endDateStr + "'")
        }
        if (info.length > 0) {
            info += " and "
        }
        info+=String("状态='%1'").arg(qsTr("正常"))
        return info
    }

    function saveToPdf(fileName) {
        alarmInfoListModel.saveToPdf(fileName)
    }
    function startPrint() {
        alarmInfoListModel.startPrint()
    }
    function printPreview() {
        alarmInfoListModel.printPreview()
    }

    function setEndDate()
    {
        var curYear = alarmEndYearSpinBox.value
        var curMonth = alarmEndMonthSpinBox.value

        var maxDate=JsDateTime.getMaxDate(curYear,curMonth)
        alarmEndDateSpinBox.maximumValue=maxDate
        if(alarmEndDateSpinBox.value>maxDate)
        {
            alarmEndDateSpinBox.value = maxDate
        }

    }

    function setStartDate()
    {
        var curYear = alarmStartYearSpinBox.value
        var curMonth = alarmStartMonthSpinBox.value

        var maxDate=JsDateTime.getMaxDate(curYear,curMonth)
        alarmStartDateSpinBox.maximumValue=maxDate
        if(alarmStartDateSpinBox.value>maxDate)
        {
            alarmStartDateSpinBox.value = maxDate
        }

    }

    Component.onCompleted: {

        setStartDate()
        setEndDate()
        alarmInfoTableView.positionViewAtRow(alarmInfoTableView.rowCount - 1,
                                             ListView.Contain)
        alarmInfoListModel.setDbOpen(true)
//        alarmInfoListModel.sqlCommit(
//                    "select 分机号,回路号,地址号,网络号,电源地址,系统,设备编码,设备,事件类型,状态,时间,建筑名称,楼层,位置,操作员,备注 from AlarmInfo")
    }

    function retranslate()
    {
        alarmInfoExtNum.text = qsTr("分机号")
        alarmInfoLoopNum.text =  qsTr("回路号")
        alarmInfoAddNum.text = qsTr("地址号")
        networkNumTxt.text =qsTr("网络号")
        powerAddrTxt.text = qsTr("电源地址")
        alarmInfoAlarmType.text = qsTr("事件类型")
        alarmInfoAlarmTypeComboBox.model =[qsTr("火警"),qsTr("监管"),qsTr("启动"),qsTr("反馈"),qsTr("故障"),qsTr("屏蔽"),qsTr("停止"),qsTr("反馈消除"),qsTr("故障恢复"),qsTr("屏蔽解除")]
        timeTxt.text = qsTr("时间:")
        yearTxt.text =qsTr("年:")
        monthTxt.text = qsTr("月:")
        dateTxt.text = qsTr("日:")
        toTxt.text = qsTr(" 到:")
        alarmEndYearTxt.text = qsTr("年:")
        alarmEndMonthTxt.text = qsTr("月:")
        alarmEndDateTxt.text = qsTr("日:")
        alarmInfoQueryBtn.text = qsTr("查询")
        alarmInfoQueryAllBtn.text = qsTr("查询所有")
        extNum.title =qsTr("分机号")
        loopNum.title = qsTr("回路号")
        addrNum.title = qsTr("地址号")
        networkNum.title = qsTr("网络号")
        powerAddr.title = qsTr("电源地址")
        deviceSys.title = qsTr("系统")
        productNum.title = qsTr("设备编码")
        deviceType.title = qsTr("设备")
        alarmType.title = qsTr("事件类型")
        currentState.title = qsTr("状态")
        alarmTime.title = qsTr("时间")
        buildingName.title = qsTr("建筑名称")
        floor.title = qsTr("楼层")
        location.title = qsTr("位置")
        operator.title = qsTr("操作员")
        remarks.title = qsTr("备注")
    }
}
