import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0

Item {
    // id: operaEventItem
    anchors.bottomMargin: 40
    anchors.topMargin: 40
    Row {
        id: operaEventQuery
        spacing: 5
        anchors.topMargin: 20

        Text {
            id: event
            text: qsTr("事件")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: eventTextField
            width: 100
            height: 30
        }

        Text {
            id: time
            text: qsTr("时间")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: timeTextField
            width: 100
            height: 30
        }

        Button {
            id: operaEventQueryBtn
            text: qsTr("查询")
            height: 30
            onClicked: {

            }
        }

        Button {
            id: operaEventQueryAllBtn
            text: qsTr("查询所有")
            height: 30
            onClicked: {

            }
        }

        Button {
            id: operaEventQueryClearBtn
            text: qsTr("清空")
            height: 30
            onClicked: {

            }
        }
    }

    Controls1_4.TableView {
        anchors.top: operaEventQuery.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 5

        Controls1_4.TableViewColumn {
            role: "userName"
            title: qsTr("用户名")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "userRight"
            title: qsTr("用户权限")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "event"
            title: qsTr("事件")
            width: 60
            //resizable: true
        }

        Controls1_4.TableViewColumn {
            role: "time"
            title: qsTr("时间")
            width: 120
            //resizable: true
        }

        model: operaEventQueryModel

        QmlTableModel {
            id: operaEventQueryModel
            roleNameList: ["userName", "userRight", "event", "time"]
            titleList: [qsTr("用户名"), qsTr("用户权限"), qsTr("事件"), qsTr("时间")]
        }
    }

    function selectInfo() {
        var info = new String
        if (productNumTextField.text.length > 0) {
            info += (qsTr("事件=") + productNumTextField.text)
        }

        if (dateTextField.text.length > 0) {
            if (info.length > 0) {
                info += " and "
            }
            info += (qsTr("时间=") + dateTextField.text)
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
