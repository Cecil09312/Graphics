import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0

Item {
    id: deviceState
    //    width: 860
    //    height: 480
    antialiasing: true
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

            Text {
                id: extNum
                text: qsTr("分机号")
                height: 40
                horizontalAlignment: TextEdit.AlignHCenter
                verticalAlignment: TextEdit.AlignVCenter
            }

            TextField {
                id: extNumTextField
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
                role: qsTr("autoStartIsOk")
                title: qsTr("自动启动是否有效")
                width: 120
                //resizable: true
            }

            Controls1_4.TableViewColumn {
                role: qsTr("manualStartIsOk")
                title: qsTr("手动启动是否有效")
                width: 120
                // resizable: true
            }

            Controls1_4.TableViewColumn {
                role: qsTr("isStart")
                title: qsTr("是否启动")
                width: 60
                // resizable: true
            }

            Controls1_4.TableViewColumn {
                role: qsTr("isFireAlarm")
                title: qsTr("是否火警")
                width: 60
                // resizable: true
            }

            Controls1_4.TableViewColumn {
                role: qsTr("isSupervise")
                title: qsTr("是否监管")
                width: 60
                //resizable: true
            }

            Controls1_4.TableViewColumn {
                role: qsTr("isShield")
                title: qsTr("是否屏蔽")
                width: 60
                //resizable: true
            }

            Controls1_4.TableViewColumn {
                role: qsTr("isFeedback")
                title: qsTr("是否反馈")
                width: 60
                // resizable: true
            }

            Controls1_4.TableViewColumn {
                role: qsTr("isFault")
                title: qsTr("是否故障")
                width: 60
                //resizable: true
            }

            Controls1_4.TableViewColumn {
                role: qsTr("remark")
                title: qsTr("备注")
                width: 60
                // resizable: true
            }

            model: deviceStateModel
        }
    }

    QmlTableModel {
        id: deviceStateModel
//        dbDriver: qsTr("QSQLITE")
//        dbName: "C:/Users/1/Desktop/mySqlite.db"
//        dbUser: "sss"
//        dbPassword: "www"
//        dbConnectionName: "defaultName2"
//        dbPort: 1111
        roleNameList: ["extNum", "loopNum", "addrNum", "autoStartIsOk", "manualStartIsOk", "isStart", "isFireAlarm", "isSupervise", "isShield", "isFeedback", "isFault", "remark"]
    }

    Component.onCompleted: {
       // deviceStateModel.setDbOpen(true)
       // deviceStateModel.sqlCommit("select *from COMPANY")
    }
}
