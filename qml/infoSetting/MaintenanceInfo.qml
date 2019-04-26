import QtQuick 2.0
import QtQuick.Controls 2.2
import operatorInfo 1.0

Rectangle {
    width: 900
    height: 560
    Row {
        id: maintenanceRow
        anchors.top: parent.top
        anchors.topMargin: 10
        spacing: 3
        Text {
            text: qsTr("设备编码:")
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: codeTextField
        }

        Text {
            text: qsTr("维保时间:")
        }

        TextField {
            id: timeTextField
            placeholderText: qsTr("格式:2019/01/01 00:00:00")
        }

        Text {
            text: qsTr("维保员:")
        }

        TextField {
            id: engineerTextField
        }
    }
    Grid {
        id: maintenanceGrid
        anchors.top: maintenanceRow.bottom
        anchors.topMargin: 5
        columns: 2
        spacing: 5

        Text {
            text: qsTr("状态现象")
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
            text: qsTr("维保方法")
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
            text: qsTr("内容描述")
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
            text: qsTr("建筑名称")
        }

        TextField {
            id: buildingNameTextField
            width: 150
        }
        Text {
            text: qsTr("楼层")
        }

        TextField {
            id: floorTextField
            width: 150
        }

        Text {
            text: qsTr("部位")
        }

        TextField {
            id: locationTextField
            width: 150
        }

        Text {
            text: qsTr("系统")
        }

        ComboBox {
            id: sysComboBox
            currentIndex: -1
            width: 150
            model: ["火灾探测报警系统", "消防联动系统", "自动喷水灭火系统", "水喷雾灭火系统", "消火栓系统", "气体灭火系统", "泡沫灭火系统", "干粉灭火系统", "防烟排烟系统", "防火及卷帘系统", "电梯系统", "消防电话系统", "消防应急广播系统", "消防应急照明和疏散指示系统", "消防电源系统"]
        }

        Text {
            text: qsTr("值班人员")
        }

        TextField {
            id: personOnDutyTextField
            width: 150
        }

        Text {
            text: qsTr("操作人员")
        }

        TextField {
            id: operatorTextField
            width: 150
        }
    }

    Button {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: selectInfoRow.bottom
        anchors.topMargin: 5
        text: qsTr("保存")
        onClicked: {
            OperatorInfo.setMaintInfoValue(qsTr("设备编码"), codeTextField.text)
            OperatorInfo.setMaintInfoValue(qsTr("维保时间"), timeTextField.text)

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

    function equipmentCode() {
        return codeTextField.text
    }

    function maintTime() {
        return timeTextField.text
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

    function setMaintTime(dateTime) {
        timeTextField.text = dateTime
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
}
