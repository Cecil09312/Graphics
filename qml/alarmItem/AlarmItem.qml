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
import QtGraphicalEffects 1.0
import QtQuick.Window 2.3

import "../infoSetting"
//import QtQuick.Controls 1.4 as Controls1_4
Rectangle {
    width: 154
    signal currentAlarmType(string type)
    signal startAutoSwitch(bool isAuto)
    signal reset
    signal clearVoice()
    signal reSendCmd()
    property int colorChangeNum:0
    property bool curState: false
    property int totalNum: 0
    property int curNum: 0

    RowLayout {
        id: alarmBtnLayout
        height: 50
        width: parent.width


        NaviButton {
            id: firmAlarmBtn
            Layout.alignment: Qt.AlignLeft
            Layout.topMargin: 10
            text: qsTr("首火警")
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: 2
            onClicked: {
                ArchitePlanView.firstFireAlarm()
            }
        }

        NaviButton {
            id: erasureBtn
            // anchors.leftMargin: 20
            Layout.alignment: Qt.AlignRight

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 10
            text: qsTr("消音")
            Layout.rightMargin:2
            onClicked: {
                emit:clearVoice()
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
            font.family: qsTr("宋体")
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
            font.family: qsTr("宋体")
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
            font.family: qsTr("宋体")
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
            font.family: qsTr("宋体")
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
            font.family: qsTr("宋体")
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
            font.family: qsTr("宋体")
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
            font.family: qsTr("宋体")
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
            font.family: qsTr("宋体")
        }


        StatusIndicator {
            id: handOrAutoIndicator//默认时紫色，手动蓝色，自动绿色。
            Layout.column: 0
            Layout.row: 8
            color: "green"
            active: true
            ColorAnimation on color {
                id: handOrAutoAnimation
                from: "green"
                to: "black"
                duration: 1000
                loops: Animation.Infinite
                running: false
            }

            MouseArea
            {
                anchors.fill: parent
               acceptedButtons: Qt.LeftButton|Qt.RightButton
               onClicked:
               {
                   //console.log("*************")

                   popup.x=mouseX
                   popup.y=mouseY
                   //if(pressedButtons==Qt.LeftButton)
                   //{
                     popup.open()
                   //}
//                   else
//                   {
//                     popup.close()
//                   }


               }

            }

        }
        Text {
            id: handOrAutoTxt
            //flat: true
            Layout.column: 1
            Layout.row: 8
            text: qsTr("自动")
            font.pointSize: 12
            font.family: qsTr("宋体")




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
            font.family: qsTr("宋体")
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

            //            onColorChanged:
            //            {
            //                if(color=="#008000")
            //                {
            //                    emit:reSendCmd()
            //                }
            //            }
        }

        Text {
            id: equiComTxt
            Layout.column: 1
            Layout.row: 10
            text: qsTr("主机通信")
            font.pointSize: 12
            font.family: qsTr("宋体")
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
            font.family: qsTr("宋体")
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

        NaviButton {
            anchors.leftMargin: 20
            width: parent.width
            // Layout.fillHeight: true
            font.pointSize: 14
            font.family: qsTr("宋体")
            text: qsTr("复位")

            onClicked: {
                emit: reset()
            }


        }

        NaviButton {
            anchors.leftMargin: 20
            width: parent.width
            // Layout.fillHeight: true
            font.pointSize: 14
            font.family: qsTr("宋体")
            text: qsTr("报警平面")
            onClicked: {
                ArchitePlanView.toAlarmView()

                /*测试*/
               // CrtWidget.sendSeralData()
            }

        }
        //        ComboBox {
        //            id: alarmTypeComboBox
        //            anchors.leftMargin: 20
        //            font.pointSize: 14
        //            font.family: qsTr("宋体")
        //            width: parent.width
        //            model:["火警"] /*["全部", "火警", "监管","启动", "反馈","故障",  "屏蔽"]*/
        //            onCurrentTextChanged: {
        //                emit: currentAlarmType(currentText)
        //                if(currentText===qsTr("全部"))
        //                {
        //                    numTxt.visible=false
        //                }
        //                else
        //                {
        //                    numTxt.visible = true;
        //                }
        //            }
        //        }
        NaviButton {
            id: previousBtn
            anchors.leftMargin: 20
            width: parent.width
            // Layout.fillHeight: true
            font.pointSize: 14
            font.family: qsTr("宋体")
            text: qsTr("上一页")
            onClicked: {
                ArchitePlanView.toPreviousPage()
            }

        }

        NaviButton {
            id: nextBtn
            anchors.leftMargin: 20
            width: parent.width
            font.pointSize: 14
            font.family: qsTr("宋体")
            text: qsTr("下一页")
            onClicked: {
                ArchitePlanView.toNextPage()
            }

        }

        Text {
            id: pageTxt
            anchors.leftMargin: 20
            width: parent.width
            font.pointSize: 12
            font.family: qsTr("宋体")
            text: qsTr("总0页/第0页")
        }

        Text {
            id: numTxt
            anchors.leftMargin: 20
            width: parent.width
            font.pointSize: 12
            font.family: qsTr("宋体")
            text: qsTr("总0个/第0个")
            visible: false
        }

        //        Text {
        //            id: explainTxt
        //            anchors.leftMargin: 20
        //            width: parent.width
        //            font.pointSize: 12
        //            font.family: qsTr("宋体")
        //            text: qsTr("总0个/第0个")
        //            visible: false
        //        }

        CheckBox {
            id: autoSwitchCheckBox
            text: qsTr("自动切换")
            width: parent.width
            font.pointSize: 12
            font.family: qsTr("宋体")
            onClicked: {
                ArchitePlanView.startAutoSwitch(checked)
            }
        }
    }


    Popup {
           id: popup
//           x: 100
//           y: 100
           width: 60
           height: 100
           modal: true
           focus: true
           closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
       }

    function currentAlarm() {
        return alarmTypeComboBox.currentText
    }

    function setPage(totalPage, currentPage) {

        pageTxt.text = String("总%1页/第%2页").arg(totalPage).arg(currentPage)
    }
    function setNum(totalNumber,currentNum)
    {
        totalNum = totalNumber
        curNum = currentNum
        numTxt.text= String("总%1个/第%2个").arg(totalNum).arg(curNum);
    }
    function setTotalNum(totalNumber)
    {
        totalNum = totalNumber
        numTxt.text= String("总%1个/第%2个").arg(totalNum).arg(curNum)
    }

    function setCurNum(curNumber)
    {
        curNum = curNumber
        numTxt.text= String("总%1个/第%2个").arg(totalNum).arg(curNum)
    }

    function enableToPreviousPageBtn(isEnable) {
        previousBtn.enabled = isEnable
    }

    function enableToNextPageBtn(isEnable) {
        nextBtn.enabled = isEnable
    }

    function setFireAlarmColor(isActived, currentColor) {
        if(!Qt.colorEqual(fireAlarmStatusIndicator.color,currentColor))
        {
            fireAlarmStatusIndicator.color = currentColor
            fireAlarmStatusIndicator.active = isActived
        }

    }

    function setLinkageAlarmColor(isActived, currentColor) {
        if(!Qt.colorEqual(linkageStatusIndicator.color,currentColor))
        {
            linkageStatusIndicator.color = currentColor
            linkageStatusIndicator.active = isActived
        }
    }

    function setSuperviseAlarmColor(isActived, currentColor) {
        if(!Qt.colorEqual(superviseStatusIndicator.color,currentColor))
        {
            superviseStatusIndicator.color = currentColor
            superviseStatusIndicator.active = isActived
        }
    }

    function setfaultAlarmColor(isActived, currentColor) {
        if(!Qt.colorEqual(faultStatusIndicator.color,currentColor))
        {
            faultStatusIndicator.color = currentColor
            faultStatusIndicator.active = isActived
        }
    }

    function setFeedbackColor(isActived, currentColor) {
        if(!Qt.colorEqual(feedbackStatusIndicator.color,currentColor))
        {
            feedbackStatusIndicator.color = currentColor
            feedbackStatusIndicator.active = isActived
        }
    }

    function setShieldAlarmColor(isActived, currentColor) {
        if(!Qt.colorEqual(shieldStatusIndicator.color,currentColor))
        {
            shieldStatusIndicator.color = currentColor
            shieldStatusIndicator.active = isActived
        }
    }

    function setMainPowerColor(isActived, currentColor) {

        if(!Qt.colorEqual(mainPowerIndicator.color,currentColor))
        {
            mainPowerIndicator.color = currentColor
            mainPowerIndicator.active = isActived
        }


    }

    function setStandbyPowerColor(isActived, currentColor) {

        if(!Qt.colorEqual(standbyPowerIndicator.color,currentColor))
        {
            standbyPowerIndicator.color = currentColor
            standbyPowerIndicator.active = isActived
        }

    }

    function setEquiComColor(isActived, currentColor) {
        if(!Qt.colorEqual(equiComIndicator.color,currentColor))
        {
            equiComIndicator.color = currentColor
            equiComIndicator.active = isActived
        }

    }

    function setCenterComColor(isActived, currentColor) {
        if(!Qt.colorEqual(centerComIndictor.color,currentColor))
        {
            centerComIndictor.color = currentColor
            centerComIndictor.active = isActived
        }

    }

    function setHandOrAutoColor(isActived, currentColor) {
         if(!Qt.colorEqual(handOrAutoIndicator.color,currentColor))
         {
             handOrAutoIndicator.color = currentColor
             handOrAutoIndicator.active = isActived
         }

    }

    function setTransformColor(isActived, currentColor) {
        //if(!Qt.colorEqual(transformIndicator.color,currentColor))
       // {
            transformIndicator.color = currentColor
            transformIndicator.active = isActived
        //}

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
        var txt = qsTr("火警 ") + String("%1").arg(value)
        fireAlarmNum.text = txt
    }

    function setLinkageText(value) {
        var txt = qsTr("启动 ") + String("%1").arg(value)
        linkageNum.text = txt
    }

    function setSuperviseText(value) {
        var txt = qsTr("监管 ") + String("%1").arg(value)
        superviseNum.text = txt
    }

    function setFaultText(value) {
        var txt = qsTr("故障 ") + String("%1").arg(value)
        faultNum.text = txt
    }

    function setFeedbackText(value) {
        var txt = qsTr("反馈 ") + String("%1").arg(value)
        feedbackNum.text = txt
    }

    function setShieldText(value) {
        var txt = qsTr("屏蔽 ") + String("%1").arg(value)
        shieldNum.text = txt
    }


    function setAlarmText(type,value)
    {
        if(type==qsTr("火警"))
        {
            setFireAlarmText(value)
        }
        else if(type==qsTr("监管"))
        {
            setSuperviseText(value)
        }
        else if(type==qsTr("启动"))
        {
            setLinkageText(value)
        }
        else if(type==qsTr("故障"))
        {
            setFaultText(value)
        }
        else if(type==qsTr("反馈"))
        {
            setFeedbackText(value)
        }
        else if(type==qsTr("屏蔽"))
        {
            setShieldText(value)
        }

    }

    function setHandOrAutoText(value)
    {
        handOrAutoTxt.text = value
    }

    function allAlarmClear(fireAlarmClear) {
        startFireAnimation(false)
        startLinkageAnimation(false)
        startSuperviseAnimation(false)
        startFaultAnimation(false)
        startFeedbackAnimation(false)
        startShieldAnimation(false)
        startMainPowerAnimation(false)
        startStandbyPowerAnimation(false)
        if(fireAlarmClear)
        {
            setFireAlarmColor(true, "gray")
            setLinkageAlarmColor(true, "gray")
        }
        setSuperviseAlarmColor(true, "gray")
        setfaultAlarmColor(true, "gray")
        setFeedbackColor(true, "gray")
        setShieldAlarmColor(true, "gray")
        setMainPowerColor(true, "green")
        setStandbyPowerColor(true, "green")
        setTransformColor(true,"gray")
        // setEquiComColor(true, "gray")
        // setCenterComColor(true, "gray")
        setHandOrAutoColor(true,"green")
        setHandOrAutoText(qsTr("自动"))
        setFireAlarmText("0")
        setLinkageText("0")
        setSuperviseText("0")
        setFaultText("0")
        setFeedbackText("0")
        setShieldText("0")
        setAutoSwitchCheckBoxState(false)
    }

    function setAutoSwitchCheckBoxState(isChecked) {
        autoSwitchCheckBox.checked = isChecked
    }
}

