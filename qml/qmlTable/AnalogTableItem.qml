import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import crtWidget 1.0
import Qt.labs.platform 1.0
import architePlanView 1.0
import "../infoSetting"

Item
{
    //anchors.fill: parent
    Row
    {
        id:column
        spacing: 5
        //Row {
        //id: grid
        //columns: 10
        //rows:2
        // spacing: 5
        Text {
            id:extNumTxt
            text: qsTr("分机号")
            height: 30
            horizontalAlignment: TextEdit.AlignLeft
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: extNumTextField
            width: 60
            height: 30
            selectionColor: "blue"
            selectedTextColor: "white"
            selectByMouse: true
        }

        Text {
            id:loopNumTxt
            text: qsTr("回路号")
            height: 30
            horizontalAlignment: TextEdit.AlignLeft
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: loopNumTextField
            width: 60
            height: 30
            selectionColor: "blue"
            selectedTextColor: "white"
            selectByMouse: true
        }

        Text {
            id:addrNumTxt
            text: qsTr("地址号")
            height: 30
            horizontalAlignment: TextEdit.AlignLeft
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: addrNumTextField
            width: 60
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
            width: 60
            height: 30
            selectionColor: "blue"
            selectedTextColor: "white"
            selectByMouse: true
        }

        Text {
            id:currentChannelTxt
            text: qsTr("当前通道")
            height: 30
            horizontalAlignment: TextEdit.AlignLeft
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: curChannelTextField
            height: 30
            width: 60
            selectionColor: "blue"
            selectedTextColor: "white"
            selectByMouse: true
        }

        NaviButton {
            id: infoQueryBtn
            text: qsTr("查询")
            height: 30
            width: 80
            onClicked: {
                // console.log(networkNumTextField)

//                CrtWidget.sendAnalogCommand(
//                            parseInt(networkNumTextField.text)&0xff, parseInt(
//                                extNumTextField.text) & 0xff,
//                            parseInt(loopNumTextField.text) & 0xff, parseInt(
//                                addrNumTextField.text) & 0xff,
//                            parseInt(curChannelTextField.text) & 0xff, 1)
                infoListModel.sqlCommit(
                            String("select * from AnalogInfo where %1").arg(
                                selectInfo()))

                infoTableView.resizeColumnsToContents()
            }
        }

        NaviButton {
            id: infoQueryAllBtn
            text: qsTr("查询所有")
            height: 30
            width: 100
            onClicked: {
//                Crt.sendAnalogCommand(
//                            parseInt(networkNumTextField.text)&0xff, parseInt(
//                                extNumTextField.text) & 0xff,
//                            parseInt(loopNumTextField.text) & 0xff, parseInt(
//                                addrNumTextField.text) & 0xff,
//                            0, 1)
                infoListModel.sqlCommit("select * from AnalogInfo")
                infoTableView.resizeColumnsToContents()
            }
        }


    }


    Controls1_4.TableView {
        id: infoTableView
        width: parent.width
        anchors.topMargin: 10
        clip: true
        anchors.bottom: parent.bottom
        anchors.top: column.bottom

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
            id:currentChannel
            role: "curChannel"
            title: qsTr("当前通道")
            width: 60
        }

        Controls1_4.TableViewColumn {
            id:analogType
            role: "analogType"
            title: qsTr("模拟量类型")
            width: 100
        }

        Controls1_4.TableViewColumn {
            id:result
            role: "result"
            title: qsTr("结果")
            width: 100
        }

        Controls1_4.TableViewColumn {
            id:time
            role: "time"
            title: qsTr("时间")
            width: 150
        }

        model: infoListModel
        onDoubleClicked: {
            var curExtNum = new String
            var curLoopNum = new String
            var curAddrNum = new String
            var curNetworkNum = new String
            var curPowerAddr = new String
            curExtNum = infoListModel.getValue(row, "extNum")
            curLoopNum = infoListModel.getValue(row, "loopNum")
            curAddrNum = infoListModel.getValue(row, "addrNum")
            curNetworkNum = infoListModel.getValue(row, "networkNum")
            curPowerAddr= infoListModel.getValue(row, "powerAddr")
            ArchitePlanView.toArchitePlan(curExtNum, curLoopNum, curAddrNum,
                                          curNetworkNum,curPowerAddr)
        }
    }

    QmlTableModel {
        id: infoListModel
        dbDriver: "QSQLITE"
        dbName: Crt.alarmInfoDbName()
        dbConnectionName: "analogInfoDb"
        dbPort: 9876
        roleNameList: ["extNum", "loopNum", "addrNum", "networkNum", "curChannel", "analogType", "result", "time"]
        titleList: [qsTr("分机号"), qsTr("回路号"), qsTr("地址号"), qsTr("网络号"), qsTr("当前通道"), qsTr("模拟量类型"), qsTr("结果"), qsTr("时间")]
    }
    function selectInfo() {
        var info = new String
        if (extNumTextField.text.replace(/\s+/g,"").length > 0) {
            info += ("分机号=" + "'" + extNumTextField.text.replace(/\s+/g,"") + "'")
        }

        if (loopNumTextField.text.replace(/\s+/g,"").length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += ("回路号=" + "'" + loopNumTextField.text.replace(/\s+/g,"") + "'")
        }
        if (addrNumTextField.text.replace(/\s+/g,"").length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += ("地址号=" + "'" + addrNumTextField.text.replace(/\s+/g,"") + "'")
        }

        if (networkNumTextField.text.replace(/\s+/g,"").length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += ("网络号=" + "'" + networkNumTextField.text.replace(/\s+/g,"") + "'")
        }

        if (curChannelTextField.text.replace(/\s+/g,"").length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += ("当前通道=" + "'" + curChannelTextField.text.replace(/\s+/g,"") + "'")
        }

        return info
    }

    function saveToPdf(fileName) {
        infoListModel.saveToPdf(fileName)
    }
    function startPrint() {
        infoListModel.startPrint()
    }
    function printPreview() {
        infoListModel.printPreview()
    }

    Component.onCompleted: {

        infoTableView.positionViewAtRow(infoTableView.rowCount - 1,
                                        ListView.Contain)
        infoListModel.setDbOpen(true)
        //infoListModel.sqlCommit("select * from AnalogInfo")
    }

    function retranslate()
    {
        extNumTxt.text = qsTr("分机号")
        loopNumTxt.text = qsTr("回路号")
        addrNumTxt.text = qsTr("地址号")
        networkNumTxt.text = qsTr("网络号")
        currentChannelTxt.text = qsTr("当前通道")
        infoQueryBtn.text = qsTr("查询")
        infoQueryAllBtn.text =qsTr("查询所有")
        extNum.title = qsTr("分机号")
        loopNum.title = qsTr("回路号")
        addrNum.title = qsTr("地址号")
        networkNum.title = qsTr("网络号")
        currentChannel.title = qsTr("当前通道")
        analogType.title = qsTr("模拟量类型")
        result.title = qsTr("结果")
        time.title = qsTr("时间")
    }
}
