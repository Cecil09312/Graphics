import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import architePlanView 1.0

Item {
    // id: deviceStateItem
    // title: qsTr("设备及设施状态查询")
    // anchors.fill: parent
    anchors.bottomMargin: 40
    anchors.topMargin: 40
    Row {
        id: deviceStateQuery
        spacing: 5
        Text {
            id: extNum
            text: qsTr("分机号")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }

        TextField {
            id: extNumTextField
            width: 100
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
            width: 100
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
            width: 100
            height: 30
        }

        Button {
            id: stateQueryBtn
            text: qsTr("查询")
            height: 30

            onClicked: {
                deviceStateModel.sqlCommit(
                            String(
                                "select extNum ,loopNum,addrNum ,manufacturers,periodOfValidity,deviceNum,equipmentModel ,currentState ,operator from ItemInfo where %1").arg(
                                selectInfo()))
            }
        }

        Button {
            id: stateQueryAllBtn
            text: qsTr("查询所有")
            height: 30

            onClicked: {
                deviceStateModel.sqlCommit(
                            "select extNum ,loopNum,addrNum ,manufacturers,periodOfValidity,deviceNum,equipmentModel ,currentState ,operator from ItemInfo")
            }
        }
    }

    Controls1_4.TableView {
        anchors.top: deviceStateQuery.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 2
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
            role: qsTr("manufacturers")
            title: qsTr("制造商")
            width: 120
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: qsTr("periodOfValidity")
            title: qsTr("有效期")
            width: 120
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: qsTr("deviceNum")
            title: qsTr("设备编码")
            width: 120
            // resizable: true
        }

        Controls1_4.TableViewColumn {
            role: qsTr("deviceName")
            title: qsTr("设备")
            width: 60
            // resizable: true
        }

        Controls1_4.TableViewColumn {
            role: qsTr("alarmState")
            title: qsTr("报警状态")
            width: 60
            // resizable: true
        }

        Controls1_4.TableViewColumn {
            role: qsTr("operator")
            title: qsTr("操作员")
            width: 60
            //resizable: true
        }
        model: deviceStateModel
        onDoubleClicked: {
            var curExtNum = new String
            var curLoopNum = new String
            var curAddrNum = new String
            curExtNum = deviceStateModel.getValue(row, "extNum")
            curLoopNum = deviceStateModel.getValue(row, "loopNum")
            curAddrNum = deviceStateModel.getValue(row, "addrNum")
            ArchitePlanView.toArchitePlan(curExtNum, curLoopNum, curAddrNum)
        }
    }
    QmlTableModel {
        id: deviceStateModel
        dbDriver: qsTr("QSQLITE")
        dbName: ArchitePlanView.architeInfoDbName()
        dbConnectionName: "deviceState"
        dbPort: 8888
        roleNameList: ["extNum", "loopNum", "addrNum", "manufacturers", "periodOfValidity", "deviceNum", "deviceName", "alarmState", "operator"]
        titleList: ["分机号", "回路号", "地址号", "制造商", "有效期", "设备编码", "设备", "报警状态", "操作员"]
    }

    Component.onCompleted: {

        deviceStateModel.setDbOpen(true)
        deviceStateModel.sqlCommit(
                    "select extNum ,loopNum,addrNum ,manufacturers,periodOfValidity,deviceNum,equipmentModel ,currentState ,operator from ItemInfo")
    }

    function selectInfo() {
        var info = new String
        if (loopNumTextField.text.length > 0) {
            info += (qsTr("loopNum=") + "'"+loopNumTextField.text+"'")
        }

        if (addressNumTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("addrNum=") + "'"+addressNumTextField.text+"'")
        }

        if (extNumTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("extNum=") + "'"+extNumTextField.text+"'")
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
