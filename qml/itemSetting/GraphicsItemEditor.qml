import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import itemIconInfoToJson 1.0
import "../infoSetting"
Item {
    width: 1040
    height: 560
    signal setSize(real size)
    signal setIcon(string icon)
    signal setItemInfo(string type, string value)
    signal setItemsManufacturers(int index, string manufacturers)
    signal setItemsPeriodOfValidity(int index, string periodOfValidity)
    signal setItemsIcon(int index, string iconName)
    signal setItemsDeviceName(int index, string deviceName)
    signal setDeviceInstallTime(int index,string devideInstallTime)

    ListModel {
        id: deviceTypeModel
        ListElement {
            deviceName: qsTr("报警装置")
        }
    }

    ListModel {
        id: deviceTypeTempModel
        ListElement {
            deviceName: qsTr("报警装置")
        }
    }

    SwipeView {
        id: swipView
        currentIndex: 0
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.bottomMargin: 10
        anchors.topMargin: 80


        interactive: false
        Item {

            antialiasing: true

            Grid {
                id: grid
                // anchors.fill: parent
                anchors.bottomMargin: 40
                anchors.topMargin: 60

                columnSpacing: 5
                rowSpacing: 5
                columns: 4
                Text {
                    text: qsTr("设备:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                ComboBox {
                    id: equipmentModelComboBox
                    width: 180
                    height: 40
                    model: deviceTypeModel
                    onCurrentTextChanged: {
                        if (currentIndex >= 0) {
                            itemIconInfo.setCurrentIconIndex(currentIndex)

                            emit: setIcon(itemIconInfo.getIconName(
                                              currentIndex))
                            emit: setItemInfo("equipmentModel", currentText)

                            emit: setItemInfo("manufacturers",
                                              manufacturersValue(currentIndex))
                            emit: setItemInfo("periodOfValidity",
                                              periodOfvalidityValue(
                                                  currentIndex))
                            emit:setItemInfo("deviceInstallTime",getDeviceInstallTime(currentIndex))

                            manufacturersText.text = manufacturersValue(
                                        currentIndex)
                            periodOfValidityText.text = periodOfvalidityValue(
                                        currentIndex)
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
                    width: 180
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
                    width: 180
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
                    width: 180
                    height: 40
                    onTextEdited: {
                        emit: setItemInfo("addrNum", addrNumTextField.text)
                    }
                }

                Text {
                    text: qsTr("网络号:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: networkNumTextField
                    width: 180
                    height: 40
                    onTextEdited: {
                        emit: setItemInfo("networkNum", networkNumTextField.text)
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
                    width: 180
                    height: 40
                    onTextEdited: {
                        emit: setItemInfo("deviceNum", deviceNumTextField.text)
                    }
                }

                Text {
                    text: qsTr("系统:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                ComboBox {
                    id: deviceSysComBox
                    Layout.fillWidth: true
                    width: 180
                    height: 40
                    currentIndex: -1
                    model: ["火灾探测报警系统", "消防联动系统", "自动喷水灭火系统", "水喷雾灭火系统", "消火栓系统", "气体灭火系统", "泡沫灭火系统", "干粉灭火系统", "防烟排烟系统", "防火及卷帘系统", "电梯系统", "消防电话系统", "消防应急广播系统", "消防应急照明和疏散指示系统", "消防电源系统"]
                    onCurrentTextChanged: {
                        emit: setItemInfo("sysOfDevice", currentText)
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
                    width: 180
                    height: 40
                    onTextEdited: {
                        emit: setItemInfo("deviceLocation",
                                          deviceLocationTextField.text)
                    }
                }

                Text {

                    text: qsTr("通道数:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: channelNumTextField
                    Layout.fillWidth: true
                    width: 180
                    height: 40
                    onTextEdited: {
                        emit: setItemInfo("channelNum",
                                          channelNumTextField.text)
                    }
                }

                Text {

                    text: qsTr("模拟量:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                ComboBox {
                    id: analogComboBox
                    Layout.fillWidth: true
                    width: 180
                    height: 40
                    model: [qsTr("无"), qsTr("有")]
                    onCurrentTextChanged: {
                        emit: setItemInfo("analogType", currentText)
                    }
                }

                Text {

                    text: qsTr("有效期:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: periodOfValidityText
                    Layout.fillWidth: true
                    width: 180
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }

                Text {

                    text: qsTr("制造商:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: manufacturersText
                    Layout.fillWidth: true
                    width: 180
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("北京利达华信电子有限公司")
                }

                Text {

                    text: qsTr("操作员:")
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: operatorTextField
                    Layout.fillWidth: true
                    width: 180
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                    onTextEdited: {
                        emit: setItemInfo("operator", operatorTextField.text)
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
                    width: 180
                    height: 40
                    from: 5
                    to: 100
                    stepSize: 1
                    onValueChanged: {
                        emit: setSize(value)
                    }
                }
            }

            Text {
                anchors.top: grid.bottom
                anchors.topMargin: 10
                color: "red"
                font.pointSize: 12
                text: qsTr("注意:当设备为光纤时，源地址对应分机号、通道号对应地址号、分区号对应网络号,\n距离对应位置。")
            }
        }

        ItemIconSetting {
            id: itemIconSetting
            onPeriodValueChanged: {
                itemIconInfo.setCurrentIconIndex(index)
                // emit: setItemInfo("periodOfValidity", periodValue)
                periodOfValidityText.text = periodValue
                emit: setItemsPeriodOfValidity(index, periodValue)
            }
            onManufacturersChanged: {
                itemIconInfo.setCurrentIconIndex(index)
                // emit: setItemInfo("manufacturers", facturers)
                manufacturersText.text = facturers
                emit: setItemsManufacturers(index, facturers)
            }
            onDeviceInstallTimeChanged:
            {
              itemIconInfo.setCurrentIconIndex(index)
              emit: setDeviceInstallTime(index, deviceInstallTime)
            }
            onDeviceNameChanged: {
                var currentDeviceObj = new Object
                currentDeviceObj["deviceName"] = device
                deviceTypeModel.set(index, currentDeviceObj)
                deviceTypeTempModel.clear()
                for (var i = 0; i < deviceTypeModel.count; i++) {
                    deviceTypeTempModel.append(deviceTypeModel.get(i))
                }
                deviceTypeModel.clear()
                for (var j = 0; j < deviceTypeTempModel.count; j++) {
                    deviceTypeModel.append(deviceTypeTempModel.get(j))
                }
                emit: setItemsDeviceName(index, device)

            }

            onIconChanged: {
                emit: setItemsIcon(index, iconPath)
            }
            onDeviceDelete:
            {
                if(deviceTypeModel.count>index)
                {
                    deviceTypeModel.remove(index,1)
                }
            }
        }
    }

    PageIndicator {
        id: indicator
        count: swipView.count
        currentIndex: swipView.currentIndex
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Row {
        anchors.bottom: swipView.top
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.bottomMargin: 10
        anchors.leftMargin: 20
        anchors.left: parent.left
        spacing: 10
        NaviButton {
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

        NaviButton {
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

    function setNetworkNum(networkNum) {
        networkNumTextField.text = networkNum
    }

    function setDeviceNum(deviceNum) {
        deviceNumTextField.text = deviceNum
    }

    function setEquipmentModel(equipmentModel) {
        equipmentModelComboBox.currentIndex = equipmentModelComboBox.find(
                    equipmentModel)
    }

    function setSysOfDevice(sysOfDevice) {
        deviceSysComBox.currentIndex = deviceSysComBox.find(sysOfDevice)
    }

    function setDeviceLocation(deviceLocation) {
        deviceLocationTextField.text = deviceLocation
    }

    function setPeriodOfValidity(period) {
        periodOfValidityText.text = period
    }

    function setManufacturers(manufacturers) {
        manufacturersText.text = manufacturers
    }

    function setOperator(deviceOperator) {
        operatorTextField.text = deviceOperator
    }

    function setItemSize(size) {
        sizeSpinBox.value = size
    }

    function setChannelNum(channelNum) {
        channelNumTextField.text = channelNum
    }

    function setAnalogType(analogType) {
        analogComboBox.currentIndex = analogComboBox.find(analogType)
    }
    function clearItemInfo() {
        extNumTextField.clear()
        loopNumTextField.clear()
        addrNumTextField.clear()
        deviceNumTextField.clear()
        deviceSysComBox.currentIndex = -1
        deviceLocationTextField.clear()
        operatorTextField.clear()
    }

    function currentIconIndex() {
        itemIconInfo.setCurrentIconIndex(equipmentModelComboBox.currentIndex)
        return equipmentModelComboBox.currentIndex
    }

    function manufacturersValue(pos)
    {
     return itemIconInfo.getValue(String("%1").arg(pos),"manufacturers")
    }

    function periodOfvalidityValue(pos)
    {
        return  itemIconInfo.getValue(String("%1").arg(pos),"periodOfvalidity")
    }
    function getDeviceInstallTime(pos)
    {
       return  itemIconInfo.getValue(String("%1").arg(pos),"deviceInstallTime")
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
        if (itemIconInfoStr.length <= 4)
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
        manufacturersText.text = manufacturersValue(
                    equipmentModelComboBox.currentIndex)
        periodOfValidityText.text = periodOfvalidityValue(
                    equipmentModelComboBox.currentIndex)
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
