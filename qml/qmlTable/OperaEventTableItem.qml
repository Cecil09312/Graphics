import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import operatorInfo 1.0
import "../infoSetting"

Item
{
    // anchors.fill: parent
    // anchors.top: parent.top
    //anchors.bottom: parent.bottom
    Column {
        id: operaEventQuery
        spacing: 5
        anchors.topMargin: 20
        //columns: 18

        Row
        {
            spacing: 5
            Text {
                text: qsTr("用户名")
                height: 30
                horizontalAlignment: TextEdit.AlignHCenter
                verticalAlignment: TextEdit.AlignVCenter
            }
            TextField {
                id: userNameTextField
                width: 80
                height: 30
            }
            Text {
                id: event
                text: qsTr("事件")
                height: 30
                horizontalAlignment: TextEdit.AlignHCenter
                verticalAlignment: TextEdit.AlignVCenter
            }
            TextField {
                id: eventTextField
                width: 80
                height: 30
            }

            Text {
                id: startTime
                text: qsTr(" 时间:")
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
                id:operaEventStartYearSpinBox

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
                id:operaEventStartMonthSpinBox

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
                id:operaEventStartDateSpinBox

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
                id:operaEventEndYearSpinBox

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
                id:operaEventEndMonthSpinBox

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
                id:operaEventEndDateSpinBox

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
                    operaEventQueryModel.sqlCommit(
                                String("select * from operator where %1").arg(
                                    selectInfo()))
                    console.log(String("select * from operator where %1").arg(
                                    selectInfo()))
                }
            }

            NaviButton {
                id: operaEventQueryAllBtn
                text: qsTr("查询所有")
                height: 30
                width: 100
                onClicked: {
                    operaEventQueryModel.sqlCommit("select * from operator")
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
                    if (info.length > 0) {
                        operaEventQueryModel.sqlCommit(
                                    String("delete from operator where %1").arg(
                                        info))
                    }
                }
            }

            NaviButton {
                id: operaEventQueryClearBtn
                text: qsTr("清空")
                height: 30
                width: 80
                onClicked: {
                    operaEventQueryModel.sqlCommit("delete from operator")
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
            role: "userName"
            title: qsTr("用户名")
            width: 100
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "userRight"
            title: qsTr("用户权限")
            width: 100
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "event"
            title: qsTr("事件")
            width: 150
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "result"
            title: qsTr("结果")
            width: 400
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "time"
            title: qsTr("时间")
            width: 200
            //resizable: true
        }

        model: operaEventQueryModel


    }

    QmlTableModel {
        id: operaEventQueryModel
        dbDriver: qsTr("QSQLITE")
        dbName: OperatorInfo.operatorInfoDbPath()
        dbConnectionName: "operaEvent"
        dbPort: 6888
        roleNameList: ["userName", "userRight", "event", "result", "time"]
        titleList: [qsTr("用户名"), qsTr("用户权限"), qsTr("事件"), qsTr(
                "结果"), qsTr("时间")]
    }

    Component.onCompleted: {
        operaEventQueryModel.setDbOpen(true)
        operaEventQueryModel.sqlCommit("select * from operator")
    }

    function selectInfo() {
        var info = new String

        if (userNameTextField.text.length > 0) {
            info += (qsTr("用户名=") + "'" + userNameTextField.text + "'")
        }

        if (eventTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("事件=") + "'" + eventTextField.text + "'")
        }


        var startDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3 0:0:0").arg(operaEventStartYearSpinBox.value).arg(operaEventStartMonthSpinBox.value).arg(operaEventStartDateSpinBox.value),"yyyy/M/d h:m:s");
        var startDate = new Date(startDateValue)
        var startDateStr=Qt.formatDateTime(startDate,"yyyy/MM/dd hh:mm:ss");

        if (startDateStr.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("时间 >=") + "'" + startDateStr + "'")
        }

        var endDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3 23:59:59").arg(operaEventEndYearSpinBox.value).arg(operaEventEndMonthSpinBox.value).arg(operaEventEndDateSpinBox.value),"yyyy/M/d h:m:s");
        var endDate = new Date(endDateValue)
        var endDateStr=Qt.formatDateTime(endDate,"yyyy/MM/dd hh:mm:ss");

        if (endDateStr.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("时间 <=") + "'" + endDateStr + "'")
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
