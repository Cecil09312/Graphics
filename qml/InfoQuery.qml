import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4 as Controls1_4

Item {

    //    readonly property var alarmQueryModel: ["num", "deviceSys", "extNum", "loopNum", "addrNum", "alarmEvent", "deviceNum", "deviceType", "currentAlarmState", "alarmTime", "receiveAlarmTime", "protectedSectionName", "buildName", "floor", "deviceLocation", "watcher", "remark"]

    //    property ListModel alarmQueryModel: ListModel {
    //        id: alarmQueryListModel
    //        dynamicRoles: true
    //    }
    SwipeView {
        id: swipView
        currentIndex: 1
        anchors.fill: parent
        Item {
            id: alarmInfo
            GroupBox {
                id: alarmInfoGroupBox
                title: qsTr("报警记录查询")
                anchors.fill: parent
                anchors.bottomMargin: 40
                anchors.topMargin: 40
                Row {
                    id: alarmInfoQuery
                    spacing: 5
                    Text {
                        id: alarmInfoNum
                        text: qsTr("编号")
                        height: 40
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }
                    TextField {
                        id: alarmInfoNumTextField
                        width: 100
                    }

                    Text {
                        id: alarmInfoDeviceNum
                        text: qsTr("设备(设施)所属编号")
                        height: 40
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }
                    TextField {
                        id: alarmInfoDeviceTextField
                        width: 100
                    }

                    Text {
                        id: alarmInfoExtNum
                        text: qsTr("分机号")
                        height: 40
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }
                    TextField {
                        id: alarmInfoExtNumTextField
                        width: 100
                    }

                    Text {
                        id: alarmInfoLoopNum
                        text: qsTr("回路号")
                        height: 40
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }
                    TextField {
                        id: alarmInfoLoopNumTextField
                        width: 100
                    }

                    Text {
                        id: alarmInfoAddNum
                        text: qsTr("地址号")
                        height: 40
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }
                    TextField {
                        id: alarmInfoAddNumTextField
                        width: 100
                    }

                    Text {
                        id: alarmInfoAlarmEvent
                        text: qsTr("报警事件")
                        height: 40
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }
                    TextField {
                        id: alarmInfoAlarmEventTextField
                        width: 100
                    }
                    Button {
                        id: alarmInfoQueryBtn
                        text: qsTr("查询")
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
                        //resizable: true
                    }
                    Controls1_4.TableViewColumn {
                        role: "deviceSys"
                        title: qsTr("设备(设施)所属系统")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        role: "extNum"
                        title: qsTr("分机号")
                        width: 120
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        role: "loopNum"
                        title: qsTr("回路号")
                        width: 120
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        role: "addrNum"
                        title: qsTr("地址号")
                        width: 120
                        // resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        role: "alarmEvent"
                        title: qsTr("报警事件")
                        width: 120
                        // resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        role: "productNum"
                        title: qsTr("设备(设施)产品编号")
                        width: 120
                        // resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        role: "deviceType"
                        title: qsTr("设备(设施)型号")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        role: "currentAlarmState"
                        title: qsTr("报警当前状态")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        role: "alarmTime"
                        title: qsTr("报警时间")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        role: "receiveAlarmTime"
                        title: qsTr("报警收到时间")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        role: "rebackAlarmTime"
                        title: qsTr("恢复正常状态时间")
                        width: 60
                        //resizable: true
                    }
                    Controls1_4.TableViewColumn {
                        role: "protectedSectionName"
                        title: qsTr("总保护区域(总图)名称")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        role: "buildName"
                        title: qsTr("建筑设施名称")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        role: "floor"
                        title: qsTr("设备(设施)所在楼层")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        role: "deviceLocation"
                        title: qsTr("设备(设施)所处部位")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        role: "watcher"
                        title: qsTr("值班员")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        role: "remark"
                        title: qsTr("备注")
                        width: 60
                        //resizable: true
                    }
                    model: ListModel {
                        id: alarmInfoListModel
                    }
                }
            }
        }
        Item {
            id: operaEvent
            GroupBox {
                id: operaEventGroupBox
                title: qsTr("操作事件记录查询")
                anchors.fill: parent
                anchors.bottomMargin: 40
                anchors.topMargin: 40
                Row {
                    id: operaEventQuery
                    spacing: 5
                    Text {
                        id: num
                        text: qsTr("编号")
                        height: 40
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }
                    TextField {
                        id: numTextField
                        width: 100
                    }

                    Text {
                        id: productNum
                        text: qsTr("产品编号")
                        height: 40
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }
                    TextField {
                        id: productNumTextField
                        width: 100
                    }

                    Text {
                        id: date
                        text: qsTr("维保日期")
                        height: 40
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }
                    TextField {
                        id: dateTextField
                        width: 100
                    }

                    Button {
                        id: operaEventQueryBtn
                        text: qsTr("查询")
                    }
                }

                Controls1_4.TableView {
                    anchors.top: operaEventQuery.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 5
                    Controls1_4.TableViewColumn {
                        // role: qsTr("系统类型")
                        title: qsTr("编号")
                        width: 60
                        //resizable: true
                    }
                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("产品编码")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("维保日期")
                        width: 120
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("事出原因")
                        width: 120
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("当前状态")
                        width: 120
                        // resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("解决方法")
                        width: 120
                        // resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("内容描述")
                        width: 120
                        // resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("操作员")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("备注")
                        width: 60
                        //resizable: true
                    }

                    model: ListModel {
                        id: operaEventQueryModel
                    }
                }
            }
        }
        Item {
            id: deviceState
            GroupBox {
                id: deviceStateGroupBox
                title: qsTr("设备及设施状态查询")
                anchors.fill: parent
                anchors.bottomMargin: 40
                anchors.topMargin: 40
                Row {
                    id: deviceStateQuery
                    spacing: 5
                    Text {
                        id: sysName
                        text: qsTr("系统名称")
                        height: 40
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }

                    ComboBox {
                        id: sysComboBox
                    }
                    Text {
                        id: deviceNum
                        text: qsTr("设备号")
                        height: 40
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }

                    TextField {
                        id: deviceNumTextField
                        width: 100
                        // height: 40
                    }

                    Text {
                        id: loopNum
                        text: qsTr("回路号")
                        height: 40
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }

                    TextField {
                        id: loopNumTextField
                        width: 100
                    }

                    Text {
                        id: addressNum
                        text: qsTr("地址号")
                        height: 40
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }

                    TextField {
                        id: addressNumTextField
                        width: 100
                    }

                    Button {
                        id: stateQueryBtn
                        text: qsTr("查询")
                    }
                }

                Controls1_4.TableView {
                    anchors.top: deviceStateQuery.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 5
                    Controls1_4.TableViewColumn {
                        // role: qsTr("系统类型")
                        title: qsTr("分机号")
                        width: 60
                        //resizable: true
                    }
                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("回路号")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("地址号")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("自动启动是否有效")
                        width: 120
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("手动启动是否有效")
                        width: 120
                        // resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("是否启动")
                        width: 60
                        // resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("是否火警")
                        width: 60
                        // resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("是否监管")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("是否屏蔽")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("是否反馈")
                        width: 60
                        // resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("是否故障")
                        width: 60
                        //resizable: true
                    }

                    Controls1_4.TableViewColumn {
                        // role: qsTr("主电是否故障")
                        title: qsTr("备注")
                        width: 60
                        // resizable: true
                    }

                    model: ListModel {
                        id: deviceStateModel
                    }
                }
            }
        }
    }

    PageIndicator {
        id: indicator
        count: swipView.count
        currentIndex: swipView.currentIndex
        anchors.bottom: swipView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Component.onCompleted: {

        /*alarmInfoListModel.append({
                                      num: "11",
                                      deviceSys: "11",
                                      extNum: "11",
                                      loopNum: "11",
                                      addrNum: "11",
                                      alarmEvent: "11",
                                      deviceNum: "11",
                                      deviceType: "11",
                                      currentAlarmState: "11",
                                      alarmTime: "11",
                                      receiveAlarmTime: "11",
                                      rebackAlarmTime: "11",
                                      protectedSectionName: "11",
                                      buildName: "11",
                                      floor: "11",
                                      deviceLocation: "11",
                                      watcher: "11",
                                      remark: "11"
                                  })*/
        /* alarmQueryModel. */

        console.log(alarmInfoTableView.rowCount)
    }
}
