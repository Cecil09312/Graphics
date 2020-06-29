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
    signal mainPowerViewShow()
    signal standbyPowerViewShow()
    signal handOrAutoStateViewShow()
    signal showOnlineState()
    property int colorChangeNum:0
    property bool curState: false
    property int totalNum: 0
    property int curNum: 0
    property int fireNum:0
    property int supervisionNum:0
    property int respondingNum:0
    property int startNum:0
    property int failureNum:0
    property int shieldingNum: 0
    property  string curHandOrAutoTxt: qsTr("自动")

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
            Layout.topMargin: 11
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
        }
        Text {
            id: fireAlarmNum
            Layout.column: 1
            Layout.row: 0
            text: qsTr("火警 0")
            font.pointSize: 12
            font.family: "Times New Roman"
        }

        StatusIndicator {
            id: superviseStatusIndicator
            Layout.column: 0
            Layout.row: 1
            color: "gray" //监管:绿色:正常
            active: true

        }
        Text {
            id: superviseNum
            Layout.column: 1
            Layout.row: 1
            text: qsTr("监管 0")
            font.pointSize: 12
            font.family: "Times New Roman"
        }

        StatusIndicator {
            id: linkageStatusIndicator
            Layout.column: 0
            Layout.row: 2
            color: "gray" //启动:绿色:正常,红色:报警
            active: true

        }
        Text {
            id: linkageNum
            Layout.column: 1
            Layout.row: 2
            text: qsTr("启动 0")
            font.pointSize: 12
            font.family: "Times New Roman"
        }

        StatusIndicator {
            id: respondStatusIndicator
            Layout.column: 0
            Layout.row: 3
            color: "gray" //反馈:绿色:正常
            active: true

        }
        Text {
            id: respondNum
            Layout.column: 1
            Layout.row: 3
            text: qsTr("反馈 0")
            font.pointSize: 12
            font.family: "Times New Roman"
        }

        StatusIndicator {
            id: faultStatusIndicator
            Layout.column: 0
            Layout.row: 4
            color: "gray" //故障:绿色:正常,黄色:故障
            active: true

        }
        Text {
            id: faultNum
            Layout.column: 1
            Layout.row: 4
            text: qsTr("故障 0")
            font.pointSize: 12
            font.family: "Times New Roman"
        }



        StatusIndicator {
            id: shieldStatusIndicator
            Layout.column: 0
            Layout.row: 5
            color: "gray" //屏蔽:绿色:正常
            active: true

        }
        Text {
            id: shieldNum
            Layout.column: 1
            Layout.row: 5
            text: qsTr("屏蔽 0")
            font.pointSize: 12
            font.family: "Times New Roman"
        }

        StatusIndicator {
            id: mainPowerIndicator
            Layout.column: 0
            Layout.row: 6
            color: "green"
            active: true


            MouseArea
            {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                onClicked:
                {
                    emit:mainPowerViewShow()
                }

            }
        }
        Text {
            id: mainPowerTxt
            Layout.column: 1
            Layout.row: 6
            text: qsTr("主电")
            font.pointSize: 12
            font.family: "Times New Roman"
        }

        StatusIndicator {
            id: standbyPowerIndicator
            Layout.column: 0
            Layout.row: 7
            color: "green"
            active: true

            MouseArea
            {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                onClicked:
                {
                    emit:standbyPowerViewShow()
                }

            }
        }
        Text {
            id: standbyPowerTxt
            Layout.column: 1
            Layout.row: 7
            text: qsTr("备电")
            font.pointSize: 12
            font.family: "Times New Roman"
        }


        StatusIndicator {
            id: handOrAutoIndicator//默认时紫色，手动蓝色，自动绿色。
            Layout.column: 0
            Layout.row: 8
            color: "green"
            active: true

            MouseArea
            {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                onClicked:
                {
                    emit:handOrAutoStateViewShow()
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
            font.family: "Times New Roman"

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
            id:transTxt
            Layout.column: 1
            Layout.row: 9
            text: qsTr("传输")
            font.pointSize: 12
            font.family: "Times New Roman"
        }

        StatusIndicator {
            id: equiComIndicator
            Layout.column: 0
            Layout.row: 10
            color: "yellow"
            active: true

            MouseArea
            {
                anchors.fill: parent
                onClicked: showOnlineState()
            }
        }

        Text {
            id: equiComTxt
            Layout.column: 1
            Layout.row: 10
            text: qsTr("主机通信")
            font.pointSize: 12
            font.family: "Times New Roman"
        }

        StatusIndicator {
            id: centerComIndictor
            Layout.column: 0
            Layout.row: 11
            color: "yellow"
            active: true

        }
        Text {
            id: centerComTxt
            Layout.column: 1
            Layout.row: 11
            text: qsTr("中心通信")
            font.pointSize: 12
            font.family: "Times New Roman"
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
            id:resetBtn
            anchors.leftMargin: 20
            width: parent.width
            // Layout.fillHeight: true
            font.pointSize: 12
            font.family: "Times New Roman"
            text: qsTr("复位")

            onClicked: {
                emit: reset()
            }


        }

        NaviButton {
            id:alarmPlanBtn
            anchors.leftMargin: 20
            width: parent.width
            // Layout.fillHeight: true
            font.pointSize: 12
            font.family: "Times New Roman"
            text: qsTr("报警平面")
            onClicked: {
                ArchitePlanView.toAlarmView()

                /*测试*/
                // CrtWidget.sendSeralData()
            }

        }

        NaviButton {
            id: previousBtn
            anchors.leftMargin: 20
            width: parent.width
            // Layout.fillHeight: true
            font.pointSize: 11
            font.family: "Times New Roman"
            text: qsTr("上一页")
            onClicked: {
                ArchitePlanView.toPreviousPage()
            }

        }

        NaviButton {
            id: nextBtn
            anchors.leftMargin: 20
            width: parent.width
            font.pointSize: 11
            font.family: "Times New Roman"
            text: qsTr("下一页")
            onClicked: {
                ArchitePlanView.toNextPage()
            }

        }

        Text {
            id: pageTxt
            anchors.leftMargin: 20
            width: parent.width
            font.pointSize: 11
            font.family: "Times New Roman"
            text: qsTr("总0页/第0页")
        }





        CheckBox {
            id: autoSwitchCheckBox
            text: qsTr("自动切换")
            width: parent.width
            font.pointSize: 11
            font.family: "Times New Roman"
            onClicked: {
                ArchitePlanView.startAutoSwitch(checked)
            }
        }

    }




    function currentAlarm() {
        return alarmTypeComboBox.currentText
    }

    function setPage(totalPage, currentPage) {

        if(totalNum==totalPage&&curNum==currentPage)
        {
           return
        }
        else
        {
            totalNum=totalPage
            curNum=currentPage
            pageTxt.text = String(qsTr("总%1页/第%2页")).arg(totalPage).arg(currentPage)
        }
    }


    function enableToPreviousPageBtn(isEnable) {
        if(previousBtn.enabled!=isEnable)
        {
            previousBtn.enabled = isEnable
        }
    }

    function enableToNextPageBtn(isEnable) {
        if(nextBtn.enabled!=isEnable)
        {
           nextBtn.enabled = isEnable
        }

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

    function setRespondColor(isActived, currentColor) {
        if(!Qt.colorEqual(respondStatusIndicator.color,currentColor))
        {
            respondStatusIndicator.color = currentColor
            respondStatusIndicator.active = isActived
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



    function startTransformAnimation(isRunning) {
        if (isRunning) {
            if (!transformAnimation.running)
                transformAnimation.start()
        } else {
            transformAnimation.stop()
        }
    }

    function setFireAlarmText(value) {
        if(fireNum != value)
        {
            var txt = qsTr("火警 ") + String("%1").arg(value)
            fireNum = value
            fireAlarmNum.text = txt
        }
    }

    function setLinkageText(value) {
        if(startNum!=value)
        {
            var txt = qsTr("启动 ") + String("%1").arg(value)
            startNum= value
            linkageNum.text = txt
        }
    }

    function setSuperviseText(value) {
        if(supervisionNum!=value)
        {
            var txt = qsTr("监管 ") + String("%1").arg(value)
            supervisionNum = value
            superviseNum.text = txt
        }
    }

    function setFaultText(value) {
        if(failureNum!=value)
        {
            var txt = qsTr("故障 ") + String("%1").arg(value)
            failureNum = value
            faultNum.text = txt
        }
    }

    function setRespondText(value) {
        if(respondingNum!=value)
        {
            var txt = qsTr("反馈 ") + String("%1").arg(value)
            respondingNum = value
            respondNum.text = txt
        }
    }

    function setShieldText(value) {
        if(shieldingNum!=value)
        {
            var txt = qsTr("屏蔽 ") + String("%1").arg(value)
            shieldingNum = value
            shieldNum.text = txt
        }
    }


    function setHandOrAutoText(value)
    {
        handOrAutoTxt.text = value
        curHandOrAutoTxt = value
    }

    function allAlarmClear(fireAlarmClear) {

        if(fireAlarmClear)
        {
            setFireAlarmColor(true, "gray")
            setLinkageAlarmColor(true, "gray")
        }
        setSuperviseAlarmColor(true, "gray")
        setfaultAlarmColor(true, "gray")
        setRespondColor(true, "gray")
        setShieldAlarmColor(true, "gray")
        setMainPowerColor(true, "green")
        setStandbyPowerColor(true, "green")
        setTransformColor(true,"gray")
        setEquiComColor(true, "yellow")
        // setCenterComColor(true, "gray")
        setHandOrAutoColor(true,"green")
        setHandOrAutoText(qsTr("自动"))
        setFireAlarmText(0)
        setLinkageText(0)
        setSuperviseText(0)
        setFaultText(0)
        setRespondText(0)
        setShieldText(0)
        setAutoSwitchCheckBoxState(false)
    }

    function setAutoSwitchCheckBoxState(isChecked) {
        autoSwitchCheckBox.checked = isChecked
    }

    function retranslate()
    {
        firmAlarmBtn.text = qsTr("首火警")
        erasureBtn.text = qsTr("消音")
        fireAlarmNum.text = qsTr("火警 ")+String("%1").arg(fireNum)
        superviseNum.text = qsTr("监管 ")+String("%1").arg(supervisionNum)
        linkageNum.text = qsTr("启动 ")+String("%1").arg(startNum)
        respondNum.text = qsTr("反馈 ")+String("%1").arg(respondingNum)
        faultNum.text = qsTr("故障 ")+String("%1").arg(failureNum)
        shieldNum.text = qsTr("屏蔽 ")+String("%1").arg(shieldingNum)
        mainPowerTxt.text = qsTr("主电")
        standbyPowerTxt.text = qsTr("备电")
        if(curHandOrAutoTxt=="自动"||curHandOrAutoTxt=="Automatic")
        {
            handOrAutoTxt.text =qsTr("自动")
        }
        else if(curHandOrAutoTxt=="手动"||curHandOrAutoTxt=="Manual")
        {
            handOrAutoTxt.text =qsTr("手动")
        }
        else if(curHandOrAutoTxt=="默认"||curHandOrAutoTxt=="Default")
        {
            handOrAutoTxt.text =qsTr("默认")
        }


        transTxt.text = qsTr("传输")
        equiComTxt.text = qsTr("主机通信")
        centerComTxt.text = qsTr("中心通信")
        resetBtn.text = qsTr("复位")
        alarmPlanBtn.text = qsTr("报警平面")
        previousBtn.text = qsTr("上一页")
        nextBtn.text = qsTr("下一页")
        pageTxt.text = qsTr("总%1页/第%2页").arg(totalNum).arg(curNum)
        autoSwitchCheckBox.text = qsTr("自动切换")
        if(CrtWidget.isEnglish())
        {
            fireAlarmNum.font.pointSize =11
            superviseNum.font.pointSize=11
            linkageNum.font.pointSize =11
            respondNum.font.pointSize =11
            faultNum.font.pointSize =11
            shieldNum.font.pointSize=11
            pageTxt.font.pointSize=11
            autoSwitchCheckBox.font.pointSize =10
        }
        else
        {
            fireAlarmNum.font.pointSize =12
            superviseNum.font.pointSize=12
            linkageNum.font.pointSize =12
            respondNum.font.pointSize =12
            faultNum.font.pointSize =12
            shieldNum.font.pointSize=12
            pageTxt.font.pointSize=12
            autoSwitchCheckBox.font.pointSize =12

        }
    }
}

