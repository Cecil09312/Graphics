import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import QtQuick.Layouts 1.3
import "../qmlTable"
import QtQuick.Controls.Styles 1.4
import Qt.labs.platform 1.0

Item {

    id: root
    width: 1080
    height:720
    signal setCurrentAlarm(string curAlarm)

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

                savePdfFileDialog.open()


            }
        }
        NaviButton {
            id: printPreviewBtn
            text: qsTr("打印预览")
            height: 30
            visible: false
            onClicked:
            {

                switch(bar.currentIndex)
                {
                case 0:
                    currentAlarm.printPreview()
                    break;
                case 1:
                    alarmInfo.printPreview()
                    break;
                case 2:
                    deviceState.printPreview()
                    break;
                case 3:
                    maintInfo.printPreview()
                    break;
                case 4:
                    operaEvent.printPreview()
                    break;
                case 5:
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
                    currentAlarm.startPrint()
                    break;
                case 1:
                    alarmInfo.startPrint()
                    break;
                case 2:
                    deviceState.startPrint()
                    break;
                case 3:
                    maintInfo.startPrint()
                    break;
                case 4:
                    operaEvent.startPrint()
                    break;
                case 5:
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
            id:currentEventBtn
            text: qsTr("当前事件查询")

        }
        MyTabButton {
            id:alarmHistoryBtn
            text: qsTr("报警历史查询")

        }
        MyTabButton {
            id:deviceInfoBtn
            text: qsTr("设备信息查询")

        }
        MyTabButton {
            id:maintenBtn
            text:  qsTr("维保信息查询")

        }

        MyTabButton {
            id:operatorEventBtn
            text: qsTr("操作事件查询")
        }
        MyTabButton {
            id:analogBtn
            text: qsTr("模拟量查询")
        }

    }

    StackLayout {
        width: parent.width
        anchors.top: btnRow.bottom
        anchors.bottom: root.bottom
        currentIndex: bar.currentIndex

        CurrentAlarmItem
        {
            id:currentAlarm
            anchors.fill: parent
            anchors.topMargin:80
            onSetCurAlarm:
            {
              emit:setCurrentAlarm(alarmType)
            }
        }
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

    FileDialog
    {
        id:savePdfFileDialog
        fileMode: FileDialog.SaveFile
        nameFilters: ["PDF file(*.pdf)"]
        flags: Qt.WindowStaysOnTopHint|Qt.MSWindowsFixedSizeDialogHint|Qt.WindowCloseButtonHint
        onAccepted:
        {
            switch(bar.currentIndex)
            {
            case 0:
                currentAlarm.saveToPdf(currentFile.toString())
                break;
            case 1:
                alarmInfo.saveToPdf(currentFile.toString())
                break;
            case 2:
                deviceState.saveToPdf(currentFile.toString())
                break;
            case 3:
                maintInfo.saveToPdf(currentFile.toString())
                break;
            case 4:
                operaEvent.saveToPdf(currentFile.toString())
                break;
            case 5:
                analogInfo.saveToPdf(currentFile.toString())
                break;
            default:
                break;

            }
        }

    }

    function retranslate()
    {
        saveToPdfBtn.text = qsTr("保存成pdf")
        printPreviewBtn.text =  qsTr("打印预览")
        printBtn.text =  qsTr("打印")
        currentEventBtn.text = qsTr("当前事件查询")
        alarmHistoryBtn.text = qsTr("报警历史查询")
        deviceInfoBtn.text= qsTr("设备信息查询")
        maintenBtn.text = qsTr("维保信息查询")
        operatorEventBtn.text = qsTr("操作事件查询")
        analogBtn.text =qsTr("模拟量查询")
        currentAlarm.retranslate()
        alarmInfo.retranslate()
        deviceState.retranslate()
        maintInfo.retranslate()
        operaEvent.retranslate()
        analogInfo.retranslate()
    }

}
