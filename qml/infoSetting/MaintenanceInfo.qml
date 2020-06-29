import QtQuick 2.0
import QtQuick.Controls 2.2
import operatorInfo 1.0
import QtQuick.Controls 1.4 as Controls1_4
import "qrc:/jsFile/JsDateTime.js" as JsDateTime
Item {
    width: 800
    height: 640
    Row {
        id: maintenanceRow
        anchors.top: parent.top
        anchors.topMargin: 60
        spacing: 3
        Text {
            id:deviceCodeTxt
            text: qsTr("设备编码:")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }
        TextField {
            id: codeTextField
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }

        Text {
            id:maintenanceTimeTxt
            text: qsTr("维保时间:")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }

        Text {
            id: yearTxt
            text: qsTr("年:")
        }

        Controls1_4.SpinBox
        {
            id:yearSpinBox
            maximumValue: 2200
            minimumValue: 1990
            value: Qt.formatDate(new Date,"yyyy")
            width: 60
            height: 25

            onValueChanged:
            {
                setDate()
            }

        }
        Text {
            id: monthTxt
            text: qsTr("月:")
        }

        Controls1_4.SpinBox
        {

            id:monthSpinBox
            maximumValue: 12
            minimumValue: 1
            value: Qt.formatDate(new Date,"M")
            width: 60
            height: 25
            onValueChanged:
            {
                setDate()
            }
        }

        Text {
            id: dateTxt
            text: qsTr("日:")
        }

        Controls1_4.SpinBox
        {
            id:dateSpinBox
            maximumValue: 31
            minimumValue: 1
            value: Qt.formatDate(new Date,"d")
            width: 60
            height: 25

        }

//        TextField {
//            id: timeTextField
//            placeholderText: qsTr("格式:2019/01/01 00:00:00")
//        }

        Text {
            id:maintenPersonTxt
            text: qsTr("维保员:")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }

        TextField {
            id: engineerTextField
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
    }
    Grid {
        id: maintenanceGrid
        anchors.top: maintenanceRow.bottom
        anchors.topMargin: 5
        columns: 2
        spacing: 5

        Text {
            id:statePhenomenonTxt
            text: qsTr("状态现象")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }

        TextArea {

            id: stateTextArea
            width: maintenanceRow.width
            height: 100
            background: Rectangle {
                border.width: 1
                color: "transparent"
            }
        }

        Text {
            id:maintenMethod
            text: qsTr("维保方法")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }
        TextArea {
            id: methodsTextArea
            width: maintenanceRow.width
            height: 100
            background: Rectangle {
                border.width: 1
                color: "transparent"
            }
        }

        Text {
            id:contentDescriptionTxt
            text: qsTr("内容描述")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }

        TextArea {
            id: contentDescTextArea
            width: maintenanceRow.width
            height: 100
            background: Rectangle {
                border.width: 1
                color: "transparent"
            }
        }
    }

    Grid {
        id: selectInfoRow
        columns: 8
        spacing: 5
        anchors.top: maintenanceGrid.bottom
        anchors.topMargin: 5
        Text {
            id:buildingNameTxt
            text: qsTr("建筑名称")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }

        TextField {
            id: buildingNameTextField
            width: 150
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
        Text {
            id:floorTxt
            text: qsTr("楼层")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }

        TextField {
            id: floorTextField
            width: 150
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }

        Text {
            id:positionTxt
            text: qsTr("部位")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }

        TextField {
            id: locationTextField
            width: 150
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }

        Text {
            id:sysTxt
            text: qsTr("系统")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }

        ComboBox {
            id: sysComboBox
            currentIndex: -1
            width: 150
            model: [qsTr("电气火灾监控系统"),qsTr("火灾探测报警系统"), qsTr("消防联动系统"), qsTr("自动喷水灭火系统"), qsTr("水喷雾灭火系统"), qsTr("消火栓系统"), qsTr("气体灭火系统"), qsTr("泡沫灭火系统"), qsTr("干粉灭火系统"), qsTr("防烟排烟系统"), qsTr("防火及卷帘系统"), qsTr("电梯系统"), qsTr("消防电话系统"), qsTr("消防应急广播系统"), qsTr("消防应急照明和疏散指示系统"), qsTr("消防电源系统")]
        }

        Text {
            id:personOndutyTxt
            text: qsTr("值班人员")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }

        TextField {
            id: personOnDutyTextField
            width: 150
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }

        Text {
            id:operatorTxt
            text: qsTr("操作人员")
            verticalAlignment: Text.AlignVCenter
            height: 30
        }

        TextField {
            id: operatorTextField
            width: 150
            selectByMouse: true
            selectionColor: "blue"
            selectedTextColor: "white"
        }
    }

    NaviButton {
        id:saveBtn
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: selectInfoRow.bottom
        anchors.topMargin: 5
        text: qsTr("保存")
        onClicked: {


            var dateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3").arg(yearSpinBox.value).arg(monthSpinBox.value).arg(dateSpinBox.value),"yyyy/M/d");
            var date = new Date(dateValue)
            var dateStr=Qt.formatDateTime(date,"yyyy/MM/dd");

            OperatorInfo.setMaintInfoValue(qsTr("设备编码"), codeTextField.text)
            OperatorInfo.setMaintInfoValue(qsTr("维保时间"), dateStr)

            OperatorInfo.setMaintInfoValue(qsTr("维保员"), engineerTextField.text)
            OperatorInfo.setMaintInfoValue(qsTr("状态现象"), stateTextArea.text)

            OperatorInfo.setMaintInfoValue(qsTr("维保方法"), methodsTextArea.text)
            OperatorInfo.setMaintInfoValue(qsTr("内容描述"),
                                           contentDescTextArea.text)

            OperatorInfo.setMaintInfoValue(qsTr("建筑名称"),
                                           buildingNameTextField.text)
            OperatorInfo.setMaintInfoValue(qsTr("楼层"), floorTextField.text)

            OperatorInfo.setMaintInfoValue(qsTr("部位"), locationTextField.text)
            OperatorInfo.setMaintInfoValue(qsTr("系统"), sysComboBox.currentText)

            OperatorInfo.setMaintInfoValue(qsTr("值班人员"),
                                           personOnDutyTextField.text)
            OperatorInfo.setMaintInfoValue(qsTr("操作人员"), operatorTextField.text)
            OperatorInfo.saveMaintInfo()
        }
    }

    Component.onCompleted:
    {
      setDate()

    }

    function equipmentCode() {
        return codeTextField.text
    }


    function statePhenomenon() {
        return stateTextArea.text
    }

    function methods() {
        return methodsTextArea.text
    }

    function contentDesc() {
        return contentDescTextArea.text
    }
    function buildingName() {
        return buildingNameTextField.text
    }

    function floor() {
        return floorTextField.text
    }

    function location() {
        return locationTextField.text
    }

    function sysOfDevice() {
        return sysComboBox.currentText
    }

    function setEquipmentCode(code) {
        codeTextField.text = code
    }


    function setBuildingName(buildingName) {
        buildingNameTextField.text = buildingName
    }

    function setFloor(floor) {
        floorTextField.text = floor
    }

    function setLocation(location) {
        locationTextField.text = location
    }

    function setSysOfDevice(sys) {
        sysComboBox.currentIndex = sysComboBox.find(sys)
    }
    function setOperator(operator) {
        operatorTextField.text = operator
    }
    function setPersonOnDuty(person) {
        personOnDutyTextField.text = person
    }

    function retranslate()
    {
       deviceCodeTxt.text = qsTr("设备编码:")
       maintenanceTimeTxt.text = qsTr("维保时间:")
       maintenPersonTxt.text = qsTr("维保员:")
       statePhenomenonTxt.text = qsTr("状态现象")
       maintenMethod.text = qsTr("维保方法")
        contentDescriptionTxt.text = qsTr("内容描述")
        buildingNameTxt.text = qsTr("建筑名称")
        floorTxt.text = qsTr("楼层")
        positionTxt.text = qsTr("部位")
        sysTxt.text = qsTr("系统")
        sysComboBox.model= [qsTr("电气火灾监控系统"),qsTr("火灾探测报警系统"), qsTr("消防联动系统"), qsTr("自动喷水灭火系统"), qsTr("水喷雾灭火系统"), qsTr("消火栓系统"), qsTr("气体灭火系统"), qsTr("泡沫灭火系统"), qsTr("干粉灭火系统"), qsTr("防烟排烟系统"), qsTr("防火及卷帘系统"), qsTr("电梯系统"), qsTr("消防电话系统"), qsTr("消防应急广播系统"), qsTr("消防应急照明和疏散指示系统"), qsTr("消防电源系统")]
        personOndutyTxt.text = qsTr("值班人员")
        operatorTxt.text = qsTr("操作人员")
        saveBtn.text = qsTr("保存")
        yearTxt.text = qsTr("年:")
        monthTxt.text = qsTr("月:")
        dateTxt.text = qsTr("日:")

    }

    function setDate()
    {
        var curYear = yearSpinBox.value
        var curMonth = monthSpinBox.value

        var maxDate=JsDateTime.getMaxDate(curYear,curMonth)
        dateSpinBox.maximumValue=maxDate
        if(dateSpinBox.value>maxDate)
        {
            dateSpinBox.value = maxDate
        }

    }
}
