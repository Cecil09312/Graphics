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
            selectionColor: "blue"
            selectedTextColor: "white"
            selectByMouse: true
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
            selectionColor: "blue"
            selectedTextColor: "white"
            selectByMouse: true
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
            selectionColor: "blue"
            selectedTextColor: "white"
            selectByMouse: true
        }

        Text {
            id:networkNum
            text: qsTr("网络号")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }

        TextField {
            id: networkNumTextField
            width: 80
            height: 30
            selectionColor: "blue"
            selectedTextColor: "white"
            selectByMouse: true
        }

        Text {
            id:powerAddr
            text: qsTr("电源地址")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
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
            id:deviceTxt
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
                                "select extNum ,loopNum,addrNum,networkNum,powerAddr,manufacturers,deviceInstallTime,periodOfValidity,deviceNum,equipmentModel ,operator from ItemInfo where %1").arg(
                                selectInfo()))
                deviceStateView.resizeColumnsToContents()
            }
        }

        NaviButton {
            id: stateQueryAllBtn
            text: qsTr("查询所有")
            height: 30
            width: 100
            onClicked: {
                deviceStateModel.sqlCommit(
                            "select extNum,loopNum,addrNum,networkNum,powerAddr,manufacturers,deviceInstallTime,periodOfValidity,deviceNum,equipmentModel ,operator from ItemInfo")
                deviceStateView.resizeColumnsToContents()
            }
        }
    }

    Controls1_4.TableView {

        id:deviceStateView
        width: parent.width
        anchors.topMargin: 10
        clip: true
        anchors.bottom: parent.bottom
        anchors.top: deviceStateQuery.bottom
        Controls1_4.TableViewColumn {
            id:extNumColumn
            role: "extNum"
            title: qsTr("分机号")
            width: 60
            //resizable: true
        }
        Controls1_4.TableViewColumn {
            id:loopNumColumn
            role: "loopNum"
            title: qsTr("回路号")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:addeNumColumn
            role: "addrNum"
            title: qsTr("地址号")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:networkNumColumn
            role: "networkNum"
            title: qsTr("网络号")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:powerAddrColumn
            role: "powerAddr"
            title: qsTr("电源地址")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:manufacturers
            role: "manufacturers"
            title: qsTr("制造商")
            width: 150
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:installTime
            role: "deviceInstallTime"
            title: qsTr("安装时间")
            width: 100
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:periodOfValidity
            role: "periodOfValidity"
            title: qsTr("有效期")
            width: 100
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            id:deviceNum
            role: "deviceNum"
            title: qsTr("设备编码")
            width: 100
            // resizable: true
        }

        Controls1_4.TableViewColumn {
            id:deviceName
            role: "deviceName"
            title: qsTr("设备")
            width: 100
            // resizable: true
        }



        Controls1_4.TableViewColumn {
            id:operator
            role: "operator"
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
        dbDriver: "QSQLITE"
        dbName: ArchitePlanView.architeInfoDbName()
        dbConnectionName: "deviceState"
        dbPort: 8888
        roleNameList: ["extNum", "loopNum", "addrNum", "networkNum", "powerAddr","manufacturers","deviceInstallTime","periodOfValidity", "deviceNum", "deviceName", "operator"]
        titleList: [qsTr("分机号"), qsTr("回路号"), qsTr("地址号"), qsTr("网络号"),qsTr("电源地址"), qsTr("制造商"), qsTr("安装时间"),qsTr("有效期"), qsTr("设备编码"), qsTr("设备"), qsTr("操作员")]
    }

    Component.onCompleted: {

        deviceStateModel.setDbOpen(true)
       // deviceStateModel.sqlCommit(
                  //  "select extNum,loopNum,addrNum,networkNum,powerAddr,manufacturers,deviceInstallTime,periodOfValidity,deviceNum,equipmentModel,operator from ItemInfo")
    }

    function selectInfo() {
        var info = new String
        if (loopNumTextField.text.replace(/\s+/g,"").length > 0) {
            info += ("loopNum=" + "'" + loopNumTextField.text.replace(/\s+/g,"") + "'")
        }

        if (addressNumTextField.text.replace(/\s+/g,"").length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += ("addrNum=" + "'" + addressNumTextField.text.replace(/\s+/g,"") + "'")
        }

        if (extNumTextField.text.replace(/\s+/g,"").length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += ("extNum=" + "'" + extNumTextField.text.replace(/\s+/g,"") + "'")
        }

        if (networkNumTextField.text.replace(/\s+/g,"").length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += ("networkNum=" + "'" + networkNumTextField.text.replace(/\s+/g,"") + "'")
        }

        if (powerAddrTextField.text.replace(/\s+/g,"").length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += ("powerAddr="+ "'" + powerAddrTextField.text.replace(/\s+/g,"") + "'")
        }

        if (deviceTextField.text.replace(/\s+/g,"").length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += ("equipmentModel=" + "'" + deviceTextField.text.replace(/\s+/g,"") + "'")
        }
        return info
    }

    function saveToPdf(fileName) {
        deviceStateModel.saveToPdf(fileName)
    }
    function startPrint() {
        deviceStateModel.startPrint()
    }
    function printPreview() {
        deviceStateModel.printPreview()
    }
    function retranslate()
    {
        extNum.text = qsTr("分机号")
        loopNum.text =  qsTr("回路号")
        addressNum.text = qsTr("地址号")
        networkNum.text = qsTr("网络号")
        powerAddr.text = qsTr("电源地址")
        deviceTxt.text = qsTr("设备")
        stateQueryBtn.text = qsTr("查询")
        stateQueryAllBtn.text = qsTr("查询所有")
        extNumColumn.title = qsTr("分机号")
        loopNumColumn.title = qsTr("回路号")
        addeNumColumn.title = qsTr("地址号")
        networkNumColumn.title = qsTr("网络号")
        powerAddrColumn.title = qsTr("电源地址")
        manufacturers.title = qsTr("制造商")
        installTime.title = qsTr("安装时间")
        periodOfValidity.title = qsTr("有效期")
        deviceNum.title = qsTr("设备编码")
        deviceName.title = qsTr("设备")
        operator.title = qsTr("操作员")

    }
}
