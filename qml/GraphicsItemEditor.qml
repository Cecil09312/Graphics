import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import itemIconInfoToJson 1.0

Rectangle {
    width: 560
    height: 360
    signal selectColor(color selectColor)
    signal setSize(real size)
    signal setText(string txt)
    signal setGeoInfo(string geoInfo)
    signal setIcon(string icon)
    signal setTypeName(string name)
    signal setIsUseIcon(bool isUseIcon)

    ListModel {
        id: deviceTypeModel
        ListElement {
            deviceName: qsTr("火警设备")
        }
    }

    Grid {

        anchors {
            fill: parent
            topMargin: 10
            leftMargin: 10
            rightMargin: 10
        }

        columnSpacing: 5
        rowSpacing: 5
        columns: 4
        Text {
            id: typeLabel
            text: qsTr("报警类型:")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }
        ComboBox {
            id: typeComboBox
            Layout.fillWidth: true
            width: 150
            height: 40
            model: [qsTr("火警"), qsTr("联动"), qsTr("监管"), qsTr("故障"), qsTr(
                    "反馈"), qsTr("屏蔽")]
            onCurrentTextChanged: {
                emit: setTypeName(currentText)
            }
        }

        Text {
            text: qsTr("设备设施型号:")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }
        ComboBox {
            id: deviceTypeComboBox
            width: 150
            height: 40
            model: deviceTypeModel
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
        }

        Text {
            text: qsTr("地址号:")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: addNumTextField
            width: 150
            height: 40
        }

        Text {
            id: num
            text: qsTr("设备产品编码:")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: numTextField
            Layout.fillWidth: true
            width: 150
            height: 40
            onTextEdited: {
                emit: setText(text)
            }
        }

        Text {
            text: qsTr("设备所属系统:")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: deviceSysTextField
            Layout.fillWidth: true
            width: 150
            height: 40
        }

        Text {
            text: qsTr("总保护区域名称:")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: protectedAreaTextField
            Layout.fillWidth: true
            width: 150
            height: 40
        }

        Text {
            id: geoInfo
            text: qsTr("建筑设施名称:")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: geoInfoTextField
            Layout.fillWidth: true
            width: 150
            height: 40
            onTextEdited: {
                emit: setGeoInfo(text)
            }
        }

        Text {

            text: qsTr("设施所在楼层:")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: floorOfDeviceTextField
            Layout.fillWidth: true
            width: 150
            height: 40
        }
        Text {

            text: qsTr("设施所在位置:")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: deviceLocationTextField
            Layout.fillWidth: true
            width: 150
            height: 40
        }
        Text {

            text: qsTr("值班人员:")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: operatorOnDutyTextField
            Layout.fillWidth: true
            width: 150
            height: 40
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

    ItemIconInfoToJson {
        id: itemIconInfo
    }

    function setItemText(txt) {
        numTextField.text = txt
        //colorRec.color = selectColor;
    }

    function setItemType(type) {
        typeComboBox.currentIndex = typeComboBox.find(type)
        // iconAddr.text = iconName;
    }

    function setItemSize(size) {
        sizeSpinBox.value = size
        //colorRec.color = selectColor;
    }

    function setItemGeoInfo(geoInfo) {
        geoInfoTextField.text = geoInfo
    }

    function readInfo() {
        if (deviceTypeModel.count > 0) {
            deviceTypeModel.clear()
        }
        var size = itemIconInfo.sizeOfHash()
        var itemIconInfoStr = itemIconInfo.readFileFromJson()
        for (var i = 0; i < size; i++) {
            var index = String("%1").arg(i)
            var obj = JSON.parse(itemIconInfoStr)[index]
            console.log(obj["deviceName"])

            //var currentObj = JSON.parse(obj["deviceName"].toString())
            deviceTypeModel.append({
                                       deviceName: obj["deviceName"]
                                   })
        }
    }

    Component.onCompleted: {
        readInfo()
    }
}
