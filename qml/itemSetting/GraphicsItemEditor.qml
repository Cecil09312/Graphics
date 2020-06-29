import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import itemIconInfoToJson 1.0
import "../infoSetting"
import controller 1.0

Item {
    width: 1060
    height: 560
    signal setSize(real size)
    signal setIcon(string icon)
    signal setItemInfo(string type, string value)
    signal setItemsManufacturers(int index, string manufacturers)
    signal setItemsPeriodOfValidity(int index, string periodOfValidity)
    signal setItemsIcon(int index, string iconName)
    signal setItemsDeviceName(int index, string deviceName)
    signal setDeviceInstallTime(int index,string devideInstallTime)
    signal startBatch()
    signal changeInfoFromFloor()
    signal importExcelFile(string filePath)
    signal setExcelFileAvailable(bool isAvailable)
    signal selectDevice(bool isSelected)
    signal selectSys(bool isSelected)
    signal selectChannel(bool isSelected)
    signal selectAnalog(bool isSelected)
    signal selectSize(bool isSelected)


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

            Row
            {
                spacing: 10
                Column
                {
                    spacing: 10

                    Grid {
                        id: grid
                        // anchors.fill: parent
                        anchors.bottomMargin: 40
                        anchors.topMargin: 60

                        columnSpacing: 5
                        rowSpacing: 5
                        columns: 4

                        Text {
                            id:extNumTxt
                            text: qsTr("分机号:")
                            height: 40
                            verticalAlignment: Text.AlignVCenter
                        }
                        TextField {
                            id: extNumTextField
                            validator: IntValidator {bottom: 0; top: 255;}
                            width: 160
                            height: 40
                            selectByMouse: true
                            selectionColor: "blue"
                            selectedTextColor: "white"
                            onTextEdited: {
                                emit: setItemInfo("extNum", extNumTextField.text.replace(/\s+/g,""))
                            }
                        }

                        Text {
                            id:loopNumTxt
                            text: qsTr("回路号:")
                            height: 40
                            verticalAlignment: Text.AlignVCenter
                        }
                        TextField {
                            id: loopNumTextField
                            validator: IntValidator {bottom: 0; top: 255;}
                            width: 160
                            height: 40
                            selectByMouse: true
                            selectionColor: "blue"
                            selectedTextColor: "white"
                            onTextEdited: {
                                emit: setItemInfo("loopNum", loopNumTextField.text.replace(/\s+/g,""))
                            }
                        }

                        Text {
                            id:addrNumTxt
                            text: qsTr("地址号:")
                            height: 40
                            verticalAlignment: Text.AlignVCenter
                        }
                        TextField {
                            id: addrNumTextField
                            validator: IntValidator {bottom: 0; top: 255;}
                            width: 160
                            height: 40
                            selectByMouse: true
                            selectionColor: "blue"
                            selectedTextColor: "white"
                            onTextEdited: {
                                emit: setItemInfo("addrNum", addrNumTextField.text.replace(/\s+/g,""))
                            }
                        }

                        Text {
                            id:networkNumTxt
                            text: qsTr("网络号:")
                            height: 40
                            verticalAlignment: Text.AlignVCenter
                        }
                        TextField {
                            id: networkNumTextField
                            validator: IntValidator {bottom: 0; top: 255;}
                            width: 160
                            height: 40
                            text: "0"
                            selectByMouse: true
                            selectionColor: "blue"
                            selectedTextColor: "white"
                            onTextEdited: {
                                emit: setItemInfo("networkNum", networkNumTextField.text.replace(/\s+/g,""))
                            }
                        }

                        Text {
                            id:powerAddrTxt
                            text: qsTr("电源地址:")
                            height: 40
                            verticalAlignment: Text.AlignVCenter
                        }
                        TextField {
                            id: powerAddrTextField
                            validator: IntValidator {bottom: 0; top: 255;}
                            width: 160
                            height: 40
                            text: "0"
                            selectByMouse: true
                            selectionColor: "blue"
                            selectedTextColor: "white"

                            onTextEdited: {
                                emit: setItemInfo("powerAddr", powerAddrTextField.text.replace(/\s+/g,""))
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
                            width: 160
                            height: 40
                            selectByMouse: true
                            selectionColor: "blue"
                            selectedTextColor: "white"
                            onTextEdited: {
                                emit: setItemInfo("deviceNum", deviceNumTextField.text)
                            }
                        }



                        Text {

                            id:locationTxt
                            text: qsTr("位置:")
                            height: 40
                            verticalAlignment: Text.AlignVCenter
                        }
                        TextField {
                            id: deviceLocationTextField
                            Layout.fillWidth: true
                            width: 160
                            height: 40
                            selectByMouse: true
                            selectionColor: "blue"
                            selectedTextColor: "white"
                            onTextEdited: {
                                emit: setItemInfo("deviceLocation",
                                                  deviceLocationTextField.text)
                            }
                        }


                        Text {

                            id:operatorTxt
                            text: qsTr("操作员:")
                            height: 40
                            verticalAlignment: Text.AlignVCenter
                        }
                        TextField {
                            id: operatorTextField
                            Layout.fillWidth: true
                            width: 160
                            height: 40
                            selectByMouse: true
                            selectionColor: "blue"
                            selectedTextColor: "white"
                            verticalAlignment: Text.AlignVCenter
                            onTextEdited: {
                                emit: setItemInfo("operator", operatorTextField.text)
                            }
                        }

                        Text {
                            id:deviceTxt
                            text: qsTr("设备:")
                            height: 40
                            verticalAlignment: Text.AlignVCenter
                        }
                        ComboBox {
                            id: equipmentModelComboBox
                            width: 160
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

                                    //                                    manufacturersText.text = manufacturersValue(
                                    //                                                currentIndex)
                                    periodOfValidityText.text = periodOfvalidityValue(
                                                currentIndex)
                                }
                            }
                        }
                        Text {
                            id:sysTxt
                            text: qsTr("系统:")
                            height: 40
                            verticalAlignment: Text.AlignVCenter
                        }
                        ComboBox {
                            id: deviceSysComBox
                            Layout.fillWidth: true
                            width: 160
                            height: 40

                            currentIndex: -1
                            model: [qsTr("电气火灾监控系统"),qsTr("火灾探测报警系统"), qsTr("消防联动系统"), qsTr("自动喷水灭火系统"), qsTr("水喷雾灭火系统"), qsTr("消火栓系统"), qsTr("气体灭火系统"), qsTr("泡沫灭火系统"), qsTr("干粉灭火系统"), qsTr("防烟排烟系统"), qsTr("防火及卷帘系统"), qsTr("电梯系统"), qsTr("消防电话系统"), qsTr("消防应急广播系统"), qsTr("消防应急照明和疏散指示系统"), qsTr("消防电源系统")]
                            onCurrentTextChanged: {
                                emit: setItemInfo("sysOfDevice", currentText)
                            }
                        }


                        Text {
                            id:channelNumTxt
                            text: qsTr("通道数:")
                            height: 40
                            verticalAlignment: Text.AlignVCenter
                        }
                        TextField {
                            id: channelNumTextField
                            validator: IntValidator {bottom: 0; top: 1000;}
                            Layout.fillWidth: true
                            width: 160
                            height: 40
                            selectByMouse: true
                            selectionColor: "blue"
                            selectedTextColor: "white"
                            onTextEdited: {
                                emit: setItemInfo("channelNum",
                                                  channelNumTextField.text.replace(/\s+/g,""))
                            }
                        }

                        Text {

                            id:analogTxt
                            text: qsTr("模拟量:")
                            height: 40
                            verticalAlignment: Text.AlignVCenter
                        }
                        ComboBox {
                            id: analogComboBox
                            Layout.fillWidth: true
                            width: 160
                            height: 40
                            model: [qsTr("无"), qsTr("有")]
                            onCurrentTextChanged: {
                                emit: setItemInfo("analogType", currentText)
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
                            width: 160
                            height: 40
                            from: 10
                            to: 80
                            stepSize: 1
                            value: 30
                            onValueChanged: {
                                emit: setSize(value)
                            }
                        }



                        Text {

                            id:periodOfValidityTxt
                            text: qsTr("有效期:")
                            height: 40
                            verticalAlignment: Text.AlignVCenter
                        }
                        Text {
                            id: periodOfValidityText
                            Layout.fillWidth: true
                            width: 160
                            height: 40
                            verticalAlignment: Text.AlignVCenter
                        }

                        //                        Text {

                        //                            text: qsTr("制造商:")
                        //                            height: 40
                        //                            verticalAlignment: Text.AlignVCenter
                        //                        }
                        //                        Text {
                        //                            id: manufacturersText
                        //                            Layout.fillWidth: true
                        //                            width: 180
                        //                            height: 40
                        //                            verticalAlignment: Text.AlignVCenter
                        //                            text: qsTr("北京利达华信电子有限公司")
                        //                        }
                    }

                    GroupBox
                    {
                        id:importExcelGroupBox
                       // visible: Controller.sysOnLinux()
                        title: qsTr("导入excel文件")
                        Column
                        {
                            spacing: 5
                            anchors.fill: parent
                            Row
                            {
                                id:excelRow
                                spacing: 5
                                CheckBox
                                {
                                    id:excelSelectCheckBox
                                    text: qsTr("使用excel数据")
                                    onCheckedChanged:
                                    {
                                        if(checked)
                                        {
                                            excelTextField.enabled = true
                                            selectExcelBtn.enabled = true

                                        }
                                        else
                                        {
                                            excelTextField.enabled = false
                                            selectExcelBtn.enabled = false

                                        }
                                        emit:setExcelFileAvailable(checked)

                                    }
                                }
                                TextField
                                {
                                    id:excelTextField
                                    readOnly: true
                                    enabled: false
                                }

                                NaviButton
                                {
                                    id:selectExcelBtn
                                    text:qsTr("选择文件")
                                    onClicked:
                                    {
                                        excelImportStateTxt.text=""
                                        chooseFileDialog.open()
                                    }


                                }
                            }

                            Text {
                                id: excelImportStateTxt
                                width: 300
                                horizontalAlignment: Text.AlignHCenter
                                font.pointSize:12
                                color: "green"

                            }


                        }


                    }



                }

                Column
                {
                    spacing: 10

                    GroupBox
                    {
                        id:groupBox
                        title: qsTr("批量设置")
                        Column
                        {
                            anchors.fill: parent
                            spacing: 5
                            Grid
                            {

                                spacing: 5
                                columns: 4
                                Text {
                                    id:minNetworkNumTxt
                                    text: qsTr("最小网络号:")
                                    height: 40
                                    font.pointSize: 10
                                    verticalAlignment: Text.AlignVCenter
                                }
                                TextField
                                {
                                    id:minNetworkTextField
                                    validator: IntValidator {bottom: 0; top: 255;}
                                    width: 80
                                    selectByMouse: true
                                    selectionColor: "blue"
                                    selectedTextColor: "white"

                                }

                                Text {
                                    id:maxNetworkNumTxt
                                    text: qsTr("最大网络号:")
                                    font.pointSize: 10
                                    height: 40
                                    verticalAlignment: Text.AlignVCenter
                                }
                                TextField
                                {
                                    id:maxNetworkTextField
                                    validator: IntValidator {bottom: 0; top: 255;}
                                    width: 80
                                    selectByMouse: true
                                    selectionColor: "blue"
                                    selectedTextColor: "white"

                                }


                                Text {

                                    id:minExtNumTxt
                                    text: qsTr("最小分机号:")
                                    font.pointSize: 10
                                    height: 40
                                    verticalAlignment: Text.AlignVCenter
                                }
                                TextField
                                {
                                    id:minExtNumTextField
                                    validator: IntValidator {bottom: 0; top: 255;}
                                    width: 80
                                    selectByMouse: true
                                    selectionColor: "blue"
                                    selectedTextColor: "white"

                                }

                                Text {
                                    id:maxExtNumTxt
                                    text: qsTr("最大分机号:")
                                    font.pointSize: 10
                                    height: 40
                                    verticalAlignment: Text.AlignVCenter
                                }
                                TextField
                                {
                                    id:maxExtNumTextField
                                    validator: IntValidator {bottom: 0; top: 255;}
                                    width: 80
                                    selectByMouse: true
                                    selectionColor: "blue"
                                    selectedTextColor: "white"

                                }



                                Text {

                                    id:minLoopNumTxt
                                    text: qsTr("最小回路号:")
                                    font.pointSize: 10
                                    height: 40
                                    verticalAlignment: Text.AlignVCenter
                                }
                                TextField
                                {
                                    id:minLoopNumTextField
                                    validator: IntValidator {bottom: 0; top: 255;}
                                    width: 80
                                    selectByMouse: true
                                    selectionColor: "blue"
                                    selectedTextColor: "white"

                                }

                                Text {

                                    id:maxLoopNumTxt
                                    text: qsTr("最大回路号:")
                                    font.pointSize: 10
                                    height: 40
                                    verticalAlignment: Text.AlignVCenter
                                }
                                TextField
                                {
                                    id:maxLoopNumTextField
                                    validator: IntValidator {bottom: 0; top: 255;}
                                    width: 80
                                    selectByMouse: true
                                    selectionColor: "blue"
                                    selectedTextColor: "white"

                                }

                                Text {

                                    id:minAddrNumTxt
                                    text: qsTr("最小地址号:")
                                    font.pointSize: 10
                                    height: 40
                                    verticalAlignment: Text.AlignVCenter
                                }
                                TextField
                                {
                                    id:minAddrNumTextField
                                    validator: IntValidator {bottom: 0; top: 255;}
                                    width: 80
                                    selectByMouse: true
                                    selectionColor: "blue"
                                    selectedTextColor: "white"

                                }

                                Text {

                                    id:maxAddrNumTxt
                                    text: qsTr("最大地址号:")
                                    font.pointSize: 10
                                    height: 40
                                    verticalAlignment: Text.AlignVCenter
                                }
                                TextField
                                {
                                    id:maxAddrNumTextField
                                    validator: IntValidator {bottom: 0; top: 255;}
                                    width: 80
                                    selectByMouse: true
                                    selectionColor: "blue"
                                    selectedTextColor: "white"

                                }



                            }

                            Row
                            {
                                spacing: 5
                                CheckBox
                                {
                                    id:deviceCheckBox
                                    text: qsTr("设备")

                                    onCheckStateChanged:
                                    {
                                        emit:selectDevice(checked)
                                      //console.log(checked)
                                    }
                                }

                                CheckBox
                                {
                                    id:sysCheckBox
                                   text: qsTr("系统")
                                   onCheckStateChanged:
                                   {
                                       emit:selectSys(checked)
                                   }
                                }

                                CheckBox
                                {
                                    id:channelCheckBox
                                    text:qsTr("通道数")
                                    onCheckStateChanged:
                                    {
                                       anglogCheckBox.checked = checked
                                       emit:selectChannel(checked)
                                    }
                                }

                                CheckBox
                                {
                                    id:anglogCheckBox
                                    text:qsTr("模拟量")
                                    onCheckStateChanged:
                                    {
                                       emit:selectAnalog(checked)
                                    }
                                }

                                CheckBox
                                {
                                    id:sizeCheckBox
                                    text:qsTr("大小")
                                    onCheckStateChanged:
                                    {
                                       emit:selectSize(checked)
                                    }
                                }

                            }

                            Row
                            {
                                spacing: 5
                                NaviButton
                                {
                                    id:batchAmendBtn
                                    text: qsTr("批量修改")
                                    onClicked: startBatch()

                                }
                                NaviButton
                                {
                                    id:asFloorAmendBtn
                                    text: qsTr("按当前楼层修改")
                                    font.pointSize: 11
                                    onClicked: changeInfoFromFloor()

                                }
                            }
                        }
                    }

                }

            }
        }

        ItemIconSetting {
            id: itemIconSetting
            onPeriodValueChanged: {
                itemIconInfo.setCurrentIconIndex(index)
                periodOfValidityText.text = periodValue
                emit: setItemsPeriodOfValidity(index, periodValue)
            }
            onManufacturersChanged: {
                itemIconInfo.setCurrentIconIndex(index)
                //manufacturersText.text = facturers
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
            onClearIcons:
            {
                deviceTypeModel.clear()
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


    FileDialog
    {
        id: chooseFileDialog
        //folder: filePath
        //folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        nameFilters: "Excel Files(*.xlsx)"
        flags: Qt.WindowStaysOnTopHint|Qt.MSWindowsFixedSizeDialogHint|Qt.WindowCloseButtonHint
        onAccepted: {
            excelTextField.text = file.toString()
            emit:importExcelFile(excelTextField.text)
        }

    }

    function curIconName()
    {
        return itemIconInfo.getIconName(equipmentModelComboBox.currentIndex)
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

    function setPowerAddr(addr)
    {
        powerAddrTextField.text = addr
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

    //    function setManufacturers(manufacturers) {
    //        manufacturersText.text = manufacturers
    //    }

    function setOperator(deviceOperator) {
        operatorTextField.text = deviceOperator
    }

    function setItemSize(size) {
        sizeSpinBox.value = size
    }

    function setChannelNum(channelNum) {
        channelNumTextField.text = channelNum
    }


    function getLoopNum()
    {
        return loopNumTextField.text
    }

    function getAddrNum()
    {
        return addrNumTextField.text
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

    function getChannelNum()
    {

        return channelNumTextField.text
    }
    function getAnalogType()
    {
        return analogComboBox.currentText

    }
    function getDeviceInstallTime(pos)
    {
        return  itemIconInfo.getValue(String("%1").arg(pos),"deviceInstallTime")
    }

    function getEquipmentModel()
    {
        return equipmentModelComboBox.currentText

    }
    function getSysName()
    {
        return deviceSysComBox.currentText
    }
    function getOperator()
    {
        return operatorTextField.text

    }
    function getIconSize()
    {
        return  sizeSpinBox.value
    }

    function getMinNetworkNum()
    {
        return minNetworkTextField.text
    }

    function getMaxNetworkNum()
    {
        return maxNetworkTextField.text
    }

    function getMinExtNum()
    {
        return minExtNumTextField.text
    }

    function getMaxExtNum()
    {
        return maxExtNumTextField.text
    }

    function getMinLoopNum()
    {
        return minLoopNumTextField.text
    }

    function getMaxLoopNum()
    {
        return maxLoopNumTextField.text
    }

    function getMinAddrNum()
    {
        return minAddrNumTextField.text
    }

    function getMaxAddrNum()
    {
        return maxAddrNumTextField.text
    }

    function setImportExcelState(isOk)
    {
        if(isOk)
        {
            excelImportStateTxt.text = qsTr("导入成功");
            excelImportStateTxt.color ="green"

        }
        else
        {
            excelImportStateTxt.text = qsTr("导入失败");
            excelImportStateTxt.color ="red"

        }


    }
    function canImportExcel()
    {
        return excelSelectCheckBox.checked
    }

    function readInfo() {

        var size = itemIconInfo.sizeOfHash()
        if (size === 0) {
            deviceTypeModel.clear()
            itemIconInfo.setCurrentIconIndex(-1)
            return
        }
        if (size > 0)
        {
            var modelCount = deviceTypeModel.count
            if (modelCount <= size) {
                for (var j = 0; j < modelCount; j++) {
                    var currentObj = new Object
                    var currentIndex = String("%1").arg(j)
                    var currentDeviceObj = new Object

                    currentDeviceObj["deviceName"] = itemIconInfo.getValue(currentIndex,"deviceName")
                    deviceTypeModel.set(j, currentDeviceObj)
                    itemIconInfo.setIconIndexHash(j, itemIconInfo.getValue(currentIndex,"imagePath"))
                }

                for (var i = modelCount; i < size; i++) {
                    var index = String("%1").arg(i)
                    var obj = new Object

                    var deviceObj = new Object
                    deviceObj["deviceName"] = itemIconInfo.getValue(index,"deviceName")
                    deviceTypeModel.append(deviceObj)
                    var imagePathStr = new String
                    imagePathStr = itemIconInfo.getValue(index,"imagePath")
                    itemIconInfo.setIconIndexHash(i, imagePathStr)

                }
            }else {

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
        //        manufacturersText.text = manufacturersValue(
        //                    equipmentModelComboBox.currentIndex)
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

    function initIconSetting()
    {
      itemIconSetting.initIconSetting()
    }

    function retranslate()
    {
        extNumTxt.text = qsTr("分机号:")
        loopNumTxt.text = qsTr("回路号:")
        addrNumTxt.text = qsTr("地址号:")
        networkNumTxt.text = qsTr("网络号:")
        powerAddrTxt.text = qsTr("电源地址:")
        deviceNumTxt.text =qsTr("设备编码:")
        locationTxt.text =qsTr("位置:")
        operatorTxt.text = qsTr("操作员:")
        deviceTxt.text = qsTr("设备:")
        sysTxt.text = qsTr("系统:")
        deviceSysComBox.model =[qsTr("电气火灾监控系统"),qsTr("火灾探测报警系统"), qsTr("消防联动系统"), qsTr("自动喷水灭火系统"), qsTr("水喷雾灭火系统"), qsTr("消火栓系统"), qsTr("气体灭火系统"), qsTr("泡沫灭火系统"), qsTr("干粉灭火系统"), qsTr("防烟排烟系统"), qsTr("防火及卷帘系统"), qsTr("电梯系统"), qsTr("消防电话系统"), qsTr("消防应急广播系统"), qsTr("消防应急照明和疏散指示系统"), qsTr("消防电源系统")]
        channelNumTxt.text = qsTr("通道数:")
        analogTxt.text = qsTr("模拟量:")
        analogComboBox.model =[qsTr("无"), qsTr("有")]
        sizeLabel.text = qsTr("大小:")
        periodOfValidityTxt.text = qsTr("有效期:")
        groupBox.title = qsTr("批量设置")
        minNetworkNumTxt.text = qsTr("最小网络号:")
        maxNetworkNumTxt.text = qsTr("最大网络号:")
        minExtNumTxt.text = qsTr("最小分机号:")
        maxExtNumTxt.text = qsTr("最大分机号:")
        minLoopNumTxt.text = qsTr("最小回路号:")
        maxLoopNumTxt.text = qsTr("最大回路号:")
        minAddrNumTxt.text = qsTr("最小地址号:")
        maxAddrNumTxt.text = qsTr("最大地址号:")
        batchAmendBtn.text = qsTr("批量修改")
        asFloorAmendBtn.text = qsTr("按当前楼层修改")
        importExcelGroupBox.title = qsTr("导入excel文件")
        excelSelectCheckBox.text=qsTr("使用excel数据")
        selectExcelBtn.text = qsTr("选择文件")
        iconSettingBtn.text = qsTr("图标信息设置")
        legendBtn.text = qsTr("图例")
        itemIconSetting.retranslate()
        deviceCheckBox.text = qsTr("设备")
        sizeCheckBox.text = qsTr("大小")
        anglogCheckBox.text = qsTr("模拟量")
        channelCheckBox.text = qsTr("通道数")
        sysCheckBox.text = qsTr("系统")

    }
}


