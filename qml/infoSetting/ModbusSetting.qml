import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import Qt.labs.platform 1.0


Item
{
    Column
    {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 5
        Row
        {

            id:radioBtnRow
            RadioButton
            {
                text: "serial"
                checked: true
                onCheckedChanged:
                {
                    if(checked)
                    {
                        serialGroupBox.visible=true
                        tcpGroupBox.visible=false
                        Modbus.setModbusType(Modbus.SerialRtu)
                    }

                }

            }
            RadioButton
            {
                text: "tcp"
                onCheckedChanged:
                {
                    if(checked)
                    {
                        serialGroupBox.visible=false
                        tcpGroupBox.visible=true
                        Modbus.setModbusType(Modbus.Tcp)
                       // Modbus.modbusType=Modbus.Tcp
                    }

                }

            }

            CheckBox
            {
              id:selectBaseCheckBox
              text: "base1"
              onCheckedChanged:
              {

                  if(checked)
                  {
                      text="base0"

                  }
                  else
                  {
                     text="base1"
                  }
                  Modbus.setIsBase0(checked);
              }

            }
        }

        GroupBox
        {
            id:serialGroupBox

            Column
            {
                spacing: 10
                Grid
                {
                    columns: 4
                    spacing: 5
                    Text {
                        id: portNameTxt
                        height: 35
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("端口")
                    }
                    ComboBox
                    {
                        id:portNameComboBox

                    }

                    Text {
                        id: baudTxt
                        height: 35
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("波特率")
                    }
                    ComboBox
                    {
                        id:baudComboBox

                    }

                    Text {
                        id: dataBitsTxt
                        height: 35
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("数据位")
                    }
                    ComboBox
                    {
                        id:dataBitsComboBox
                        model: [8,7,6,5]

                    }

                    Text {
                        id: stopBitsTxt
                        height: 35
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("停止位")
                    }
                    ComboBox
                    {
                        id:stopBitsComboBox
                        model: [0,1]

                    }
                    Text {
                        id: parityTxt
                        height: 35
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("校验")
                    }
                    ComboBox
                    {
                        id:parityComboBox
                        model: ["NoParity"]

                    }


                }

                Row
                {
                    id:btnRow
                    spacing: 5

                    NaviButton
                    {
                        id:openSerialBtn
                      text: qsTr("打开")
                      onClicked:
                      {
                          Modbus.settingSerialModbus(portNameComboBox.currentText,parseInt(baudComboBox.currentText)
                                                     ,parseInt(dataBitsComboBox.currentText),parseInt(stopBitsComboBox.currentText),parityComboBox.currentText)
                          Modbus.connectRtuSerialModbus()

                      }

                    }

                    NaviButton
                    {
                        id:frushBtn
                      text: qsTr("刷新")
                      onClicked:
                      {
                          baudComboBox.model=Modbus.baudRateModel()
                          portNameComboBox.model = Modbus.portNameModel()
                          baudComboBox.currentIndex = baudComboBox.find("9600")

                      }

                    }

                    NaviButton
                    {
                        id:closeSerialBtn
                      text: qsTr("关闭")
                      onClicked:
                      {
                         Modbus.closeRtuSerialModbus();

                      }


                    }


                }


            }
        }

        GroupBox
        {

            id:tcpGroupBox
            visible: false
            Column
            {
                spacing: 10
                Grid
                {
                    columns: 2
                    spacing: 5
                    Text {
                        id: addrTxt
                        height: 35
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("地址")
                    }
                    TextField
                    {
                      id:addrTextField

                    }

                    Text {
                        id: tcpPortTxt
                        height: 35
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("端口")
                    }
                    TextField
                    {
                      id:tcpPortTextField

                    }


                }

                Row
                {
                    id:tcpBtnRow
                    spacing: 5

                    NaviButton
                    {
                        id:tcpConnectBtn
                      text: qsTr("连接")
                      onClicked:
                      {

                          Modbus.settingTcpModbus(addrTextField.text,parseInt(tcpPortTextField.text))
                          Modbus.connectTcpModbus()
                      }
                     // modbusObj.connectTcpModbus()
                    }



                    NaviButton
                    {
                        id:tcpCloseBtn
                      text: qsTr("断开")
                      onClicked:
                      {

                       Modbus.closeTcpModbus()
                      }


                    }


                }

            }
        }

        Text {
            id: stateTxt
            font.pointSize: 12
            height: 40
        }
    }

    function init()
    {
        baudComboBox.model=Modbus.baudRateModel()
        portNameComboBox.model = Modbus.portNameModel()
        baudComboBox.currentIndex = baudComboBox.find("9600")
        portNameComboBox.currentIndex=portNameComboBox.find(Modbus.portName())
        dataBitsComboBox.currentIndex=dataBitsComboBox.find(Modbus.dataBits())
        stopBitsComboBox.currentIndex=stopBitsComboBox.find(Modbus.stopBits())
        parityComboBox.currentIndex=parityComboBox.find(Modbus.parity())
        baudComboBox.currentIndex = baudComboBox.find(Modbus.baudRate())
        tcpPortTextField.text=Modbus.ipPort()
        addrTextField.text = Modbus.ipAddr()


    }

    function saveModbusInfo()
    {
        Modbus.settingSerialModbus(portNameComboBox.currentText,parseInt(baudComboBox.currentText)
                                   ,parseInt(dataBitsComboBox.currentText),parseInt(stopBitsComboBox.currentText),parityComboBox.currentText)

       Modbus.settingTcpModbus(addrTextField.text,parseInt(tcpPortTextField.text))
       Modbus.saveToJson()
    }
    
    function setSerialModbusState(isOk)
    {
        if(isOk)
        {
            stateTxt.color="green"
            stateTxt.text=qsTr("modbus 串口连接成功")
        }
        else
        {
            stateTxt.color="red"
            stateTxt.text=qsTr("modbus 串口连接失败")
        }
        
    }
    function setTcpModbusState(isOk)
    {
        
        if(isOk)
        {
            stateTxt.color="green"
            stateTxt.text=qsTr("modbus Tcp连接成功")
        }
        else
        {
            stateTxt.color="red"
            stateTxt.text=qsTr("modbus Tcp连接失败")
        }
    }

    function retranslate()
    {
        portNameTxt.text=qsTr("端口")
        baudTxt.text=qsTr("波特率")
        dataBitsTxt.text=qsTr("数据位")
        stopBitsTxt.text=qsTr("停止位")
        parityTxt.text=qsTr("校验")
        openSerialBtn.text=qsTr("打开")
        frushBtn.text=qsTr("刷新")
        closeSerialBtn.text=qsTr("关闭")
        addrTxt.text=qsTr("地址")
        tcpPortTxt.text=qsTr("端口")
        tcpConnectBtn.text=qsTr("连接")
        tcpCloseBtn.text=qsTr("断开")
    }

}
