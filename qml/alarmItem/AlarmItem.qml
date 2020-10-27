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
import QtQuick.Controls 1.4 as Controls1_4

import "../infoSetting"
//import QtQuick.Controls 1.4 as Controls1_4
Rectangle {
    width: 250
    anchors.fill: parent
    signal currentAlarmType(string type)
    signal startAutoSwitch(bool isAuto)
    signal reset
    signal clearVoice()
    signal reSendCmd()
    signal mainPowerViewShow()
    signal standbyPowerViewShow()
    signal handOrAutoStateViewShow()
    signal showOnlineState()
    signal checkTheVersion()
    signal selectCurAlarm(string curAlarm)
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
    property  string curHandOrAutoTxt: qsTr("默认")

    color: "transparent"
    RowLayout {
        id: alarmBtnLayout
        height: 40
        width: parent.width



        Button  {
            id: firmAlarmBtn
            Layout.alignment: Qt.AlignLeft
            Layout.topMargin: 5
            text: qsTr("首火警")
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: 2

            flat:false
            background: Rectangle
            {
                id:backRect

                implicitWidth: 80
                implicitHeight: 40
                color:parent.highlighted ?  "#E17F7F":"#FF4D3F"
                radius:10
                border.width: 1


            }

            onPressed: {
                highlighted = true
            }
            onReleased: {
               highlighted = false

            }
            onClicked: {
                ArchitePlanView.firstFireAlarm()
            }
        }

        Button  {
            id: erasureBtn
            // anchors.leftMargin: 20
            Layout.alignment: Qt.AlignCenter

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 5
            //font.pointSize:11
            text: qsTr("消音")
            highlighted: true

            flat:false
            background: Rectangle
            {
                //id:backRect

                implicitWidth: 80
                implicitHeight: 40
                color:parent.highlighted ? "#4a87ee" : "#39afc3"
                radius:10
                border.width: 1
               // border.color:"lightGray"

            }
            onPressed: {
                highlighted = false
            }
            onReleased: {
               highlighted = true

            }

            onClicked: {
                emit:clearVoice()
            }
        }

        Button {
            id:resetBtn
            anchors.leftMargin: 2
            Layout.alignment: Qt.AlignRight
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 5
            Layout.rightMargin: 5

            //font.pointSize: 11
            // font.family: "Times New Roman"
            text: qsTr("复位")
            flat:false
            background: Rectangle
            {
                //id:backRect

                implicitWidth: 80
                implicitHeight: 40
                color:parent.highlighted ?  "#cb9c36":"#f0b840"
                radius:10
                border.width: 1
               // border.color:"lightGray"

            }
            onPressed: {
                highlighted = true
            }
            onReleased: {
               highlighted = false

            }

            onClicked: {
                emit: reset()
            }
        }
    }

    GroupBox
    {
        anchors.top: alarmBtnLayout.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        id:alarmIndicatorGroupBox
        anchors.topMargin: 5
        width: parent.width
        background: Rectangle
        {
            color:"transparent"
            border.width:1

        }
        GridLayout {
            id: alarmIndicatorLayout
            anchors.fill: parent

            Layout.fillWidth: true

            columnSpacing: 5
            rowSpacing: 10
            StatusIndicator {
                id: fireAlarmStatusIndicator
                Layout.column: 0
                Layout.row: 0
                color: "gray" //火警:绿色:正常,红色:报警
                active: true
                Layout.preferredWidth: 20
                Layout.preferredHeight: 20


//                MouseArea
//                {
//                    anchors.fill: parent
//                    onClicked:
//                    {
//                        emit:selectCurAlarm(qsTr("火警"))
//                    }

//                }
            }
            Controls1_4.Button {
                id: fireAlarmNum
                Layout.column: 1
                Layout.row: 0
                text: qsTr("火警 0")
                style: ButtonStyle {
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 10
                        text: fireAlarmNum.text
                    }
                }
                onClicked:
                {
                    emit:selectCurAlarm(qsTr("火警"))
                }
               // font.pointSize: 11
                //font.family: "Times New Roman"
            }

            StatusIndicator {
                id: superviseStatusIndicator
                Layout.column: 2
                Layout.row: 0
                color: "gray" //监管:绿色:正常
                active: true
                Layout.preferredWidth: 20
                Layout.preferredHeight: 20


            }
            Controls1_4.Button {
                id: superviseNum
                Layout.column: 3
                Layout.row: 0
                text: qsTr("监管 0")
                onClicked:
                {
                    emit:selectCurAlarm(qsTr("监管"))
                }
                style: ButtonStyle {
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 10
                        text: superviseNum.text
                    }
                }
               // font.pointSize: 11
                //font.family: "Times New Roman"
            }

            StatusIndicator {
                id: linkageStatusIndicator
                Layout.column: 0
                Layout.row: 1
                color: "gray" //启动:绿色:正常,红色:报警
                active: true
                Layout.preferredWidth: 20
                Layout.preferredHeight: 20
//                MouseArea
//                {
//                    anchors.fill: parent
//                    onClicked:
//                    {
//                        emit:selectCurAlarm(qsTr("启动"))
//                    }

//                }

            }
            Controls1_4.Button {
                id: linkageNum
                Layout.column: 1
                Layout.row: 1
                text: qsTr("启动 0")
                onClicked:
                {
                    emit:selectCurAlarm(qsTr("启动"))
                }
                style: ButtonStyle {
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 10
                        text: linkageNum.text
                    }
                }

                //font.pointSize: 11
                //font.family: "Times New Roman"
            }

            StatusIndicator {
                id: respondStatusIndicator
                Layout.column: 2
                Layout.row: 1
                color: "gray" //反馈:绿色:正常
                active: true
                Layout.preferredWidth: 20
                Layout.preferredHeight: 20

//                MouseArea
//                {
//                    anchors.fill: parent
//                    onClicked:
//                    {
//                        emit:selectCurAlarm(qsTr("反馈"))
//                    }

//                }


            }
            Controls1_4.Button {
                id: respondNum
                Layout.column: 3
                Layout.row: 1
                text: qsTr("反馈 0")
                onClicked:
                {
                    emit:selectCurAlarm(qsTr("反馈"))
                }
                style: ButtonStyle {
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 10
                        text: respondNum.text
                    }
                }
                //font.pointSize: 11
                //font.family: "Times New Roman"
            }

            StatusIndicator {
                id: faultStatusIndicator
                Layout.column: 0
                Layout.row: 2
                color: "gray" //故障:绿色:正常,黄色:故障
                active: true
                Layout.preferredWidth: 20
                Layout.preferredHeight: 20
//                MouseArea
//                {
//                    anchors.fill: parent
//                    onClicked:
//                    {
//                        emit:selectCurAlarm(qsTr("故障"))
//                    }

//                }

            }
            Controls1_4.Button {
                id: faultNum
                Layout.column: 1
                Layout.row: 2
                text: qsTr("故障 0")
                onClicked:
                {
                    emit:selectCurAlarm(qsTr("故障"))
                }
                style: ButtonStyle {
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 10
                        text:faultNum.text
                    }
                }
                //font.pointSize: 11
                //font.family: "Times New Roman"
            }



            StatusIndicator {
                id: shieldStatusIndicator
                Layout.column: 2
                Layout.row: 2
                color: "gray" //屏蔽:绿色:正常
                active: true
                Layout.preferredWidth: 20
                Layout.preferredHeight: 20
//                MouseArea
//                {
//                    anchors.fill: parent
//                    onClicked:
//                    {
//                        emit:selectCurAlarm(qsTr("屏蔽"))
//                    }

//                }

            }
            Controls1_4.Button {
                id: shieldNum
                Layout.column: 3
                Layout.row: 2
                text: qsTr("屏蔽 0")
                onClicked:
                {
                    emit:selectCurAlarm(qsTr("屏蔽"))
                }

                style: ButtonStyle {
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 10
                        text:shieldNum.text
                    }
                }
                //font.pointSize: 11
                //font.family: "Times New Roman"
            }

            StatusIndicator {
                id: mainPowerIndicator
                Layout.column: 0
                Layout.row: 3
                color: "green"
                active: true
                Layout.preferredWidth: 20
                Layout.preferredHeight: 20
//                MouseArea
//                {
//                    anchors.fill: parent
//                    acceptedButtons: Qt.LeftButton
//                    onClicked:
//                    {
//                        emit:mainPowerViewShow()
//                    }

//                }
            }
            Controls1_4.Button {
                id: mainPowerTxt
                Layout.column: 1
                Layout.row: 3
                text: qsTr("主电")
                onClicked:
                {
                    emit:mainPowerViewShow()
                }
                style: ButtonStyle {
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 10
                        text:mainPowerTxt.text
                    }
                }
                //font.pointSize: 11
                //font.family: "Times New Roman"
            }

            StatusIndicator {
                id: standbyPowerIndicator
                Layout.column: 2
                Layout.row: 3
                color: "green"
                active: true
                Layout.preferredWidth: 20
                Layout.preferredHeight: 20

//                MouseArea
//                {
//                    anchors.fill: parent
//                    acceptedButtons: Qt.LeftButton
//                    onClicked:
//                    {
//                        emit:standbyPowerViewShow()
//                    }

//                }
            }
            Controls1_4.Button {
                id: standbyPowerTxt
                Layout.column: 3
                Layout.row: 3
                text: qsTr("备电")
                style: ButtonStyle {
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 10
                        text:standbyPowerTxt.text
                    }
                }
                onClicked:
                {
                    emit:standbyPowerViewShow()
                }
                //font.pointSize: 11
                //font.family: "Times New Roman"
            }


            StatusIndicator {
                id: handOrAutoIndicator//默认时紫色，手动蓝色，自动绿色。
                Layout.column: 0
                Layout.row: 4
                color: "purple"
                active: true
                Layout.preferredWidth: 20
                Layout.preferredHeight: 20

//                MouseArea
//                {
//                    anchors.fill: parent
//                    acceptedButtons: Qt.LeftButton
//                    onClicked:
//                    {
//                        emit:handOrAutoStateViewShow()
//                    }

//                }

            }
            Controls1_4.Button {
                id: handOrAutoTxt
                //flat: true
                Layout.column: 1
                Layout.row: 4
                text: qsTr("默认")
                style: ButtonStyle {
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 10
                        text:handOrAutoTxt.text
                    }
                }
                onClicked:
                {
                    emit:handOrAutoStateViewShow()
                }
                //font.pointSize: 11
                //font.family: "Times New Roman"

            }



            StatusIndicator {
                id: transformIndicator //传输指示，传输正常：绿色；异常：红色；传输过程中闪烁。
                Layout.column: 2
                Layout.row: 4
                color: "gray"
                active: true
                Layout.preferredWidth: 20
                Layout.preferredHeight: 20
                ColorAnimation on color {
                    id: transformAnimation
                    from: "gray"
                    to: "green"
                    duration: 500
                    loops: Animation.Infinite
                    running: false
                }

                onColorChanged:
                {
                    if(colorChangeNum%50==0)
                    {
                        curState = !curState
                        CrtWidget.transportIndicator(curState)
                    }
                    colorChangeNum +=1
                }
            }

            Text {
                id:transTxt
                Layout.column: 3
                Layout.row: 4
                text: qsTr("传输")
                font.pointSize: 11
                //font.family:"Times New Roman"
            }

            StatusIndicator {
                id: equiComIndicator
                Layout.column: 0
                Layout.row: 5
                color: "yellow"
                active: true
                Layout.preferredWidth: 20
                Layout.preferredHeight: 20

//                MouseArea
//                {
//                    anchors.fill: parent
//                    onClicked: showOnlineState()
//                }
            }

            Controls1_4.Button {
                id: equiComTxt
                Layout.column: 1
                Layout.row: 5
                text: qsTr("主机通信")
                //font.pointSize: 11
                onClicked: showOnlineState()
                style: ButtonStyle {
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 10
                        text:equiComTxt.text
                    }
                }
                //font.family: "Times New Roman"
            }

            StatusIndicator {
                id: centerComIndictor
                Layout.column: 2
                Layout.row: 5
                color: "yellow"
                active: true
                Layout.preferredWidth: 20
                Layout.preferredHeight: 20

            }
            Text {
                id: centerComTxt
                Layout.column: 3
                Layout.row: 5
                text: qsTr("中心通信")
                font.pointSize: 11
                //font.family: "Times New Roman"
            }
        }
    }

    GroupBox
    {
        id:pageGroupBox
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: alarmIndicatorGroupBox.bottom
        anchors.topMargin: 5
        width: alarmIndicatorGroupBox.width
        background: Rectangle
        {
            color:"transparent"
            border.width:1
        }
        Column {

            width: parent.width
            spacing: 5
            anchors.fill: parent
            anchors.horizontalCenter: parent.horizontalCenter
            Layout.fillWidth: true

            Row
            {
                spacing: 5
                width: parent.width
                Layout.fillHeight: true
                Layout.fillWidth: true
                NaviButton {
                    id: previousBtn
                    anchors.leftMargin:5
                    width: 100
                    Layout.fillWidth: true
                    //width: parent.width
                    // Layout.fillHeight: true
                    font.pointSize: 11
                    //font.family: "Times New Roman"
                    text: qsTr("上一页")
                    onClicked: {
                        ArchitePlanView.toPreviousPage()
                    }

                }

                NaviButton {
                    id: nextBtn
                    anchors.leftMargin: 5
                    width: 100
                    Layout.fillWidth: true
                    //width: parent.width
                    font.pointSize: 11
                    //font.family: "Times New Roman"
                    text: qsTr("下一页")
                    onClicked: {
                        ArchitePlanView.toNextPage()
                    }

                }
            }

            Text {
                id: pageTxt
                // anchors.leftMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 5
                //anchors.horizontalCenter: parent.horizontalCenter

                width: parent.width
                font.pointSize: 11
                //font.family: "Times New Roman"
                text: qsTr("总0页/第0页")
            }





            CheckBox {
                id: autoSwitchCheckBox
                text: qsTr("自动切换")
                width: parent.width
                font.pointSize: 11
                //font.family: "Times New Roman"
                onClicked: {
                    ArchitePlanView.startAutoSwitch(checked)
                    //CrtWidget.sendSerialData()//用来测试

                }
            }



            //        Text {
            //            id: verValue
            //            text: qsTr("通讯板程序版本:")
            //        }


        }
    }

    NaviButton {
        id: checkVerBtn
        anchors.left: parent.left
        anchors.top: pageGroupBox.bottom
        anchors.topMargin: 5
        anchors.leftMargin:10
        width: 150
        font.pointSize: 10
        //font.family: "Times New Roman"
        text: qsTr("查询LD6901-A版本")
        onClicked:checkTheVersion()

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
        setHandOrAutoColor(true,"purple")
        setHandOrAutoText(qsTr("默认"))
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
        //alarmPlanBtn.text = qsTr("报警平面")
        previousBtn.text = qsTr("上一页")
        nextBtn.text = qsTr("下一页")
        pageTxt.text = qsTr("总%1页/第%2页").arg(totalNum).arg(curNum)
        autoSwitchCheckBox.text = qsTr("自动切换")
        checkVerBtn.text = qsTr("查询LD6901-A版本")
        if(CrtWidget.isEnglish())
        {
//            fireAlarmNum.font.pointSize =8
//            superviseNum.font.pointSize=8
//            linkageNum.font.pointSize =8
//            respondNum.font.pointSize =8
//            faultNum.font.pointSize =8
//            shieldNum.font.pointSize=8
//            pageTxt.font.pointSize=8
//            mainPowerTxt.font.pointSize=8
//            autoSwitchCheckBox.font.pointSize =10
//            standbyPowerTxt.font.pointSize =8
            transTxt.font.pointSize=8
//            mainPowerTxt.font.pointSize=8
//            equiComTxt.font.pointSize=8
            centerComTxt.font.pointSize=8
//            handOrAutoTxt.font.pointSize=8
            //firmAlarmBtn.font.pointSize=9
            //erasureBtn.font.pointSize=9
           // resetBtn.font.pointSize=9
            nextBtn.font.pointSize=9
            previousBtn.font.pointSize=9
            checkVerBtn.font.pointSize=8
        }
        else
        {
//            fireAlarmNum.font.pointSize =11
//            superviseNum.font.pointSize=11
//            linkageNum.font.pointSize =11
//            respondNum.font.pointSize =11
//            faultNum.font.pointSize =11
//            shieldNum.font.pointSize=11
//            pageTxt.font.pointSize=11
//            autoSwitchCheckBox.font.pointSize =11
//            standbyPowerTxt.font.pointSize =11
            transTxt.font.pointSize=11
//            mainPowerTxt.font.pointSize=11
//            equiComTxt.font.pointSize=11
            centerComTxt.font.pointSize=11
//            handOrAutoTxt.font.pointSize=11
            //firmAlarmBtn.font.pointSize=11
            //erasureBtn.font.pointSize=11
            //resetBtn.font.pointSize=11
            nextBtn.font.pointSize=11
            previousBtn.font.pointSize=11
            checkVerBtn.font.pointSize=11
   }
    }

    //    function setProgramVer(value)
    //    {
    //        verValue.text = qsTr("LD6901-A版本:%1").arg(value)
    //    }
}

