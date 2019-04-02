import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import serialConfigurationManager 1.0
import userManager 1.0

Rectangle {
    //    width: 420
    //    height: 360
    property int baudRatesIndex: 0
    property int portNameIndex: 0
    property int dataBitsIndex: 0
    property int stopBitsIndex: 0
    property int parityIndex: 0
    property int flowContralIndex: 0
    GridLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            id: port
            text: qsTr("端口")
            Layout.row: 0
            Layout.column: 0
        }
        ComboBox {
            id: portComboBox
            Layout.row: 0
            Layout.column: 1
            model: ListModel {
                id: portListModel
            }
        }

        Text {
            id: baud
            text: qsTr("波特率")
            Layout.row: 1
            Layout.column: 0
        }
        ComboBox {
            id: baudComboBox
            Layout.row: 1
            Layout.column: 1
            model: ListModel {
                id: baudListModel
            }
        }

        Text {
            id: dataBits
            text: qsTr("数据位")
            Layout.row: 2
            Layout.column: 0
        }
        ComboBox {
            id: dataBitsComboBox
            Layout.row: 2
            Layout.column: 1
            model: [8, 7, 6, 5]
        }

        Text {
            id: stopBits
            text: qsTr("停止位")
            Layout.row: 3
            Layout.column: 0
        }
        ComboBox {
            id: stopBitsComboBox
            Layout.row: 3
            Layout.column: 1
            model: [1, 0]
        }

        Text {
            id: parity
            text: qsTr("校验")
            Layout.row: 4
            Layout.column: 0
        }
        ComboBox {
            id: parityComboBox
            Layout.row: 4
            Layout.column: 1
            model: ["无校验", "奇校验", "偶校验"]
        }

        Text {
            id: flowControl
            text: qsTr("流控制")
            Layout.row: 5
            Layout.column: 0
        }
        ComboBox {
            id: flowControlComboBox
            Layout.row: 5
            Layout.column: 1
            model: ["无"]
        }

        Button {
            id: saveBtn
            text: qsTr("保存")
            Layout.row: 6
            Layout.column: 1

            Layout.fillWidth: true
            onClicked: {
                SerialPortInfo.setConfigurationValue("portName",
                                                     portComboBox.currentText)
                SerialPortInfo.setConfigurationValue(
                            "baudRate", parseInt(baudComboBox.currentText, 10))

                SerialPortInfo.setConfigurationValue(
                            "dataBits",
                            parseInt(dataBitsComboBox.currentText, 10))

                SerialPortInfo.setConfigurationValue(
                            "stopBits",
                            parseInt(stopBitsComboBox.currentText, 10))
                SerialPortInfo.setConfigurationValue("parity",
                                                     parityComboBox.currentText)
                SerialPortInfo.setConfigurationValue(
                            "flowControl", flowControlComboBox.currentText)
                SerialPortInfo.setConfiguration()
                SerialPortInfo.saveConfiguration()
            }
        }
    }

    Component.onCompleted: {
        setBaudRatesValue()
        setPortName()
        setDataBits()
        setStopBits()
        setParity()
        setFlowControl()
    }
    function setBaudRatesValue() {
        for (var i = 0; i < SerialPortInfo.baudRatesNum(); i++) {

            baudListModel.insert(i, {
                                     value: SerialPortInfo.baudRatesValue(i)
                                 })
        }

        if (SerialPortInfo.baudRatesNum() > baudRatesIndex) {
            baudComboBox.currentIndex = baudRatesIndex
        }
    }
    function setPortName() {
        for (var i = 0; i < SerialPortInfo.portNameNum(); i++) {

            portListModel.insert(i, {
                                     value: SerialPortInfo.portNameValue(i)
                                 })
        }

        if (SerialPortInfo.portNameNum() > portNameIndex) {
            portComboBox.currentIndex = portNameIndex
        }
    }
    function setDataBits() {
        if (dataBitsComboBox.count > dataBitsIndex) {
            dataBitsComboBox.currentIndex = dataBitsIndex
        }
    }

    function setStopBits() {
        if (stopBitsComboBox.count > stopBitsIndex) {
            stopBitsComboBox.currentIndex = stopBitsIndex
        }
    }

    function setParity() {
        if (parityComboBox.count > parityIndex) {
            parityComboBox.currentIndex = parityIndex
        }
    }

    function setFlowControl() {
        if (flowControlComboBox.count > flowContralIndex) {
            flowControlComboBox.currentIndex = flowContralIndex
        }
    }
}
