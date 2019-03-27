import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import "../qmlTable"
import QtQuick.Controls.Styles 1.4

Item {

    anchors.fill: parent
    Row {

        anchors.top: parent.top
        id: btnRow
        spacing: 10
        Button {
            id: saveToPdfBtn
            text: qsTr("保存成pdf")
            height: 30
            onClicked: {
                tabView.saveToPdf()
            }
        }
        Button {
            id: printPreviewBtn
            text: qsTr("打印预览")
            height: 30
        }
        Button {
            id: printBtn
            text: qsTr("打印")
            height: 30
        }
    }
    Controls1_4.TabView {
        id: tabView
        anchors.top: btnRow.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        //width: parent.width
        anchors.topMargin: 20

        Controls1_4.Tab {
            id: tab1
            title: qsTr("报警历史查询")
            anchors.topMargin: 20

            AlarmInfoTableItem {
                id: alarmInfo
                Connections {
                    target: printBtn
                    onClicked: {
                        if (tabView.currentIndex == 0) {
                            alarmInfo.startPrint()
                        }
                    }
                }

                Connections {
                    target: printPreviewBtn
                    onClicked: {
                        if (tabView.currentIndex == 0) {
                            alarmInfo.printPreview()
                        }
                    }
                }

                Connections {
                    target: saveToPdfBtn
                    onClicked: {
                        if (tabView.currentIndex == 0) {
                            alarmInfo.saveToPdf()
                        }
                    }
                }
            }
        }

        Controls1_4.Tab {
            title: qsTr("设备信息查询")
            anchors.topMargin: 20
            DeviceStateTableItem {
                id: deviceState
                Connections {
                    target: printBtn
                    onClicked: {
                        if (tabView.currentIndex == 1) {
                            deviceState.startPrint()
                        }
                    }
                }

                Connections {
                    target: printPreviewBtn
                    onClicked: {
                        if (tabView.currentIndex == 1) {
                            deviceState.printPreview()
                        }
                    }
                }

                Connections {
                    target: saveToPdfBtn
                    onClicked: {
                        if (tabView.currentIndex == 1) {
                            deviceState.saveToPdf()
                        }
                    }
                }
            }
        }

        Controls1_4.Tab {
            title: qsTr("维保信息查询")
            anchors.topMargin: 20
            MaintInfoTableItem {
                anchors.topMargin: 20
                id: maintInfo

                Connections {
                    target: printBtn
                    onClicked: {
                        if (tabView.currentIndex == 2) {
                            maintInfo.startPrint()
                        }
                    }
                }

                Connections {
                    target: printPreviewBtn
                    onClicked: {
                        if (tabView.currentIndex == 2) {
                            maintInfo.printPreview()
                        }
                    }
                }

                Connections {
                    target: saveToPdfBtn
                    onClicked: {
                        if (tabView.currentIndex == 2) {
                            maintInfo.saveToPdf()
                        }
                    }
                }
            }
        }

        Controls1_4.Tab {
            title: qsTr("操作事件查询")
            anchors.topMargin: 20
            OperaEventTableItem {
                anchors.topMargin: 20

                id: operaEvent
                Connections {
                    target: printBtn
                    onClicked: {
                        if (tabView.currentIndex == 3) {
                            operaEvent.startPrint()
                        }
                    }
                }

                Connections {
                    target: printPreviewBtn
                    onClicked: {
                        if (tabView.currentIndex == 3) {
                            operaEvent.printPreview()
                        }
                    }
                }

                Connections {
                    target: saveToPdfBtn
                    onClicked: {
                        if (tabView.currentIndex == 3) {
                            operaEvent.saveToPdf()
                        }
                    }
                }
            }
        }

        style: TabViewStyle {
            frameOverlap: 1
            tab: Rectangle {
                color: styleData.selected ? "steelblue" : "lightsteelblue"
                border.color: "steelblue"
                implicitWidth: Math.max(text.width + 4, 80)
                implicitHeight: 40
                radius: 2
                Text {
                    id: text
                    anchors.centerIn: parent
                    text: styleData.title
                    color: styleData.selected ? "white" : "black"
                }
            }
            frame: Rectangle {
                id: rec
                color: "white"
                transitions: Transition {
                    PropertyAnimation {
                        target: rec
                        property: "opacity"
                        duration: 100
                        from: 0
                        to: 1
                    }
                }
            }
        }
    }
}
