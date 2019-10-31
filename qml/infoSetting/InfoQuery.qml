import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import "../qmlTable"
import QtQuick.Controls.Styles 1.4

Item {

    id: root
    width: 980
    height: 680

    Row {

        anchors.top: parent.top
        anchors.topMargin: 10
        id: btnRow
        spacing: 10
        NaviButton {
            id: saveToPdfBtn
            text: qsTr("保存成pdf")
            height: 30
        }
        NaviButton {
            id: printPreviewBtn
            text: qsTr("打印预览")
            height: 30
        }
        NaviButton {
            id: printBtn
            text: qsTr("打印")
            height: 30
        }
    }
    Controls1_4.TabView {
        id: tabView
        anchors.top: btnRow.bottom
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom
        width: root.width
        anchors.topMargin: 20
        frameVisible: true

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

        Controls1_4.Tab {
            title: qsTr("模拟量查询")
            anchors.topMargin: 20
            AnalogTableItem {
                anchors.topMargin: 20

                id: analogTable
                Connections {
                    target: printBtn
                    onClicked: {
                        if (tabView.currentIndex == 4) {
                            analogTable.startPrint()
                        }
                    }
                }

                Connections {
                    target: printPreviewBtn
                    onClicked: {
                        if (tabView.currentIndex == 4) {
                            analogTable.printPreview()
                        }
                    }
                }

                Connections {
                    target: saveToPdfBtn
                    onClicked: {
                        if (tabView.currentIndex == 4) {
                            analogTable.saveToPdf()
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
