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
    // anchors.top: parent.top
    //anchors.bottom: parent.bottom

        Row
        {
            id: operaEventQuery
            spacing: 5
            anchors.topMargin: 20

            Text {
                id:usrNameTxt
                text: qsTr("用户名")
                height: 30
                horizontalAlignment: TextEdit.AlignHCenter
                verticalAlignment: TextEdit.AlignVCenter
            }
            TextField {
                id: userNameTextField
                width: 80
                height: 30
                selectionColor: "blue"
                selectedTextColor: "white"
                selectByMouse: true
            }
            Text {
                id: eventTxt
                text: qsTr("事件")
                height: 30
                horizontalAlignment: TextEdit.AlignHCenter
                verticalAlignment: TextEdit.AlignVCenter
            }
            TextField {
                id: eventTextField
                width: 80
                height: 30
                selectionColor: "blue"
                selectedTextColor: "white"
                selectByMouse: true
            }

            Text {
                id: startTime
                text: qsTr(" 时间:")
                height: 30
                horizontalAlignment: TextEdit.AlignHCenter
                verticalAlignment: TextEdit.AlignVCenter
            }
            Text {

                id:startTimeYearTxt
                text: qsTr("年:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }

            Controls1_4.SpinBox
            {
                id:operaEventStartYearSpinBox

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

                id:startTimeMonthTxt
                text: qsTr("月:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }

            Controls1_4.SpinBox
            {
                id:operaEventStartMonthSpinBox

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

                id:startTimeDateTxt
                text: qsTr("日:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }
            Controls1_4.SpinBox
            {
                id:operaEventStartDateSpinBox

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

                id:operaEventEndYearTxt
                text: qsTr("年:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }

            Controls1_4.SpinBox
            {
                id:operaEventEndYearSpinBox

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

                id:operaEventEndMonthTxt
                text: qsTr("月:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }

            Controls1_4.SpinBox
            {
                id:operaEventEndMonthSpinBox

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

                id:operaEventEndDateTxt
                text: qsTr("日:")
                height: 30
                horizontalAlignment: TextEdit.AlignLeft
                verticalAlignment: TextEdit.AlignVCenter
            }
            Controls1_4.SpinBox
            {
                id:operaEventEndDateSpinBox

                maximumValue: 31
                minimumValue: 1
                value: Qt.formatDate(new Date,"d")
                width: 50
                height: 25

            }

            NaviButton {
                id: operaEventQueryBtn
                text: qsTr("查询")
                height: 30
                width: 80
                onClicked: {
                    operaEventQueryModel.sqlCommit(
                                String("select * from operator where %1").arg(
                                    selectInfo()))
                  operaEventTableView.resizeColumnsToContents()
                }
            }

            NaviButton {
                id: operaEventQueryAllBtn
                text: qsTr("查询所有")
                height: 30
                width: 100
                onClicked: {
                    operaEventQueryModel.sqlCommit("select * from operator")
                    operaEventTableView.resizeColumnsToContents()
                }
            }
        }



    Controls1_4.TableView {


        id:operaEventTableView
        width: parent.width
        anchors.topMargin: 10

        clip: true
        anchors.bottom: parent.bottom
        anchors.top: operaEventQuery.bottom


        Controls1_4.TableViewColumn {
            id:userName
            role: "userName"
            title: qsTr("用户名")
            width: 100
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:userRight
            role: "userRight"
            title: qsTr("用户权限")
            width: 100
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:event
            role: "event"
            title: qsTr("事件")
            width: 150
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:result
            role: "result"
            title: qsTr("结果")
            width: 400
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:time
            role: "time"
            title: qsTr("时间")
            width: 200
            //resizable: true
        }

        model: operaEventQueryModel


    }

    QmlTableModel {
        id: operaEventQueryModel
        dbDriver: "QSQLITE"
        dbName: OperatorInfo.operatorInfoDbPath()
        dbConnectionName: "operaEvent"
        dbPort: 6888
        roleNameList: ["userName", "userRight", "event", "result", "time"]
        titleList: [qsTr("用户名"), qsTr("用户权限"), qsTr("事件"), qsTr(
                "结果"), qsTr("时间")]
    }

    Component.onCompleted: {
        setStartTime()
        setEndTime()
        operaEventQueryModel.setDbOpen(true)
       // operaEventQueryModel.sqlCommit("select * from operator")
    }

    function setStartTime()
    {
        var curYear = operaEventStartYearSpinBox.value
        var curMonth = operaEventStartMonthSpinBox.value

        var maxDate=JsDateTime.getMaxDate(curYear,curMonth)
        operaEventStartDateSpinBox.maximumValue=maxDate
        if(operaEventStartDateSpinBox.value>maxDate)
        {
            operaEventStartDateSpinBox.value = maxDate
        }
    }

    function setEndTime()
    {
        var curYear = operaEventEndYearSpinBox.value
        var curMonth = operaEventEndMonthSpinBox.value

        var maxDate=JsDateTime.getMaxDate(curYear,curMonth)
        operaEventEndDateSpinBox.maximumValue=maxDate
        if(operaEventEndDateSpinBox.value>maxDate)
        {
            operaEventEndDateSpinBox.value = maxDate
        }
    }


    function selectInfo() {
        var info = new String

        if (userNameTextField.text.length > 0) {
            info += ("用户名=" + "'" + userNameTextField.text + "'")
        }

        if (eventTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += ("事件=" + "'" + eventTextField.text + "'")
        }


        var startDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3 0:0:0").arg(operaEventStartYearSpinBox.value).arg(operaEventStartMonthSpinBox.value).arg(operaEventStartDateSpinBox.value),"yyyy/M/d h:m:s");
        var startDate = new Date(startDateValue)
        var startDateStr=Qt.formatDateTime(startDate,"yyyy/MM/dd hh:mm:ss");

        if (startDateStr.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += ("时间 >=" + "'" + startDateStr + "'")
        }

        var endDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3 23:59:59").arg(operaEventEndYearSpinBox.value).arg(operaEventEndMonthSpinBox.value).arg(operaEventEndDateSpinBox.value),"yyyy/M/d h:m:s");
        var endDate = new Date(endDateValue)
        var endDateStr=Qt.formatDateTime(endDate,"yyyy/MM/dd hh:mm:ss");

        if (endDateStr.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += ("时间 <=" + "'" + endDateStr + "'")
        }
        return info
    }

    function saveToPdf(fileName) {
        operaEventQueryModel.saveToPdf(fileName)
    }
    function startPrint() {
        operaEventQueryModel.startPrint()
    }
    function printPreview() {
        operaEventQueryModel.printPreview()
    }

    function retranslate()
    {
       usrNameTxt.text = qsTr("用户名")
        eventTxt.text = qsTr("事件")
        startTime.text = qsTr(" 时间:")
        startTimeYearTxt.text = qsTr("年:")
        startTimeMonthTxt.text = qsTr("月:")
        startTimeDateTxt.text = qsTr("日:")
        toTxt.text = qsTr(" 到:")
        operaEventEndYearTxt.text = qsTr("年:")
        operaEventEndMonthTxt.text = qsTr("月:")
        operaEventEndDateTxt.text = qsTr("日:")
        operaEventQueryBtn.text= qsTr("查询")
        operaEventQueryAllBtn.text=qsTr("查询所有")
        userName.title = qsTr("用户名")
        userRight.title = qsTr("用户权限")
        event.title = qsTr("事件")
        result.title = qsTr("结果")
        time.title = qsTr("时间")

    }
}
