import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import architePlanView 1.0
import "../infoSetting"

    Item
    {
       // anchors.fill: parent
    Row {
        id: deviceStateQuery
        spacing: 5
        //columns: 14
        Text {
            id: extNum
            text: qsTr("分机号")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }

        TextField {
            id: extNumTextField
            width: 80
            height: 30
        }

        Text {
            id: loopNum
            text: qsTr("回路号")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }

        TextField {
            id: loopNumTextField
            width: 80
            height: 30
        }

        Text {
            id: addressNum
            text: qsTr("地址号")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }

        TextField {
            id: addressNumTextField
            width: 80
            height: 30
        }

        Text {
            text: qsTr("网络号")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }

        TextField {
            id: networkNumTextField
            width: 80
            height: 30
        }

        Text {
            text: qsTr("电源地址")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }

        TextField {
            id: powerAddrTextField
            width: 100
            height: 30
        }

        Text {
            text: qsTr("设备")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }

        TextField {
            id: deviceTextField
            width: 100
            height: 30
        }

        NaviButton {
            id: stateQueryBtn
            text: qsTr("查询")
            height: 30
            width: 80
            onClicked: {
                deviceStateModel.sqlCommit(
                            String(
                                "select extNum ,loopNum,addrNum,networkNum,powerAddr,manufacturers,deviceInstallTime,periodOfValidity,deviceNum,equipmentModel ,currentState ,operator from ItemInfo where %1").arg(
                                selectInfo()))
            }
        }

        NaviButton {
            id: stateQueryAllBtn
            text: qsTr("查询所有")
            height: 30
            width: 100
            onClicked: {
                deviceStateModel.sqlCommit(
                            "select extNum,loopNum,addrNum,networkNum,powerAddr,manufacturers,deviceInstallTime,periodOfValidity,deviceNum,equipmentModel ,currentState ,operator from ItemInfo")
            }
        }
    }

    Controls1_4.TableView {

        width: parent.width
        anchors.topMargin: 10
        clip: true
        anchors.bottom: parent.bottom
        anchors.top: deviceStateQuery.bottom
        Controls1_4.TableViewColumn {
            role: qsTr("extNum")
            title: qsTr("分机号")
            width: 60
            //resizable: true
        }
        Controls1_4.TableViewColumn {
            role: qsTr("loopNum")
            title: qsTr("回路号")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: qsTr("addrNum")
            title: qsTr("地址号")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: qsTr("networkNum")
            title: qsTr("网络号")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: qsTr("powerAddr")
            title: qsTr("电源地址")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: qsTr("manufacturers")
            title: qsTr("制造商")
            width: 150
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: qsTr("deviceInstallTime")
            title: qsTr("安装时间")
            width: 100
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: qsTr("periodOfValidity")
            title: qsTr("有效期")
            width: 100
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: qsTr("deviceNum")
            title: qsTr("设备编码")
            width: 100
            // resizable: true
        }

        Controls1_4.TableViewColumn {
            role: qsTr("deviceName")
            title: qsTr("设备")
            width: 100
            // resizable: true
        }

        Controls1_4.TableViewColumn {
            role: qsTr("alarmState")
            title: qsTr("状态")
            width: 100
            // resizable: true
        }

        Controls1_4.TableViewColumn {
            role: qsTr("operator")
            title: qsTr("操作员")
            width: 100
            //resizable: true
        }
        model: deviceStateModel
        onDoubleClicked: {
            var curExtNum = new String
            var curLoopNum = new String
            var curAddrNum = new String
            var curNetworkNum = new String
            var curPowerAddr = new String
            curExtNum = deviceStateModel.getValue(row, "extNum")
            curLoopNum = deviceStateModel.getValue(row, "loopNum")
            curAddrNum = deviceStateModel.getValue(row, "addrNum")
            curNetworkNum = deviceStateModel.getValue(row, "networkNum")
            curPowerAddr = deviceStateModel.getValue(row, "powerAddr")
            ArchitePlanView.toArchitePlan(curExtNum, curLoopNum, curAddrNum,
                                          curNetworkNum,curPowerAddr)
        }
    }
    QmlTableModel {
        id: deviceStateModel
        dbDriver: qsTr("QSQLITE")
        dbName: ArchitePlanView.architeInfoDbName()
        dbConnectionName: "deviceState"
        dbPort: 8888
        roleNameList: ["extNum", "loopNum", "addrNum", "networkNum", "powerAddr","manufacturers","deviceInstallTime","periodOfValidity", "deviceNum", "deviceName", "alarmState", "operator"]
        titleList: ["分机号", "回路号", "地址号", "网络号","电源地址", "制造商", "安装时间","有效期", "设备编码", "设备", "状态", "操作员"]
    }

    Component.onCompleted: {

        deviceStateModel.setDbOpen(true)
        deviceStateModel.sqlCommit(
                    "select extNum,loopNum,addrNum,networkNum,powerAddr,manufacturers,deviceInstallTime,periodOfValidity,deviceNum,equipmentModel,currentState,operator from ItemInfo")
    }

    function selectInfo() {
        var info = new String
        if (loopNumTextField.text.length > 0) {
            info += (qsTr("loopNum=") + "'" + loopNumTextField.text + "'")
        }

        if (addressNumTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("addrNum=") + "'" + addressNumTextField.text + "'")
        }

        if (extNumTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("extNum=") + "'" + extNumTextField.text + "'")
        }

        if (networkNumTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("networkNum=") + "'" + networkNumTextField.text + "'")
        }

        if (powerAddrTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("powerAddr=") + "'" + powerAddrTextField.text + "'")
        }

        if (deviceTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("equipmentModel=") + "'" + deviceTextField.text + "'")
        }
        return info
    }

    function saveToPdf() {
        deviceStateModel.saveToPdf()
    }
    function startPrint() {
        deviceStateModel.startPrint()
    }
    function printPreview() {
        deviceStateModel.printPreview()
    }
}
