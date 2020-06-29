import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import operatorInfo 1.0
import "../infoSetting"
import "qrc:/jsFile/JsDateTime.js" as JsDateTime

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
                selectionColor: "blue"
                selectedTextColor: "white"
                selectByMouse: true
            }

            Text {
                id:maintainerTxt
                text: qsTr("维保员")
                height: 30
                horizontalAlignment: TextEdit.AlignHCenter
                verticalAlignment: TextEdit.AlignVCenter
            }
            TextField {
                id: maintEngineerTextField
                width: 80
                height: 30
                selectionColor: "blue"
                selectedTextColor: "white"
                selectByMouse: true
            }

            Text {
                id: maintTime
                text: qsTr(" 维保时间:")
                height: 30
                horizontalAlignment: TextEdit.AlignHCenter
                verticalAlignment: TextEdit.AlignVCenter
            }
            Text {
                id:maintStartYearTxt
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
                onValueChanged:
                {
                    setStartTime()
                }

            }
            Text {

                id:maintStartMonthTxt
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
                onValueChanged:
                {
                    setStartTime()
                }

            }
            Text {

                id:maintStartDateTxt
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
                id:toTxt
                text: qsTr(" 到:")
                height: 30
                horizontalAlignment: TextEdit.AlignHCenter
                verticalAlignment: TextEdit.AlignVCenter
            }
            Text {

                id:maintEndYearTxt
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
                onValueChanged:
                {
                    setEndTime()
                }

            }
            Text {

                id:maintEndMonthTxt
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
                onValueChanged:
                {
                    setEndTime()
                }

            }
            Text {

                id:maintEndDateTxt
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
                    maintInfoTableView.resizeColumnsToContents()
                }
            }

            NaviButton {
                id: operaEventQueryAllBtn
                text: qsTr("查询所有")
                height: 30
                width: 100
                onClicked: {
                    maintInfoQueryModel.sqlCommit("select * from maintenance")
                    maintInfoTableView.resizeColumnsToContents()
                }
            }



        }
    }


    Controls1_4.TableView {

        id:maintInfoTableView
        width: parent.width
        anchors.topMargin: 10
        clip: true
        anchors.bottom: parent.bottom
        anchors.top: operaEventQuery.bottom

        Controls1_4.TableViewColumn {
            id:equipmentCode
            role: "equipmentCode"
            title: qsTr("设备编码")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:maintTimeColumn
            role: "maintTime"
            title: qsTr("维保时间")
            width: 120
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:state
            role: "state"
            title: qsTr("状态现象")
            width: 120
            // resizable: true
        }

        Controls1_4.TableViewColumn {
            id:methods
            role: "methods"
            title: qsTr("维保方法")
            width: 120
            // resizable: true
        }

        Controls1_4.TableViewColumn {
            id:contentDesc
            role: "contentDesc"
            title: qsTr("内容描述")
            width: 120
            // resizable: true
        }

        Controls1_4.TableViewColumn {
            id:maintEngineer
            role: "maintEngineer"
            title: qsTr("维保员")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:floor
            role: "floor"
            title: qsTr("楼层")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:position
            role: "position"
            title: qsTr("部位")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:system
            role: "system"
            title: qsTr("系统")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:buildingName
            role: "buildingName"
            title: qsTr("建筑名称")
            width: 60
        }

        Controls1_4.TableViewColumn {
            id:personOnDuty
            role: "personOnDuty"
            title: qsTr("值班人员")
            width: 60
        }

        Controls1_4.TableViewColumn {
            id:operator
            role: "operator"
            title: qsTr("操作人员")
            width: 60
        }
        model: maintInfoQueryModel


    }

    QmlTableModel {
        id: maintInfoQueryModel
        dbDriver: "QSQLITE"
        dbName: OperatorInfo.operatorInfoDbPath()
        dbConnectionName: "maintInfo"
        dbPort: 6688
        roleNameList: ["equipmentCode", "maintTime", "state", "methods", "contentDesc", "maintEngineer", "floor", "position", "system", "buildingName", "personOnDuty", "operator"]
        titleList: [qsTr("设备编码"), qsTr("维保时间"), qsTr("状态现象"), qsTr(
                "维保方法"), qsTr("内容描述"), qsTr("维保员"), qsTr("楼层"), qsTr(
                "部位"), qsTr("系统"), qsTr("建筑名称"), qsTr("值班人员"), qsTr("操作人员")]
    }

    Component.onCompleted: {
        setStartTime()
        setEndTime()
        maintInfoQueryModel.setDbOpen(true)
       // maintInfoQueryModel.sqlCommit("select * from maintenance")
    }

    function setStartTime()
    {
        var curYear = maintStartYearSpinBox.value
        var curMonth = maintStartMonthSpinBox.value

        var maxDate=JsDateTime.getMaxDate(curYear,curMonth)
        maintStartDateSpinBox.maximumValue=maxDate
        if(maintStartDateSpinBox.value>maxDate)
        {
            maintStartDateSpinBox.value = maxDate
        }
    }

    function setEndTime()
    {
        var curYear = maintEndYearSpinBox.value
        var curMonth = maintEndMonthSpinBox.value

        var maxDate=JsDateTime.getMaxDate(curYear,curMonth)
        maintEndDateSpinBox.maximumValue=maxDate
        if(maintEndDateSpinBox.value>maxDate)
        {
            maintEndDateSpinBox.value = maxDate
        }
    }

    function selectInfo() {
        var info = new String
        if (equipmentNumTextField.text.length > 0) {
            info += ("设备编码=" + "'" + equipmentNumTextField.text + "'")
        }

        if (maintEngineerTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += ("维保员 =" + "'" + maintEngineerTextField.text + "'")
        }


        var startDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3 0:0:0").arg(maintStartYearSpinBox.value).arg(maintStartMonthSpinBox.value).arg(maintStartDateSpinBox.value),"yyyy/M/d h:m:s");
        var startDate = new Date(startDateValue)
        var startDateStr=Qt.formatDateTime(startDate,"yyyy/MM/dd hh:mm:ss");

        if (startDateStr.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += ("维保时间 >=" + "'" + startDateStr + "'")
        }

        var endDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3 23:59:59").arg(maintEndYearSpinBox.value).arg(maintEndMonthSpinBox.value).arg(maintEndDateSpinBox.value),"yyyy/M/d h:m:s");
        var endDate = new Date(endDateValue)
        var endDateStr=Qt.formatDateTime(endDate,"yyyy/MM/dd hh:mm:ss");

        if (endDateStr.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += ("维保时间 <=" + "'" + endDateStr + "'")
        }

        return info
    }

    function saveToPdf(fileName) {
        maintInfoQueryModel.saveToPdf(fileName)
    }
    function startPrint() {
        maintInfoQueryModel.startPrint()
    }
    function printPreview() {
        maintInfoQueryModel.printPreview()
    }

    function retranslate()
    {
        equipmentNum.text = qsTr("设备编码")
        maintainerTxt.text =  qsTr("维保员")
        maintTime.text = qsTr(" 维保时间:")
        maintStartYearTxt.text = qsTr("年:")
        maintStartMonthTxt.text = qsTr("月:")
        maintStartDateTxt.text = qsTr("日:")
        toTxt.text = qsTr(" 到:")
        maintEndYearTxt.text = qsTr("年:")
        maintEndMonthTxt.text = qsTr("月:")
        maintEndDateTxt.text = qsTr("日:")
        operaEventQueryBtn.text=qsTr("查询")
        operaEventQueryAllBtn.text = qsTr("查询所有")
        equipmentCode.title = qsTr("设备编码")
        maintTimeColumn.title = qsTr("维保时间")
        state.title = qsTr("状态现象")
        methods.title = qsTr("维保方法")
        contentDesc.title = qsTr("内容描述")
        maintEngineer.title = qsTr("维保员")
        floor.title = qsTr("楼层")
        position.title = qsTr("部位")
        system.title = qsTr("系统")
        buildingName.title = qsTr("建筑名称")
        personOnDuty.title = qsTr("值班人员")
        operator.title = qsTr("操作人员")
    }
}
