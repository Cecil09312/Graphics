import QtQuick 2.9
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import Qt.labs.platform 1.0
import QtQuick.Dialogs 1.2
import speechObj 1.0
import crtWidget 1.0
import operatorInfo 1.0

Rectangle {
    width: 150
    signal currentAlarmType(string type)
    signal startAutoSwitch(bool isAuto)
    signal reset
    signal clearVoice()
    property int colorChangeNum:0
    property bool curState: false
    RowLayout {
        id: alarmBtnLayout
        height: 50
        width: parent.width
        Button {
            id: firmAlarmBtn
            Layout.alignment: Qt.AlignLeft
            Layout.topMargin: 10
            text: qsTr("首火警")
            Layout.fillWidth: true
            Layout.fillHeight: true

            //iconSource: "qrc:/images/fire.png"
            font.pointSize: 14
            font.family: "Times New Roman"
            onClicked: {
                ArchitePlanView.firstFireAlarm()
            }
            onPressed: {
                firmAlarmBtn.highlighted = true
            }
            onReleased: {
                firmAlarmBtn.highlighted = false
            }
        }

        Button {
            id: resetBtn
            // anchors.leftMargin: 20
            Layout.alignment: Qt.AlignRight

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 10
            font.pointSize: 14
            font.family: qsTr("Times New Roman")
            text: qsTr("消音")
            onClicked: {
                emit:clearVoice()
            }

            onPressed: {
                highlighted = true
            }
            onReleased: {
                highlighted = false
            }
        }
    }
    GridLayout {
        id: alarmIndicatorLayout
        anchors.top: alarmBtnLayout.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        Layout.fillWidth: true
        anchors.topMargin: 5
        StatusIndicator {
            id: fireAlarmStatusIndicator
            Layout.column: 0
            Layout.row: 0
            color: "gray" //火警:绿色:正常,红色:报警
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
            font.pointSize: 12
            font.family: qsTr("Times New Roman")
        }

        StatusIndicator {
            id: superviseStatusIndicator
            Layout.column: 0
            Layout.row: 1
            color: "gray" //监管:绿色:正常
            active: true
            ColorAnimation on color {
                id: superviseAnimation
                from: "orange"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }
        }
        Text {
            id: superviseNum
            Layout.column: 1
            Layout.row: 1
            text: qsTr("监管 0")
            font.pointSize: 12
            font.family: qsTr("Times New Roman")
        }

        StatusIndicator {
            id: linkageStatusIndicator
            Layout.column: 0
            Layout.row: 2
            color: "gray" //启动:绿色:正常,红色:报警
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
            Layout.row: 2
            text: qsTr("启动 0")
            font.pointSize: 12
            font.family: qsTr("Times New Roman")
        }

        StatusIndicator {
            id: feedbackStatusIndicator
            Layout.column: 0
            Layout.row: 3
            color: "gray" //反馈:绿色:正常
            active: true
            ColorAnimation on color {

                id: feedbackAnimation
                from: "blue"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }
        }
        Text {
            id: feedbackNum
            Layout.column: 1
            Layout.row: 3
            text: qsTr("反馈 0")
            font.pointSize: 12
            font.family: qsTr("Times New Roman")
        }

        StatusIndicator {
            id: faultStatusIndicator
            Layout.column: 0
            Layout.row: 4
            color: "gray" //故障:绿色:正常,黄色:故障
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
            Layout.row: 4
            text: qsTr("故障 0")
            font.pointSize: 12
            font.family: qsTr("Times New Roman")
        }



        StatusIndicator {
            id: shieldStatusIndicator
            Layout.column: 0
            Layout.row: 5
            color: "gray" //屏蔽:绿色:正常
            active: true
            ColorAnimation on color {
                id: shieldAnimation
                from: "#c93756"
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
            font.pointSize: 12
            font.family: qsTr("Times New Roman")
        }

        StatusIndicator {
            id: mainPowerIndicator
            Layout.column: 0
            Layout.row: 6
            color: "green"
            active: true
            ColorAnimation on color {
                id: mainPowerAnimation
                from: "yellow"
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
            font.pointSize: 12
            font.family: qsTr("Times New Roman")
        }

        StatusIndicator {
            id: standbyPowerIndicator
            Layout.column: 0
            Layout.row: 7
            color: "green"
            active: true
            ColorAnimation on color {
                id: standbyPowerAnimation
                from: "yellow"
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
            font.pointSize: 12
            font.family: qsTr("Times New Roman")
        }


        StatusIndicator {
            id: handOrAutoIndicator//默认时紫色，手动蓝色，自动绿色。
            Layout.column: 0
            Layout.row: 8
            color: "purple"
            active: true
            ColorAnimation on color {
                id: handOrAutoAnimation
                from: "purple"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }
        }
        Text {
            id: handOrAutoTxt
            Layout.column: 1
            Layout.row: 8
            text: qsTr("默认")
            font.pointSize: 12
            font.family: qsTr("Times New Roman")
        }



        StatusIndicator {
            id: transformIndicator //传输指示，传输正常：绿色；异常：红色；传输过程中闪烁。
            Layout.column: 0
            Layout.row: 9
            color: "gray"
            active: true

            ColorAnimation on color {
                id: transformAnimation
                from: "gray"
                to: "green"
                duration: 200
                loops: Animation.Infinite
                running: false
            }



            onColorChanged:
            {
                if(colorChangeNum%12==0)
                {
                   curState = !curState
                   CrtWidget.transportIndicator(curState)
                }
                colorChangeNum +=1
            }
        }

        Text {
            Layout.column: 1
            Layout.row: 9
            text: qsTr("传输")
            font.pointSize: 12
            font.family: qsTr("Times New Roman")
        }

        StatusIndicator {
            id: equiComIndicator
            Layout.column: 0
            Layout.row: 10
            color: "yellow"
            active: true
            ColorAnimation on color {
                id: equiComAnimation
                from: "yellow"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }
        }

        Text {
            id: equiComTxt
            Layout.column: 1
            Layout.row: 10
            text: qsTr("主机通信")
            font.pointSize: 12
            font.family: qsTr("Times New Roman")
        }

        StatusIndicator {
            id: centerComIndictor
            Layout.column: 0
            Layout.row: 11
            color: "yellow"
            active: true
            ColorAnimation on color {
                id: centerComAnimation
                from: "yellow"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }
        }
        Text {
            id: centerComTxt
            Layout.column: 1
            Layout.row: 11
            text: qsTr("中心通信")
            font.pointSize: 12
            font.family: qsTr("Times New Roman")
        }
    }

    Column {
        anchors.top: alarmIndicatorLayout.bottom
        width: parent.width - 40
        spacing: 5
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.topMargin: 5
        anchors.verticalCenter: parent.verticalCenter

        Button {
            anchors.leftMargin: 20
            width: parent.width
            // Layout.fillHeight: true
            font.pointSize: 14
            font.family: qsTr("Times New Roman")
            text: qsTr("复位")
            onClicked: {
                emit: reset()
            }

            onPressed: {
                highlighted = true
            }
            onReleased: {
                highlighted = false
            }
        }

        Button {
            anchors.leftMargin: 20
            width: parent.width
            // Layout.fillHeight: true
            font.pointSize: 14
            font.family: qsTr("Times New Roman")
            text: qsTr("报警平面")
            onClicked: {
                ArchitePlanView.toAlarmView()

                /*测试*/
               //CrtWidget.sendSeralData()
            }
            onPressed: {
                highlighted = true
            }
            onReleased: {
                highlighted = false
            }
        }
        ComboBox {
            id: alarmTypeComboBox
            anchors.leftMargin: 20
            font.pointSize: 14
            font.family: qsTr("Times New Roman")
            width: parent.width
            model: ["全部", "火警", "启动", "监管", "故障", "反馈", "屏蔽"]
            onCurrentTextChanged: {
                emit: currentAlarmType(currentText)
                if(currentText===qsTr("全部"))
                {
                     numTxt.visible=false
                }
                else
                {
                   numTxt.visible = true;
                }
            }
        }
        Button {
            id: previousBtn
            anchors.leftMargin: 20
            width: parent.width
            // Layout.fillHeight: true
            font.pointSize: 14
            font.family: qsTr("Times New Roman")
            text: qsTr("上一页")
            onClicked: {
                ArchitePlanView.toPreviousPage()
            }
            onPressed: {
                highlighted = true
            }
            onReleased: {
                highlighted = false
            }
        }

        Button {
            id: nextBtn
            anchors.leftMargin: 20
            width: parent.width
            font.pointSize: 14
            font.family: qsTr("Times New Roman")
            text: qsTr("下一页")
            onClicked: {
                ArchitePlanView.toNextPage()
            }
            onPressed: {
                highlighted = true
            }
            onReleased: {
                highlighted = false
            }
        }

        Text {
            id: pageTxt
            anchors.leftMargin: 20
            width: parent.width
            font.pointSize: 12
            font.family: qsTr("Times New Roman")
            text: qsTr("总0页/第0页")
        }

        Text {
            id: numTxt
            anchors.leftMargin: 20
            width: parent.width
            font.pointSize: 12
            font.family: qsTr("Times New Roman")
            text: qsTr("总0个/第0个")
            visible: false
        }

        CheckBox {
            id: autoSwitchCheckBox
            text: qsTr("自动切换")
            width: parent.width
            font.pointSize: 12
            font.family: qsTr("Times New Roman")
            onClicked: {
                ArchitePlanView.startAutoSwitch(checked)
            }
        }
    }

    function currentAlarm() {
        return alarmTypeComboBox.currentText
    }

    function setPage(totalPage, currentPage) {

        pageTxt.text = String("总%1页/第%2页").arg(totalPage).arg(currentPage)
    }
    function setNum(totalNum,currentNum)
    {
       numTxt.text= String("总%1个/第%2个").arg(totalNum).arg(currentNum);
    }

    function enableToPreviousPageBtn(isEnable) {
        previousBtn.enabled = isEnable
    }

    function enableToNextPageBtn(isEnable) {
        nextBtn.enabled = isEnable
    }

    function setFireAlarmColor(isActived, currentColor) {
        fireAlarmStatusIndicator.color = currentColor
        fireAlarmStatusIndicator.active = isActived
    }

    function setLinkageAlarmColor(isActived, currentColor) {
        linkageStatusIndicator.color = currentColor
        linkageStatusIndicator.active = isActived
    }

    function setSuperviseAlarmColor(isActived, currentColor) {
        superviseStatusIndicator.color = currentColor
        superviseStatusIndicator.active = isActived
    }

    function setfaultAlarmColor(isActived, currentColor) {
        faultStatusIndicator.color = currentColor
        faultStatusIndicator.active = isActived
    }

    function setFeedbackColor(isActived, currentColor) {
        feedbackStatusIndicator.color = currentColor
        feedbackStatusIndicator.active = isActived
    }

    function setShieldAlarmColor(isActived, currentColor) {
        shieldStatusIndicator.color = currentColor
        shieldStatusIndicator.active = isActived
    }

    function setMainPowerColor(isActived, currentColor) {
        mainPowerIndicator.color = currentColor
        mainPowerIndicator.active = isActived
    }

    function setStandbyPowerColor(isActived, currentColor) {
        standbyPowerIndicator.color = currentColor
        standbyPowerIndicator.active = isActived
    }

    function setEquiComColor(isActived, currentColor) {
        equiComIndicator.color = currentColor
        equiComIndicator.active = isActived
    }

    function setCenterComColor(isActived, currentColor) {
        centerComIndictor.color = currentColor
        centerComIndictor.active = isActived
    }

    function setHandOrAutoColor(isActived, currentColor) {
        handOrAutoIndicator.color = currentColor
        handOrAutoIndicator.active = isActived
    }

    function setTransformColor(isActived, currentColor) {
        transformIndicator.color = currentColor
        transformIndicator.active = isActived
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

    function startMainPowerAnimation(isRunning) {
        if (isRunning) {
            if (!mainPowerAnimation.running)
                mainPowerAnimation.start()
        } else {
            mainPowerAnimation.stop()
        }
    }

    function startStandbyPowerAnimation(isRunning) {
        if (isRunning) {
            if (!standbyPowerAnimation.running)
                standbyPowerAnimation.start()
        } else {
            standbyPowerAnimation.stop()
        }
    }

    function startTransformAnimation(isRunning) {
        if (isRunning) {
            if (!transformAnimation.running)
                transformAnimation.start()
        } else {
            transformAnimation.stop()
        }
    }

    function setFireAlarmText(value) {
        var txt = qsTr("火警 ") + value
        fireAlarmNum.text = txt
    }

    function setLinkageText(value) {
        var txt = qsTr("启动 ") + value
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

    function setHandOrAutoText(value)
    {
        handOrAutoTxt.text = value
    }

    function allAlarmClear() {
        startFireAnimation(false)
        startLinkageAnimation(false)
        startSuperviseAnimation(false)
        startFaultAnimation(false)
        startFeedbackAnimation(false)
        startShieldAnimation(false)
        startMainPowerAnimation(false)
        startStandbyPowerAnimation(false)
        setFireAlarmColor(true, "gray")
        setLinkageAlarmColor(true, "gray")
        setSuperviseAlarmColor(true, "gray")
        setfaultAlarmColor(true, "gray")
        setFeedbackColor(true, "gray")
        setShieldAlarmColor(true, "gray")
        setMainPowerColor(true, "green")
        setStandbyPowerColor(true, "green")
       // setEquiComColor(true, "gray")
       // setCenterComColor(true, "gray")
        setHandOrAutoColor(true,"purple")
        setHandOrAutoText(qsTr("默认"))
        setFireAlarmText("0")
        setLinkageText("0")
        setSuperviseText("0")
        setFaultText("0")
        setFeedbackText("0")
        setShieldText("0")
    }

    function setAutoSwitchCheckBoxState(isChecked) {
        autoSwitchCheckBox.checked = isChecked
    }
}
