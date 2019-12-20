import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import crtWidget 1.0
import QtQuick.Dialogs 1.2
import architePlanView 1.0
import "../infoSetting"
Item {

    Grid {
        id: row
        columns: 8
        spacing: 5
        Text {
            text: qsTr("分机号")
            height: 30
            horizontalAlignment: TextEdit.AlignLeft
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: extNumTextField
            width: 100
            height: 30
        }

        Text {
            text: qsTr("回路号")
            height: 30
            horizontalAlignment: TextEdit.AlignLeft
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: loopNumTextField
            width: 100
            height: 30
        }

        Text {

            text: qsTr("地址号")
            height: 30
            horizontalAlignment: TextEdit.AlignLeft
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: addrNumTextField
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
            text: qsTr("当前通道")
            height: 30
            horizontalAlignment: TextEdit.AlignLeft
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: curChannelTextField
            height: 30
            width: 100
        }

        Text {
            text: qsTr("模拟量类型")
            height: 30
            horizontalAlignment: TextEdit.AlignLeft
            verticalAlignment: TextEdit.AlignVCenter
        }
//        ComboBox {
//            id: analogComboBox
//            height: 30
//            width: 150
//            model: [qsTr("高度(m)"), qsTr("温度(℃)"), qsTr(
//                    "压力(Mpa)"), qsTr("压力(Kpa)"), qsTr(
//                    "气体浓度(%LEL)"), qsTr("气体浓度(PPM)"), qsTr(
//                    "气体浓度(%V/V)"), qsTr("气体浓度(KPPM)"), qsTr(
//                    "气体浓度(Mg/m^3)"), qsTr("时间(s)"), qsTr(
//                    "电压(V)"), qsTr("电流(A)"), qsTr(
//                    "流量(L/s)"), qsTr("风量(m^3/min)"), qsTr(
//                    "风速(m/s)"), qsTr("剩余电流(mA)"), qsTr(
//                    "烟参量"), qsTr("距离(m)"), qsTr(
//                    "交流电流(mA)"), qsTr("直流电流(mA)"), qsTr(
//                    "交流电压(V)"), qsTr("直流电压(V)")]
//        }

        NaviButton {
            id: infoQueryBtn
            text: qsTr("查询")
            height: 30
            width: 80
            onClicked: {
               // console.log(networkNumTextField)

                Crt.sendAnalogCommand(
                            parseInt(networkNumTextField.text)&0xff, parseInt(
                                extNumTextField.text) & 0xff,
                            parseInt(loopNumTextField.text) & 0xff, parseInt(
                                addrNumTextField.text) & 0xff,
                            parseInt(curChannelTextField.text) & 0xff, 1)
                infoListModel.sqlCommit(
                            String("select * from AnalogInfo where %1").arg(
                                selectInfo()))
            }
        }

        NaviButton {
            id: infoQueryAllBtn
            text: qsTr("查询所有")
            height: 30
            width: 100
            onClicked: {
                Crt.sendAnalogCommand(
                            parseInt(networkNumTextField.text)&0xff, parseInt(
                                extNumTextField.text) & 0xff,
                            parseInt(loopNumTextField.text) & 0xff, parseInt(
                                addrNumTextField.text) & 0xff,
                            0, 1)
                infoListModel.sqlCommit("select * from AnalogInfo")
            }
        }

        NaviButton {
            id: infoDeleteBtn
            text: qsTr("删除")
            height: 30
            width: 80

            onClicked: {
                var info = new String
                info = selectInfo()
                if (info.length > 0) {
                    infoListModel.sqlCommit(
                                String("delete  from AnalogInfo where %1").arg(
                                    info))
                }
            }
        }

        NaviButton {
            id: infoClearBtn
            text: qsTr("清空")
            height: 30
            width: 80
            onClicked: {
                messageDialog.open()
            }
        }
    }

    Controls1_4.TableView {
        id: infoTableView
        anchors.top: row.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        width: row.width
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
            role: "networkNum"
            title: qsTr("网络号")
            width: 60
        }
        Controls1_4.TableViewColumn {
            role: "curChannel"
            title: qsTr("当前通道")
            width: 60
        }

        Controls1_4.TableViewColumn {
            role: "analogType"
            title: qsTr("模拟量类型")
            width: 100
        }

        Controls1_4.TableViewColumn {
            role: "result"
            title: qsTr("结果")
            width: 100
        }

        Controls1_4.TableViewColumn {
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
            curExtNum = infoListModel.getValue(row, "extNum")
            curLoopNum = infoListModel.getValue(row, "loopNum")
            curAddrNum = infoListModel.getValue(row, "addrNum")
            curNetworkNum = infoListModel.getValue(row, "networkNum")
            ArchitePlanView.toArchitePlan(curExtNum, curLoopNum, curAddrNum,
                                          curNetworkNum)
        }
    }
    QmlTableModel {
        id: infoListModel
        dbDriver: qsTr("QSQLITE")
        dbName: Crt.alarmInfoDbName()
        dbConnectionName: "analogInfoDb"
        dbPort: 9876
        roleNameList: ["extNum", "loopNum", "addrNum", "networkNum", "curChannel", "analogType", "result", "time"]
        titleList: ["分机号", "回路号", "地址号", "网络号", "当前通道", "模拟量类型", "结果", "时间"]
    }
    function selectInfo() {
        var info = new String
        if (extNumTextField.text.length > 0) {
            info += (qsTr("分机号=") + "'" + extNumTextField.text + "'")
        }

        if (loopNumTextField.text.length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("回路号=") + "'" + loopNumTextField.text + "'")
        }
        if (addrNumTextField.text.length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("地址号=") + "'" + addrNumTextField.text + "'")
        }

        if (networkNumTextField.text.length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("网络号=") + "'" + networkNumTextField.text + "'")
        }

        if (curChannelTextField.text.length > 0) {

            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("当前通道=") + "'" + curChannelTextField.text + "'")
        }

//        if (analogComboBox.currentText.length > 0) {

//            if (analogComboBox.currentText != qsTr("全部")) {
//                if (info.length > 0) {
//                    info += " and "
//                }
//                info += (qsTr(
//                             "模拟量类型=") + "'" + analogComboBox.currentText + "'")
//            }
//        }

        return info
    }

    function saveToPdf() {
        infoListModel.saveToPdf()
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
        infoListModel.sqlCommit("select * from AnalogInfo")
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("警告提示")
        text: qsTr("如果信息超过1000条，或者没有正在报警的信息可以清除。注意信息的保存和备份。")
        icon: StandardIcon.Warning
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {
            infoListModel.sqlCommit("delete from AnalogInfo")
        }
        onNo: {
            close()
        }
    }
}
