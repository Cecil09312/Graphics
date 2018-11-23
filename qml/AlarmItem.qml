import QtQuick 2.9
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Rectangle {
    width: 150
    Rectangle {
        id: alarmBtnRec
        height: 100
        width: parent.width
        RowLayout {
            anchors.fill: parent
            Button {
                Layout.alignment: Qt.AlignLeft
                text: qsTr("首火警")
                Layout.fillWidth: true
                onClicked: {
                    ArchitePlanView.firstFireAlarm()
                }
            }

            Button {
                Layout.alignment: Qt.AlignRight
                text: qsTr("末火警")
                Layout.fillWidth: true
                onClicked: {
                    ArchitePlanView.lastFireAlarm()
                }
            }
        }
    }
    GridLayout {
        id: alarmIndicatorLayout
        anchors.top: alarmBtnRec.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        Layout.fillWidth: true
        StatusIndicator {
            id: fireAlarmStatusIndicator
            Layout.column: 0
            Layout.row: 0
            color: "green"
            active: true

            ColorAnimation on color {
                id: fireAlarmAnimation
                from: "red"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }
        }
        Text {
            id: fireAlarmNum
            Layout.column: 1
            Layout.row: 0
            text: qsTr("火警 0")
        }

        StatusIndicator {
            id: linkageStatusIndicator
            Layout.column: 0
            Layout.row: 1
            color: "green"
            active: true
            ColorAnimation on color {
                id: linkageAnimation
                from: "red"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }
        }
        Text {
            id: linkageNum
            Layout.column: 1
            Layout.row: 1
            text: qsTr("联动 0")
        }

        StatusIndicator {
            id: superviseStatusIndicator
            Layout.column: 0
            Layout.row: 2
            color: "green"
            active: true
            ColorAnimation on color {
                id: superviseAnimation
                from: "red"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }
        }
        Text {
            id: superviseNum
            Layout.column: 1
            Layout.row: 2
            text: qsTr("监管 0")
        }

        StatusIndicator {
            id: faultStatusIndicator
            Layout.column: 0
            Layout.row: 3
            color: "black"
            active: true
            ColorAnimation on color {
                id: faultAnimation
                from: "yellow"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }
        }
        Text {
            id: faultNum
            Layout.column: 1
            Layout.row: 3
            text: qsTr("故障 0")
        }

        StatusIndicator {
            id: feedbackStatusIndicator
            Layout.column: 0
            Layout.row: 4
            color: "black"
            active: true
            ColorAnimation on color {

                id: feedbackAnimation
                from: "red"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }
        }
        Text {
            id: feedbackNum
            Layout.column: 1
            Layout.row: 4
            text: qsTr("反馈 0")
        }

        StatusIndicator {
            id: shieldStatusIndicator
            Layout.column: 0
            Layout.row: 5
            color: "green"
            active: true
            ColorAnimation on color {
                id: shieldAnimation
                from: "red"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }
        }
        Text {
            id: shieldNum
            Layout.column: 1
            Layout.row: 5
            text: qsTr("屏蔽 0")
        }

        StatusIndicator {
            id: mainPowerIndicator
            Layout.column: 0
            Layout.row: 6
            color: "green"
            active: true
            ColorAnimation on color {
                id: mainPowerAnimation
                from: "red"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }
        }
        Text {
            id: mainPowerTxt
            Layout.column: 1
            Layout.row: 6
            text: qsTr("主电")
        }

        StatusIndicator {
            id: standbyPowerIndicator
            Layout.column: 0
            Layout.row: 7
            color: "green"
            active: true
            ColorAnimation on color {
                id: standbyPowerAnimation
                from: "red"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }
        }
        Text {
            id: standbyPowerTxt
            Layout.column: 1
            Layout.row: 7
            text: qsTr("备电")
        }

        StatusIndicator {
            id: equiComIndicator
            Layout.column: 0
            Layout.row: 8
            color: "green"
            active: true
            ColorAnimation on color {
                id: equiComAnimation
                from: "red"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }
        }
        Text {
            id: equiComTxt
            Layout.column: 1
            Layout.row: 8
            text: qsTr("设备通信")
        }

        StatusIndicator {
            id: centerComIndictor
            Layout.column: 0
            Layout.row: 9
            color: "green"
            active: true
            ColorAnimation on color {
                id: centerComAnimation
                from: "red"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }
        }
        Text {
            id: centerComTxt
            Layout.column: 1
            Layout.row: 9
            text: qsTr("中心通信")
        }
    }

    Column {
        anchors.top: alarmIndicatorLayout.bottom
        width: parent.width - 40
        spacing: 5
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter

        Repeater {
            id: repeater
            Layout.fillHeight: true

            model: ListModel {
                ListElement {
                    name: qsTr("消音")
                }
                ListElement {
                    name: qsTr("复位")
                }
                ListElement {
                    name: qsTr("模拟火警")
                }

                ListElement {
                    name: qsTr("上一页")
                }

                ListElement {
                    name: qsTr("下一页")
                }
            }

            Button {

                text: name
                anchors.leftMargin: 20
                width: parent.width

                onClicked: {
                    if (text === qsTr("模拟火警")) {
                        ArchitePlanView.creatAlarm()
                    }
                }
            }
        }
    }

    function startFireAnimation(isRunning) {
        if (isRunning) {
            if (!fireAlarmAnimation.running)
                fireAlarmAnimation.start()
        } else {
            fireAlarmAnimation.stop()
        }
    }

    function startLinkageAnimation(isRunning) {
        if (isRunning) {
            if (!linkageAnimation.running)
                linkageAnimation.start()
        } else {
            linkageAnimation.stop()
        }
    }

    function startSuperviseAnimation(isRunning) {
        if (isRunning) {
            if (!superviseAnimation.running)
                superviseAnimation.start()
        } else {
            superviseAnimation.stop()
        }
    }

    function startFaultAnimation(isRunning) {
        if (isRunning) {
            if (!faultAnimation.running)
                faultAnimation.start()
        } else {
            faultAnimation.stop()
        }
    }

    function startFeedbackAnimation(isRunning) {
        if (isRunning) {
            if (!feedbackAnimation.running)
                feedbackAnimation.start()
        } else {
            feedbackAnimation.stop()
        }
    }

    function startShieldAnimation(isRunning) {
        if (isRunning) {
            if (!shieldAnimation.running)
                shieldAnimation.start()
        } else {
            shieldAnimation.stop()
        }
    }

    function startMainConnunicationAnimation(isRunning) {
        if (isRunning) {
            if (!mainConnunicationAnimation.running)
                mainConnunicationAnimation.start()
        } else {
            mainConnunicationAnimation.stop()
        }
    }

    function setFireAlarmText(value) {
        var txt = qsTr("火警 ") + value
        fireAlarmNum.text = txt
    }

    function setLinkageText(value) {
        var txt = qsTr("联动 ") + value
        linkageNum.text = txt
    }

    function setSuperviseText(value) {
        var txt = qsTr("监管 ") + value
        superviseNum.text = txt
    }

    function setFaultText(value) {
        var txt = qsTr("故障 ") + value
        faultNum.text = txt
    }

    function setFeedbackText(value) {
        var txt = qsTr("反馈 ") + value
        feedbackNum.text = txt
    }

    function setShieldText(value) {
        var txt = qsTr("屏蔽 ") + value
        shieldNum.text = txt
    }
}
