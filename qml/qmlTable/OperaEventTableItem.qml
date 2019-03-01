import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0


Item {
    id: operaEventItem
    // title: qsTr("操作事件记录查询")
    //anchors.fill: parent
    anchors.bottomMargin: 40
    anchors.topMargin: 40
    Row {
        id: operaEventQuery
        spacing: 5
        anchors.topMargin: 20

        Text {
            id: productNum
            text: qsTr("产品编号")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: productNumTextField
            width: 100
            height: 30
        }

        Text {
            id: date
            text: qsTr("维保日期")
            height: 30
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        TextField {
            id: dateTextField
            width: 100
            height: 30
        }

        Button {
            id: operaEventQueryBtn
            text: qsTr("查询")
            height: 30
            onClicked: {


                //   console.log(selectInfo())
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

        //            rowDelegate: Loader {
        //                asynchronous: false
        //            }
        //            itemDelegate: Loader {

        //                asynchronous: false
        //            }

    }

    function selectInfo() {
        var info = new String
        if (productNumTextField.text.length > 0) {
            info += (qsTr("产品编号=") + productNumTextField.text)
        }

        if (dateTextField.text.length > 0) {
            if (info.length > 0) {
                info += ","
            }
            info += (qsTr("维保日期=") + dateTextField.text)
        }
        return info
    }
}
