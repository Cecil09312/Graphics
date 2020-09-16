import QtQuick 2.9
import QtQuick.Controls 2.2
import Qt.labs.platform 1.0
import itemIconInfoToJson 1.0
import controller 1.0
import "../infoSetting"
//import Qt.labs.calendar 1.0
import QtQuick.Controls 1.4 as Controls1_4
import "qrc:/jsFile/JsDateTime.js" as JsDateTime
Rectangle {
    width: 1020
    height: 480

    signal saveItemInfoToJson
    signal periodValueChanged(int index, string periodValue)
    signal manufacturersChanged(int index, string facturers)
    signal deviceInstallTimeChanged(int index, string deviceInstallTime)
    signal iconChanged(int index, string iconPath)
    signal deviceNameChanged(int index, string device)
    signal deviceDelete(int index)
    signal clearIcons()
    property url oneFilePath: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
    property url someFilePath: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)

    ListModel {
        id: listModel
        ListElement {
            deviceName: qsTr("报警装置")
            imagePath: "qrc:/images/alarmDevice.svg"
            manufacturers: qsTr("北京利达华信电子有限公司")
            deviceInstallTime:""
            periodOfvalidity:""
            selectIconBtnName:qsTr("选择图标")
        }
    }

    Component {
        id: listDelegate
        Item {
            id: delegateItem
            width: listView.width
            height: 35
            clip: true
            Row {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                spacing: 8
                Image {
                    id: image
                    source: imagePath
                    width: 30
                    height: 30
                    sourceSize.width: 30
                    sourceSize.height: 30
                }

                TextField {
                    id: deviceNameTextFild
                    width: 160
                    height: 30
                    text: deviceName
                    selectByMouse: true
                    selectionColor: "blue"
                    selectedTextColor: "white"

                    onTextChanged: {
                        deviceName = deviceNameTextFild.text

                    }
                    onEditingFinished:
                    {
                        emit: deviceNameChanged(index, deviceName)
                        // saveInfo()

                    }

                }

                TextField {
                    id: manufacturersTextField
                    width: 160
                    height: 30
                    text: manufacturers
                    selectByMouse: true
                    selectionColor: "blue"
                    selectedTextColor: "white"

                    onTextChanged: {
                        manufacturers = manufacturersTextField.text


                    }
                    onEditingFinished:
                    {
                        emit: manufacturersChanged(index, manufacturers)
                        // saveInfo()
                    }
                }


                Row {
                    spacing: 2

                    Controls1_4.SpinBox
                    {
                        id:deviceInstallYearSpinBox
                        height: 30
                        width: 60
                        minimumValue: 1990
                        maximumValue: 2200
                        value:parseInt(JsDateTime.getDataFromStr(deviceInstallTime,"yyyy/MM/dd",0))
                        onValueChanged:
                        {
                            var curYear = deviceInstallYearSpinBox.value
                            var curMonth = deviceInstallMonthSpinBox.value

                            var maxDate=JsDateTime.getMaxDate(curYear,curMonth)
                            deviceInstallDateSpinBox.maximumValue=maxDate
                            if(deviceInstallDateSpinBox.value>maxDate)
                            {
                                deviceInstallDateSpinBox.value = maxDate
                            }

                        }


                        onEditingFinished:
                        {
                            var deviceInstallDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3").arg(deviceInstallYearSpinBox.value).arg(deviceInstallMonthSpinBox.value).arg(deviceInstallDateSpinBox.value),"yyyy/M/d");
                            var deviceInstallDate = new Date(deviceInstallDateValue)
                            var deviceInstallDateStr=Qt.formatDateTime(deviceInstallDate,"yyyy/MM/dd");
                            deviceInstallTime = deviceInstallDateStr
                            emit: deviceInstallTimeChanged(index, deviceInstallTime)
                        }
                        Component.onCompleted:
                        {

                            var curYear = deviceInstallYearSpinBox.value
                            var curMonth = deviceInstallMonthSpinBox.value

                            var maxDate=JsDateTime.getMaxDate(curYear,curMonth)
                            deviceInstallDateSpinBox.maximumValue=maxDate
                            if(deviceInstallDateSpinBox.value>maxDate)
                            {
                                deviceInstallDateSpinBox.value = maxDate
                            }


                        }
                    }

                    Controls1_4.SpinBox
                    {
                        id:deviceInstallMonthSpinBox
                        height: 30
                        width: 50
                        minimumValue: 1
                        maximumValue: 12
                        value:parseInt(JsDateTime.getDataFromStr(deviceInstallTime,"yyyy/MM/dd",1))
                        onValueChanged:
                        {
                            var curYear = deviceInstallYearSpinBox.value
                            var curMonth = deviceInstallMonthSpinBox.value

                            var maxDate=JsDateTime.getMaxDate(curYear,curMonth)
                            deviceInstallDateSpinBox.maximumValue=maxDate
                            if(deviceInstallDateSpinBox.value>maxDate)
                            {
                                deviceInstallDateSpinBox.value = maxDate
                            }

                        }
                        onEditingFinished:
                        {


                            var deviceInstallDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3").arg(deviceInstallYearSpinBox.value).arg(deviceInstallMonthSpinBox.value).arg(deviceInstallDateSpinBox.value),"yyyy/M/d");
                            var deviceInstallDate = new Date(deviceInstallDateValue)
                            var deviceInstallDateStr=Qt.formatDateTime(deviceInstallDate,"yyyy/MM/dd");
                            deviceInstallTime = deviceInstallDateStr
                            emit: deviceInstallTimeChanged(index, deviceInstallTime)
                        }

                        Component.onCompleted:
                        {

                            var curYear = deviceInstallYearSpinBox.value
                            var curMonth = deviceInstallMonthSpinBox.value

                            var maxDate=JsDateTime.getMaxDate(curYear,curMonth)
                            deviceInstallDateSpinBox.maximumValue=maxDate
                            if(deviceInstallDateSpinBox.value>maxDate)
                            {
                                deviceInstallDateSpinBox.value = maxDate
                            }

                        }
                    }

                    Controls1_4.SpinBox
                    {
                        id:deviceInstallDateSpinBox
                        height: 30
                        width: 50
                        minimumValue: 1
                        maximumValue: 31
                        value:parseInt(JsDateTime.getDataFromStr(deviceInstallTime,"yyyy/MM/dd",2))
                        onEditingFinished:
                        {
                            var deviceInstallDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3").arg(deviceInstallYearSpinBox.value).arg(deviceInstallMonthSpinBox.value).arg(deviceInstallDateSpinBox.value),"yyyy/M/d");
                            var deviceInstallDate = new Date(deviceInstallDateValue)
                            var deviceInstallDateStr=Qt.formatDateTime(deviceInstallDate,"yyyy/MM/dd");
                            deviceInstallTime = deviceInstallDateStr
                            emit: deviceInstallTimeChanged(index, deviceInstallTime)
                        }
                    }
                }

                Row {
                    spacing: 2

                    Controls1_4.SpinBox
                    {
                        id:periodYearSpinBox
                        height: 30
                        width: 60
                        minimumValue: 1990
                        maximumValue: 2200
                        value: parseInt(JsDateTime.getDataFromStr(periodOfvalidity,"yyyy/MM/dd",0))
                        onValueChanged:
                        {
                            var curYear = periodYearSpinBox.value
                            var curMonth = periodMonthSpinBox.value

                            var maxDate=JsDateTime.getMaxDate(curYear,curMonth)
                            periodDateSpinBox.maximumValue=maxDate
                            if(periodDateSpinBox.value>maxDate)
                            {
                                periodDateSpinBox.value = maxDate
                            }

                        }
                        onEditingFinished:
                        {
                            //setPeriodTime()

                            var periodDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3").arg(periodYearSpinBox.value).arg(periodMonthSpinBox.value).arg(periodDateSpinBox.value),"yyyy/M/d");
                            var periodDate = new Date(periodDateValue)
                            var periodDateStr=Qt.formatDateTime(periodDate,"yyyy/MM/dd");

                            periodOfvalidity = periodDateStr
                            emit: periodValueChanged(index, periodOfvalidity)
                        }

                        Component.onCompleted:
                        {
                            var curYear = periodYearSpinBox.value
                            var curMonth = periodMonthSpinBox.value

                            var maxDate=JsDateTime.getMaxDate(curYear,curMonth)
                            periodDateSpinBox.maximumValue=maxDate
                            if(periodDateSpinBox.value>maxDate)
                            {
                                periodDateSpinBox.value = maxDate
                            }
                        }
                    }

                    Controls1_4.SpinBox
                    {
                        id:periodMonthSpinBox
                        height: 30
                        width: 50
                        minimumValue: 1
                        maximumValue: 12
                        value: parseInt(JsDateTime.getDataFromStr(periodOfvalidity,"yyyy/MM/dd",1))
                        onValueChanged:
                        {
                            var curYear = periodYearSpinBox.value
                            var curMonth = periodMonthSpinBox.value

                            var maxDate=JsDateTime.getMaxDate(curYear,curMonth)
                            periodDateSpinBox.maximumValue=maxDate
                            if(periodDateSpinBox.value>maxDate)
                            {
                                periodDateSpinBox.value = maxDate
                            }
                        }
                        onEditingFinished:
                        {
                            //setPeriodTime()

                            var periodDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3").arg(periodYearSpinBox.value).arg(periodMonthSpinBox.value).arg(periodDateSpinBox.value),"yyyy/M/d");
                            var periodDate = new Date(periodDateValue)
                            var periodDateStr=Qt.formatDateTime(periodDate,"yyyy/MM/dd");

                            periodOfvalidity = periodDateStr
                            emit: periodValueChanged(index, periodOfvalidity)
                        }
                        Component.onCompleted:
                        {
                            var curYear = periodYearSpinBox.value
                            var curMonth = periodMonthSpinBox.value

                            var maxDate=JsDateTime.getMaxDate(curYear,curMonth)
                            periodDateSpinBox.maximumValue=maxDate
                            if(periodDateSpinBox.value>maxDate)
                            {
                                periodDateSpinBox.value = maxDate
                            }
                        }
                    }

                    Controls1_4.SpinBox
                    {
                        id:periodDateSpinBox
                        height: 30
                        width: 50
                        minimumValue: 1
                        maximumValue: 31
                        value: parseInt(JsDateTime.getDataFromStr(periodOfvalidity,"yyyy/MM/dd",2))
                        onEditingFinished:
                        {
                            var periodDateValue= Date.fromLocaleString(Qt.locale(),String("%1/%2/%3").arg(periodYearSpinBox.value).arg(periodMonthSpinBox.value).arg(periodDateSpinBox.value),"yyyy/M/d");
                            var periodDate = new Date(periodDateValue)
                            var periodDateStr=Qt.formatDateTime(periodDate,"yyyy/MM/dd");

                            periodOfvalidity = periodDateStr
                            emit: periodValueChanged(index, periodOfvalidity)
                        }
                    }
                }

                TextField {
                    id: imagePathTextField
                    width: 160
                    height: 30
                    text: imagePath
                    selectByMouse: true
                    selectionColor: "blue"
                    selectedTextColor: "white"

                    onTextChanged: {
                        imagePath = imagePathTextField.text
                    }
                }

                NaviButton {
                    id: imageSettingBtn
                    width: 100
                    height: 30
                    text:selectIconBtnName
                    font.pointSize: 10
                    onClicked: {
                        fileDialog.open()
                    }


                }

                Image {
                    id: deleteImage
                    width: 30
                    height: 30
                    source: "qrc:/images/delete.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var currentItemIndex=index
                            listModel.remove(currentItemIndex)
                            itemIconInfo.removeIconInfo(String("%1").arg(currentItemIndex))

                            emit:deviceDelete(currentItemIndex)
                            saveInfo()

                        }
                    }
                }
            }

            FileDialog {
                id: fileDialog
                title: "Please choose a file"
                folder: oneFilePath
                flags: Qt.WindowStaysOnTopHint|Qt.MSWindowsFixedSizeDialogHint|Qt.WindowCloseButtonHint

                onAccepted: {
                    oneFilePath= file
                    imagePath = Qt.resolvedUrl(decodeURI(
                                                   currentFile.toString()))
                    deviceName = Controller.getFileNameFromUrl(currentFile.toString(),false)

                    emit: iconChanged(index, imagePath)
                    saveInfo()
                }
            }
        }


    }




    FileDialog {
        id: selectFilesDialog
        title: "Please choose some files"
        fileMode: FileDialog.OpenFiles
        folder: someFilePath
        flags:Qt.WindowStaysOnTopHint|Qt.WindowMaximizeButtonHint|Qt.MSWindowsFixedSizeDialogHint|Qt.WindowCloseButtonHint

        onAccepted: {
            someFilePath=file
            for(var i=0;i<currentFiles.length;i++)
            {
                var obj = new Object
                var curDate = new Date
                var periodOfvalidity= new Date
                periodOfvalidity.setFullYear(curDate.getFullYear()+13)
                obj["imagePath"] = Qt.resolvedUrl(
                            decodeURI(currentFiles[i]))
                obj["deviceName"] = Controller.getFileNameFromUrl(currentFiles[i].toString(),false)
                obj["manufacturers"] = qsTr("北京利达华信电子有限公司")
                obj["deviceInstallTime"] = Qt.formatDate(curDate,"yyyy/MM/dd")
                obj["periodOfvalidity"] = Qt.formatDate(periodOfvalidity,"yyyy/MM/dd")
                obj["selectIconBtnName"] = qsTr("选择图标")
                listModel.append(obj)
            }
            saveInfo()
        }
    }

    Row {
        id: titleRow
        anchors.top: parent.top
        anchors.bottomMargin: 20
        anchors.margins: 10
        spacing: 5
        Text {
            id: iconTxt
            width: 40
            // height: 40
            text: qsTr("图标")
            //font.family: "Times New Roman"
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        Text {
            id: deviceNameTxt
            //font.family: "Times New Roman"
            font.pixelSize: 14
            // height: 40
            width: 150
            text: qsTr("设备")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        Text {
            id: manufacturersTxt
            width: 160
            // height: 40
            text: qsTr("制造商")
            //font.family: "Times New Roman"
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: deviceInstallTimeTxt
            width: 160
            // height: 40
            text: qsTr("安装时间")
            //font.family: "Times New Roman"
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: periodTxt
            width: 150
            // height: 40
            text: qsTr("有效期")
            //font.family: "Times New Roman"
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: pathTxt
            //font.family: "Times New Roman"
            font.pixelSize: 14
            width: 150
            text: qsTr("图标路径")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }
    ScrollView
    {
        anchors.top: titleRow.bottom
        anchors.left: parent.left
        anchors.bottom: buttons.top
        anchors.right: parent.right
        ListView {
            id: listView
            anchors.fill: parent
            model: listModel
            delegate: listDelegate

            clip: true
        }
    }
    Row {
        id: buttons
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        //anchors.bottomMargin: 40
        anchors.margins: 10
        spacing: 20
        NaviButton {
            id: addItemBtn
            text: qsTr("增加项目")
            onClicked: {
                var obj = new Object
                var curDate = new Date
                var periodOfvalidity= new Date
                periodOfvalidity.setFullYear(curDate.getFullYear()+13)
                obj["imagePath"] = Qt.resolvedUrl(
                            decodeURI("qrc:/images/alarmDevice.svg"))
                obj["deviceName"] = String(qsTr("报警装置%1").arg(listModel.count))
                obj["manufacturers"] = qsTr("北京利达华信电子有限公司")
                obj["periodOfvalidity"] = Qt.formatDate(periodOfvalidity,"yyyy/MM/dd")
                obj["deviceInstallTime"] = Qt.formatDate(curDate,"yyyy/MM/dd")
                obj["selectIconBtnName"] = qsTr("选择图标")
                listModel.append(obj)
                saveInfo()
            }
        }


        NaviButton {
            id: batchInsertItemBtn
            text: qsTr("批量插入")
            onClicked: {

                selectFilesDialog.open()
            }
        }

        NaviButton {
            id: saveBtn
            text: qsTr("保存")
            onClicked: {
                saveInfo()
            }
        }

        NaviButton {
            id: clearBtn
            text: qsTr("清空")
            onClicked: {
                listModel.clear()
                itemIconInfo.clearIconInfo()
                itemIconInfo.setCurrentIconIndex(-1)
                saveInfo()
                emit:clearIcons()
            }
        }
    }

    ItemIconInfoToJson {
        id: itemIconInfo
    }



    function formatText(count, modelData) {
        var data = count === 12 ? modelData + 1 : modelData;
        return data.toString().length < 2 ? "0" + data : data;
    }
    function saveInfo() {

        if(listModel.count>0)
        {
            for (var i = 0; i < listModel.count; i++) {
                var obj = new Object
                obj = listModel.get(i)

                itemIconInfo.saveItemIconInfo(String("%1").arg(i), "deviceName",
                                              obj["deviceName"].toString())
                itemIconInfo.saveItemIconInfo(String("%1").arg(i), "imagePath",
                                              obj["imagePath"].toString())

                itemIconInfo.saveItemIconInfo(String("%1").arg(i), "manufacturers",
                                              obj["manufacturers"].toString())
                itemIconInfo.saveItemIconInfo(String("%1").arg(i), "deviceInstallTime",
                                              obj["deviceInstallTime"].toString())
                itemIconInfo.saveItemIconInfo(String("%1").arg(i),
                                              "periodOfvalidity",
                                              obj["periodOfvalidity"].toString())
                itemIconInfo.saveItemIconInfo(String("%1").arg(i),
                                              "selectIconBtnName",
                                              obj["selectIconBtnName"].toString())

            }


        }
        itemIconInfo.itemIconInfoToJson()
        emit: saveItemInfoToJson()
    }
    function readInfo() {

        var size = itemIconInfo.sizeOfHash()
        if (size === 0) {
            listModel.clear()
            return
        }
        var itemIconInfoStr = new String
        if(listModel.count>0)
        {
            if (size >= listModel.count) {
                for (var currentIndex = 0; currentIndex < listModel.count; currentIndex++) {
                    var currentObj = new Object
                    currentObj["deviceInstallTime"] = itemIconInfo.getValue(String("%1").arg(currentIndex),"deviceInstallTime");
                    currentObj["deviceName"] = itemIconInfo.getValue(String("%1").arg(currentIndex),"deviceName");
                    currentObj["imagePath"] = itemIconInfo.getValue(String("%1").arg(currentIndex),"imagePath");
                    currentObj["manufacturers"] = itemIconInfo.getValue(String("%1").arg(currentIndex),"manufacturers");
                    currentObj["periodOfvalidity"] = itemIconInfo.getValue(String("%1").arg(currentIndex),"periodOfvalidity");
                    currentObj["selectIconBtnName"]=itemIconInfo.getValue(String("%1").arg(currentIndex),"selectIconBtnName");
                    listModel.set(currentIndex, currentObj)
                }

                for (var i = listModel.count; i < size; i++) {
                    var index = String("%1").arg(i)
                    var obj = new Object
                    obj["deviceInstallTime"] = itemIconInfo.getValue(index,"deviceInstallTime");
                    obj["deviceName"] = itemIconInfo.getValue(index,"deviceName");
                    obj["imagePath"] = itemIconInfo.getValue(index,"imagePath");
                    obj["manufacturers"] = itemIconInfo.getValue(index,"manufacturers");
                    obj["periodOfvalidity"] = itemIconInfo.getValue(index,"periodOfvalidity");
                    obj["selectIconBtnName"]=itemIconInfo.getValue(index,"selectIconBtnName");
                    listModel.append(obj)
                }

            }

            else {
                for (var j = 0; j < size; j++) {
                    var index2 = String("%1").arg(j)
                    var obj2 = new Object
                    obj2["deviceInstallTime"] = itemIconInfo.getValue(index2,"deviceInstallTime");
                    obj2["deviceName"] = itemIconInfo.getValue(index2,"deviceName");
                    obj2["imagePath"] = itemIconInfo.getValue(index2,"imagePath");
                    obj2["manufacturers"] = itemIconInfo.getValue(index2,"manufacturers");
                    obj2["periodOfvalidity"] = itemIconInfo.getValue(index2,"periodOfvalidity");
                    obj2["selectIconBtnName"]=itemIconInfo.getValue(index2,"selectIconBtnName");
                    listModel.append(obj2)
                }
            }
        }
    }


    Component.onDestruction: {
        saveInfo()
    }

    function initIconSetting()
    {
       itemIconInfo.initIconInfo()
       readInfo()
       emit: saveItemInfoToJson()
    }

    function retranslate()
    {

        iconTxt.text = qsTr("图标")
        deviceNameTxt.text = qsTr("设备")
        manufacturersTxt.text = qsTr("制造商")
        deviceInstallTimeTxt.text = qsTr("安装时间")
        periodTxt.text = qsTr("有效期")
        pathTxt.text = qsTr("图标路径")
        addItemBtn.text = qsTr("增加项目")
        batchInsertItemBtn.text = qsTr("批量插入")
        saveBtn.text = qsTr("保存")
        clearBtn.text = qsTr("清空")

    }
}
