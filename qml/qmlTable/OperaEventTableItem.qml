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
        Row {
            id: operaEventQuery
            spacing: 5
            anchors.topMargin: 20

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
                text: qsTr("时间")
                height: 30
                horizontalAlignment: TextEdit.AlignHCenter
                verticalAlignment: TextEdit.AlignVCenter
            }
            TextField {
                id: startTimeTextField
                width: 150
                height: 30
                placeholderText: qsTr("如:2019/01/01 00:00:00")
            }

            Text {

                text: qsTr("到")
                height: 30
                horizontalAlignment: TextEdit.AlignHCenter
                verticalAlignment: TextEdit.AlignVCenter
            }
            TextField {
                id: endTimeTextField
                width: 150
                height: 30
                placeholderText: qsTr("如:2050/01/01 00:00:00")
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

        if (startTimeTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("时间 >=") + "'" + startTimeTextField.text + "'")
        }

        if (endTimeTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("时间 <=") + "'" + endTimeTextField.text + "'")
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
