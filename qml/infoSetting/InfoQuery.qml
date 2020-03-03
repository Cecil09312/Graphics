import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import QtQuick.Layouts 1.3
import "../qmlTable"
import QtQuick.Controls.Styles 1.4

Item {

    id: root
    width: 980
    height: 640

    Row {


        anchors.top: parent.top
        anchors.topMargin: 20
        id: btnRow
        spacing: 10
        NaviButton {
            id: saveToPdfBtn
            text: qsTr("保存成pdf")
            height: 30
            onClicked:
            {

                switch(bar.currentIndex)
                {
                case 0:
                    alarmInfo.saveToPdf()
                    break;
                case 1:
                    deviceState.saveToPdf()
                    break;
                case 2:
                    maintInfo.saveToPdf()
                    break;
                case 3:
                    operaEvent.saveToPdf()
                    break;
                case 4:
                    analogInfo.saveToPdf()
                    break;
                default:
                    break;

                }

            }
        }
        NaviButton {
            id: printPreviewBtn
            text: qsTr("打印预览")
            height: 30
            onClicked:
            {

                switch(bar.currentIndex)
                {
                case 0:
                    alarmInfo.printPreview()
                    break;
                case 1:
                    deviceState.printPreview()
                    break;
                case 2:
                    maintInfo.printPreview()
                    break;
                case 3:
                    operaEvent.printPreview()
                    break;
                case 4:
                    analogInfo.printPreview()
                    break;
                default:
                    break;

                }


            }
        }
        NaviButton {
            id: printBtn
            text: qsTr("打印")
            height: 30
            onClicked:
            {

                switch(bar.currentIndex)
                {
                case 0:
                    alarmInfo.startPrint()
                    break;
                case 1:
                    deviceState.startPrint()
                    break;
                case 2:
                    maintInfo.startPrint()
                    break;
                case 3:
                    operaEvent.startPrint()
                    break;
                case 4:
                    analogInfo.startPrint()
                    break;
                default:
                    break;

                }


            }
        }
    }

    TabBar {
        id: bar
        anchors.top: btnRow.bottom
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom
        width: root.width
        anchors.topMargin: 20
        MyTabButton {
            text: qsTr("报警历史查询")
        }
        MyTabButton {
            text: qsTr("设备信息查询")
        }
        MyTabButton {
            text:  qsTr("维保信息查询")
        }

        MyTabButton {
            text: qsTr("操作事件查询")
        }
        MyTabButton {
            text: qsTr("模拟量查询")
        }

    }

    StackLayout {
        width: parent.width
        anchors.top: btnRow.bottom
       anchors.bottom: root.bottom
        currentIndex: bar.currentIndex

        AlarmInfoTableItem {

            id: alarmInfo
            anchors.fill: parent
            anchors.topMargin:80
        }
        DeviceStateTableItem {
            id: deviceState
            anchors.fill: parent
            anchors.topMargin:80
        }

        MaintInfoTableItem {

            id: maintInfo
            anchors.fill: parent
            anchors.topMargin:80
//            width: 800
//            height: 640
        }

        OperaEventTableItem {
            id: operaEvent
//            width: 800
//            height: 640
            anchors.fill: parent
            anchors.topMargin:80
            //anchors.bottomMargin: 120

        }
        AnalogTableItem {
            id:analogInfo
            anchors.fill: parent
            anchors.topMargin:80
        }

    }

}
