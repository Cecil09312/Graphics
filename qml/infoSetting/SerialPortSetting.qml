import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import serialConfigurationManager 1.0
import userManager 1.0
import serialLink 1.0
import indicatorConfigurationManager 1.0
import indicatorLightCom 1.0

Rectangle {
    //    width: 420
    //    height: 360
    property int baudRatesIndex: 0
    property int portNameIndex: 0
    property int dataBitsIndex: 0
    property int stopBitsIndex: 0
    property int parityIndex: 0
    property int flowContralIndex: 0

    property int baudRatesIndex1: 0
    property int portNameIndex1: 0
    property int dataBitsIndex1: 0
    property int stopBitsIndex1: 0
    property int parityIndex1: 0
    property int flowContralIndex1: 0
    Row {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 50
        GroupBox {
            title: qsTr("主机通信")
            GridLayout {

                anchors.fill: parent
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
                    id: flushBtn
                    text: qsTr("刷新")
                    Layout.row: 6
                    Layout.column: 1

                    Layout.fillWidth: true
                    onClicked: {
                        setPortName()
                    }
                }

                Button {
                    id: saveBtn
                    text: qsTr("保存并打开")
                    Layout.row: 7
                    Layout.column: 1

                    Layout.fillWidth: true
                    onClicked: {
                        SerialPortInfo.setConfigurationValue(
                                    "portName", portComboBox.currentText)
                        SerialPortInfo.setConfigurationValue(
                                    "baudRate",
                                    parseInt(baudComboBox.currentText, 10))

                        SerialPortInfo.setConfigurationValue(
                                    "dataBits",
                                    parseInt(dataBitsComboBox.currentText, 10))

                        SerialPortInfo.setConfigurationValue(
                                    "stopBits",
                                    parseInt(stopBitsComboBox.currentText, 10))
                        SerialPortInfo.setConfigurationValue(
                                    "parity", parityComboBox.currentText)
                        SerialPortInfo.setConfigurationValue(
                                    "flowControl",
                                    flowControlComboBox.currentText)
                        SerialPortInfo.setConfiguration()
                        SerialPortInfo.saveConfiguration()
                        SerialLink.connectLink()
                    }
                }

                Button {
                    id: closeBtn
                    text: "关闭"
                    Layout.row: 8
                    Layout.column: 1
                    Layout.fillWidth: true
                    onClicked: {
                        SerialLink.disconnectLink()
                    }
                }
            }
        }

        GroupBox {
            title: qsTr("指示灯通信")
            GridLayout {
                anchors.fill: parent

                Text {
                    id: port1
                    text: qsTr("端口")
                    Layout.row: 0
                    Layout.column: 0
                }
                ComboBox {
                    id: portComboBox1
                    Layout.row: 0
                    Layout.column: 1
                    model: ListModel {
                        id: portListModel1
                    }
                }

                Text {
                    id: baud1
                    text: qsTr("波特率")
                    Layout.row: 1
                    Layout.column: 0
                }
                ComboBox {
                    id: baudComboBox1
                    Layout.row: 1
                    Layout.column: 1
                    model: ListModel {
                        id: baudListModel1
                    }
                }

                Text {
                    id: dataBits1
                    text: qsTr("数据位")
                    Layout.row: 2
                    Layout.column: 0
                }
                ComboBox {
                    id: dataBitsComboBox1
                    Layout.row: 2
                    Layout.column: 1
                    model: [8, 7, 6, 5]
                }

                Text {
                    id: stopBits1
                    text: qsTr("停止位")
                    Layout.row: 3
                    Layout.column: 0
                }
                ComboBox {
                    id: stopBitsComboBox1
                    Layout.row: 3
                    Layout.column: 1
                    model: [1, 0]
                }

                Text {
                    id: parity1
                    text: qsTr("校验")
                    Layout.row: 4
                    Layout.column: 0
                }
                ComboBox {
                    id: parityComboBox1
                    Layout.row: 4
                    Layout.column: 1
                    model: ["无校验", "奇校验", "偶校验"]
                }

                Text {
                    id: flowControl1
                    text: qsTr("流控制")
                    Layout.row: 5
                    Layout.column: 0
                }
                ComboBox {
                    id: flowControlComboBox1
                    Layout.row: 5
                    Layout.column: 1
                    model: ["无"]
                }

                Button {
                    id: flushBtn1
                    text: qsTr("刷新")
                    Layout.row: 6
                    Layout.column: 1

                    Layout.fillWidth: true
                    onClicked: {
                        setPortName1()
                    }
                }

                Button {
                    id: saveBtn1
                    text: qsTr("保存并打开")
                    Layout.row: 7
                    Layout.column: 1

                    Layout.fillWidth: true
                    onClicked: {

                        IndicatorConfiguration.setConfigurationValue(
                                    "portName", portComboBox1.currentText)
                        IndicatorConfiguration.setConfigurationValue(
                                    "baudRate",
                                    parseInt(baudComboBox1.currentText, 10))

                        IndicatorConfiguration.setConfigurationValue(
                                    "dataBits",
                                    parseInt(dataBitsComboBox1.currentText, 10))

                        IndicatorConfiguration.setConfigurationValue(
                                    "stopBits",
                                    parseInt(stopBitsComboBox1.currentText, 10))
                        IndicatorConfiguration.setConfigurationValue(
                                    "parity", parityComboBox1.currentText)
                        IndicatorConfiguration.setConfigurationValue(
                                    "flowControl",
                                    flowControlComboBox1.currentText)
                        IndicatorConfiguration.setConfiguration()
                        IndicatorConfiguration.saveConfiguration()
                        IndicatorLightCom.connectLink()
                    }
                }

                Button {
                    id: closeBtn1
                    text: qsTr("关闭")
                    Layout.row: 8
                    Layout.column: 1

                    Layout.fillWidth: true
                    onClicked: {
                        IndicatorLightCom.disconnectLink()
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        setBaudRatesValue()
        setPortName()
        setPortName1()
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

        baudComboBox.currentIndex = baudComboBox.find(
                    String("%1").arg(SerialPortInfo.currentBaudRate()))

        for (var j = 0; j < IndicatorConfiguration.baudRatesNum(); j++) {

            baudListModel1.insert(j, {
                                      value: IndicatorConfiguration.baudRatesValue(
                                                 j)
                                  })
        }

        baudComboBox1.currentIndex = baudComboBox1.find(
                    String("%1").arg(IndicatorConfiguration.currentBaudRate()))
    }
    function setPortName() {
        portListModel.clear()

        for (var i = 0; i < SerialPortInfo.portNameNum(); i++) {
            portListModel.insert(i, {
                                     value: SerialPortInfo.portNameValue(i)
                                 })
        }
        portComboBox.currentIndex = portComboBox.find(
                    SerialPortInfo.currentPortName())
    }

    function setPortName1() {

        portListModel1.clear()
        for (var j = 0; j < IndicatorConfiguration.portNameNum(); j++) {

            portListModel1.insert(j, {
                                      value: IndicatorConfiguration.portNameValue(
                                                 j)
                                  })
        }
        portComboBox1.currentIndex = portComboBox1.find(
                    IndicatorConfiguration.currentPortName())
    }

    function setDataBits() {
        dataBitsComboBox.currentIndex = dataBitsComboBox.find(
                    String("%1").arg(SerialPortInfo.currentDataBits()))
        dataBitsComboBox1.currentIndex = dataBitsComboBox1.find(
                    String("%1").arg(IndicatorConfiguration.currentDataBits()))
    }

    function setStopBits() {

        stopBitsComboBox.currentIndex = stopBitsComboBox.find(
                    String("%1").arg(SerialPortInfo.currentStopBits()))
        stopBitsComboBox1.currentIndex = stopBitsComboBox1.find(
                    String("%1").arg(IndicatorConfiguration.currentStopBits()))
    }

    function setParity() {
        parityComboBox.currentIndex = parityComboBox.find(
                    SerialPortInfo.currentParity())
        parityComboBox1.currentIndex = parityComboBox1.find(
                    IndicatorConfiguration.currentParity())
    }

    function setFlowControl() {

        flowControlComboBox.currentIndex = flowControlComboBox.find(
                    SerialPortInfo.currentFlowControl())
        flowControlComboBox1.currentIndex = flowControlComboBox1.find(
                    IndicatorConfiguration.currentFlowControl())
    }
}
