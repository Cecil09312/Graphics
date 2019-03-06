import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import itemIconInfoToJson 1.0

Rectangle {
    width: 640
    height: 560
    signal setSize(real size)
    signal setIcon(string icon)
    signal setItemInfo(string type, string value)

    ListModel {
        id: deviceTypeModel
        ListElement {
            deviceName: qsTr("报警装置")
        }
    }

    SwipeView {
        id: swipView
        currentIndex: 0
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.bottomMargin: 20
        anchors.topMargin: 40
        interactive: false
        Item {

            antialiasing: true
            Grid {
                anchors.fill: parent
                anchors.bottomMargin: 40
                anchors.topMargin: 40
                columnSpacing: 5
                rowSpacing: 5
                columns: 4
//                Text {

//                    text: qsTr("状态:")
//                    height: 40
//                    verticalAlignment: Text.AlignVCenter
//                }
//                ComboBox {
//                    id: alarmTypeComboBox
//                    Layout.fillWidth: true
//                    width: 150
//                    height: 40
//                    model: [qsTr("正常"), qsTr("火警"), qsTr("联动"), qsTr("监管"), qsTr(
//                            "故障"), qsTr("反馈"), qsTr("屏蔽")]
//                    onCurrentTextChanged: {
//                        emit: setItemInfo("alarmType", currentText)
//                    }
//                }

                Text {
                    text: qsTr("设备型号:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                ComboBox {
                    id: equipmentModelComboBox
                    width: 150
                    height: 40
                    model: deviceTypeModel
                    onCurrentTextChanged: {
                        if (currentIndex >= 0) {
                            itemIconInfo.setCurrentIconIndex(currentIndex)

                            emit: setIcon(itemIconInfo.getIconName(
                                              currentIndex))
                            emit: setItemInfo("equipmentModel", currentText)
                        }
                    }
                }
                Text {
                    text: qsTr("分机号:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: extNumTextField
                    width: 150
                    height: 40
                    onTextEdited: {
                        emit: setItemInfo("extNum", extNumTextField.text)
                    }
                }

                Text {
                    text: qsTr("回路号:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: loopNumTextField
                    width: 150
                    height: 40
                    onTextEdited: {
                        emit: setItemInfo("loopNum", loopNumTextField.text)
                    }
                }

                Text {
                    text: qsTr("地址号:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: addrNumTextField
                    width: 150
                    height: 40
                    onTextEdited: {
                        emit: setItemInfo("addrNum", addrNumTextField.text)
                    }
                }

                Text {
                    id: deviceNumTxt
                    text: qsTr("设备编码:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: deviceNumTextField
                    Layout.fillWidth: true
                    width: 150
                    height: 40
                    onTextEdited: {
                        emit: setItemInfo("deviceNum",
                                          deviceLocationTextField.text)
                    }
                }

                Text {
                    text: qsTr("系统:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: deviceSysTextField
                    Layout.fillWidth: true
                    width: 150
                    height: 40
                    onTextEdited: {
                        emit: setItemInfo("sysOfDevice",
                                          deviceSysTextField.text)
                    }
                }

                Text {

                    text: qsTr("建筑名称:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: buildingNameTextField
                    Layout.fillWidth: true
                    width: 150
                    height: 40
                    onTextEdited: {
                        emit: setItemInfo("buildingName",
                                          buildingNameTextField.text)
                    }
                }

                Text {

                    text: qsTr("楼层:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: floorOfDeviceTextField
                    Layout.fillWidth: true
                    width: 150
                    height: 40
                    onTextEdited: {
                        emit: setItemInfo("floorOfDevice",
                                          floorOfDeviceTextField.text)
                    }
                }
                Text {

                    text: qsTr("位置:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: deviceLocationTextField
                    Layout.fillWidth: true
                    width: 150
                    height: 40
                    onTextEdited: {
                        emit: setItemInfo("deviceLocation",
                                          deviceLocationTextField.text)
                    }
                }


                Text {

                    text: qsTr("有效期:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: periodOfValidityTextField
                    Layout.fillWidth: true
                    width: 150
                    height: 40
                    onTextEdited: {
                        emit: setItemInfo("periodOfValidity",
                                          periodOfValidityTextField.text)
                    }
                }

                Text {

                    text: qsTr("制造商:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: manufacturersTextField
                    Layout.fillWidth: true
                    width: 150
                    height: 40
                    onTextEdited: {
                        emit: setItemInfo("manufacturers",
                                          manufacturersTextField.text)
                    }
                }

                Text {
                    id: sizeLabel
                    text: qsTr("大小:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                SpinBox {
                    id: sizeSpinBox
                    Layout.fillWidth: true
                    width: 150
                    height: 40
                    from: 5
                    to: 100
                    stepSize: 1
                    onValueChanged: {
                        emit: setSize(value)
                    }
                }
            }
        }

        ItemIconSetting {
            id: itemIconSetting
        }
    }

    PageIndicator {
        id: indicator
        count: swipView.count
        currentIndex: swipView.currentIndex
        anchors.bottom: swipView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Row {
        anchors.bottom: swipView.top
        anchors.top: parent.top
        spacing: 10
        Button {
            id: iconSettingBtn
            text: qsTr("图标信息设置")

            onClicked: {
                if (swipView.count > 0) {
                    swipView.currentIndex = 0
                    highlighted = true
                    legendBtn.highlighted = false
                }
            }
        }

        Button {
            id: legendBtn
            text: qsTr("图例")
            onClicked: {
                if (swipView.count > 1) {
                    swipView.currentIndex = 1
                    highlighted = true
                    iconSettingBtn.highlighted = false
                }
            }
        }
    }
    ItemIconInfoToJson {
        id: itemIconInfo
    }

    function setExtNum(extNum) {
        extNumTextField.text = extNum
    }

    function setLoopNum(loopNum) {
        loopNumTextField.text = loopNum
    }

    function setAddrNum(addrNum) {
        addrNumTextField.text = addrNum
    }

    function setDeviceNum(deviceNum) {
        deviceNumTextField.text = deviceNum
    }

    function setEquipmentModel(equipmentModel) {
        equipmentModelComboBox.currentIndex = equipmentModelComboBox.find(
                    equipmentModel)
    }

    function setSysOfDevice(sysOfDevice) {
        deviceSysTextField.text = sysOfDevice
    }



    function setBuildingName(buildingName) {
        buildingNameTextField.text = buildingName
    }

    function setFloorOfDevice(floorOfDevice) {
        floorOfDeviceTextField.text = floorOfDevice
    }

    function setDeviceLocation(deviceLocation) {
        deviceLocationTextField.text = deviceLocation
    }

    function setPeriodOfValidity(period)
    {
         periodOfValidityTextField.text = period
    }

    function setManufacturers(manufacturers)
    {
        manufacturersTextField.text = manufacturers
    }



    function setItemSize(size) {
        sizeSpinBox.value = size
    }
    function clearItemInfo() {
        extNumTextField.clear()
        loopNumTextField.clear()
        addrNumTextField.clear()
        deviceNumTextField.clear()
        deviceSysTextField.clear()
        buildingNameTextField.clear()
        floorOfDeviceTextField.clear()
        deviceLocationTextField.clear()
    }

    function currentIconIndex() {
        itemIconInfo.setCurrentIconIndex(equipmentModelComboBox.currentIndex)
        return equipmentModelComboBox.currentIndex
    }

    function readInfo() {

        var size = itemIconInfo.sizeOfHash()
        if (size === 0) {
            deviceTypeModel.clear()
            itemIconInfo.setCurrentIconIndex(-1)
            return
        }
        var itemIconInfoStr = new String
        itemIconInfoStr = itemIconInfo.readFileFromJson()
        if (itemIconInfoStr.length === 0)
            return

        if (size > 0) {
            var modelCount = deviceTypeModel.count
            if (modelCount <= size) {
                for (var j = 0; j < modelCount; j++) {
                    var currentObj = new Object
                    var currentIndex = String("%1").arg(j)
                    currentObj = JSON.parse(itemIconInfoStr)[currentIndex]
                    var currentDeviceObj = new Object

                    currentDeviceObj["deviceName"] = currentObj["deviceName"]
                    deviceTypeModel.set(j, currentDeviceObj)
                    var currentImagePathStr = new String
                    currentImagePathStr = currentObj["imagePath"]
                    itemIconInfo.setIconIndexHash(j, currentImagePathStr)
                }

                for (var i = modelCount; i < size; i++) {
                    var index = String("%1").arg(i)
                    var obj = new Object
                    obj = JSON.parse(itemIconInfoStr)[index]

                    //var currentObj = JSON.parse(obj["deviceName"].toString())
                    var deviceObj = new Object
                    deviceObj["deviceName"] = obj["deviceName"]
                    deviceTypeModel.append(deviceObj)
                    var imagePathStr = new String
                    imagePathStr = obj["imagePath"]
                    itemIconInfo.setIconIndexHash(i, imagePathStr)
                }
            } else {

                deviceTypeModel.remove(size, modelCount - size)
            }
        }
    }

    Component.onCompleted: {

        readInfo()

        if (equipmentModelComboBox.count > 0) {
            equipmentModelComboBox.currentIndex = 0
        }
        itemIconInfo.setCurrentIconIndex(equipmentModelComboBox.currentIndex)
    }

    Connections {
        target: itemIconSetting
        onSaveItemInfoToJson: {
            readInfo()
            if (equipmentModelComboBox.count > 0)
                equipmentModelComboBox.currentIndex = 0
        }
    }
}
