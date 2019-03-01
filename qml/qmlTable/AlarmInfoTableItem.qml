import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
Item {
    id: alarmInfoItem
    //title: qsTr("报警记录查询")
   // anchors.fill: parent
    anchors.bottomMargin: 40
    anchors.topMargin: 20
    Row {
        id: alarmInfoQuery
        spacing: 5
        Text {
            id: alarmInfoExtNum
            text: qsTr("分机号")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: alarmInfoExtNumTextField
            width: 100
            height: 30
        }

        Text {
            id: alarmInfoLoopNum
            text: qsTr("回路号")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: alarmInfoLoopNumTextField
            width: 100
            height: 30
        }

        Text {
            id: alarmInfoAddNum
            text: qsTr("地址号")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: alarmInfoAddNumTextField
            width: 100
            height: 30
        }

        Text {
            id: alarmInfoAlarmType
            text: qsTr("报警类型")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: alarmInfoAlarmTypeTextField
            height: 30
            width: 100
        }
        Button {
            id: alarmInfoQueryBtn
            text: qsTr("查询")
            height: 30
            onClicked: {
                // console.log(selectInfo())
            }
        }
    }

    Controls1_4.TableView {
        id: alarmInfoTableView
        anchors.top: alarmInfoQuery.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 5
        Controls1_4.TableViewColumn {
            role: "num"
            title: qsTr("编号")
            width: 60
        }

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
            role: "deviceSys"
            title: qsTr("设备所属系统")
            width: 100
        }

        Controls1_4.TableViewColumn {
            role: "alarmType"
            title: qsTr("报警类型")
            width: 100
        }

        Controls1_4.TableViewColumn {
            role: "productNum"
            title: qsTr("设备产品编号")
            width: 100
        }

        Controls1_4.TableViewColumn {
            role: "deviceType"
            title: qsTr("设备设施型号")
            width: 100
        }

        Controls1_4.TableViewColumn {
            role: "currentAlarmState"
            title: qsTr("报警当前状态")
            width: 100
        }

        Controls1_4.TableViewColumn {
            role: "alarmTime"
            title: qsTr("报警时间")
            width: 100
        }

        Controls1_4.TableViewColumn {
            role: "receiveAlarmTime"
            title: qsTr("报警收到时间")
            width: 100
        }

        Controls1_4.TableViewColumn {
            role: "rebackAlarmTime"
            title: qsTr("恢复正常状态时间")
            width: 120
        }
        Controls1_4.TableViewColumn {
            role: "protectedSectionName"
            title: qsTr("总保护区域名称")
            width: 100
        }

        Controls1_4.TableViewColumn {
            role: "buildName"
            title: qsTr("建筑设施名称")
            width: 100
        }

        Controls1_4.TableViewColumn {
            role: "floor"
            title: qsTr("设备所在楼层")
            width: 100
        }

        Controls1_4.TableViewColumn {
            role: "deviceLocation"
            title: qsTr("设备所处部位")
            width: 100
        }

        Controls1_4.TableViewColumn {
            role: "watcher"
            title: qsTr("值班员")
            width: 100
        }

        Controls1_4.TableViewColumn {
            role: "remark"
            title: qsTr("备注")
            width: 100
        }
        model: alarmInfoListModel
    }
    QmlTableModel {
        id: alarmInfoListModel
        //        dbDriver: qsTr("QSQLITE")
        //        dbName: "C:/Users/1/Desktop/mySqlite.db"
        //        dbUser: "sss"
        //        dbPassword: "www"
        //        dbConnectionName: "defaultName"
        //        dbPort: 1111
        roleNameList: ["num", "extNum", "loopNum", "addrNum", "deviceSys", "alarmType", "productNum", "deviceType", "currentAlarmState", "alarmTime", "receiveAlarmTime", "rebackAlarmTime", "protectedSectionName", "buildName", "floor", "deviceLocation", "watcher", "remark"]
    }
    function selectInfo() {
        var info = new String
        if (alarmInfoExtNumTextField.text.length > 0) {
            info += (qsTr("分机号=") + alarmInfoExtNumTextField.text)
        }

        if (alarmInfoLoopNumTextField.text.length > 0) {
            if (info.length > 0) {
                info += ","
            }
            info += (qsTr("回路号=") + alarmInfoLoopNumTextField.text)
        }
        if (alarmInfoAddNumTextField.text.length > 0) {
            if (info.length > 0) {
                info += ","
            }
            info += (qsTr("地址号=") + alarmInfoAddNumTextField.text)
        }

        if (alarmInfoAlarmTypeTextField.text.length > 0) {
            if (info.length > 0) {
                info += ","
            }
            info += (qsTr("报警类型=") + alarmInfoAlarmTypeTextField.text)
        }
        return info
    }

    Component.onCompleted: {


        // alarmInfoListModel.setDbOpen(true)
        // alarmInfoListModel.sqlCommit("select *from COMPANY")
    }
}
