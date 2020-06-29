import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import ftpConfigurationManager 1.0
import tcpConfigurationManager 1.0
import tcpLink 1.0
import transportInfo 1.0


Item {

    signal controlCenterCheckBoxChecked(bool enable)
    signal closeControlCenterHeartbeat();
    CheckBox
    {
        id:controlCenterCheckBox
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.bottom: row.top
        anchors.left: row.left
        text: qsTr("中心通信权限打开")
        checked: true
        onCheckedChanged:
        {
            enableControlCenter(checked)
            emit:controlCenterCheckBoxChecked(checked)
            TransportInfo.setControlCenterRight(checked)

        }
    }
    Row
    {
        id:row
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20


        Column {

            spacing: 10

            GroupBox {
                id: tcpSettingGroupBox
                title: qsTr("TCP信息设置")
                // anchors.verticalCenter: parent.verticalCenter
                // anchors.horizontalCenter: parent.horizontalCenter
                GridLayout {
                    Text {
                        id:addrTxt
                        text: qsTr("地址:")
                        Layout.row: 0
                        Layout.column: 0
                    }

                    TextField {
                        id: tcpAddrTextField
                        Layout.row: 0
                        Layout.column: 1
                        selectByMouse: true
                        selectionColor: "blue"
                        selectedTextColor: "white"
                    }

                    Text {
                        id:portTxt
                        text: qsTr("端口:")
                        Layout.row: 1
                        Layout.column: 0

                    }

                    TextField {
                        id: tcpPortTextField
                        Layout.row: 1
                        Layout.column: 1
                        selectByMouse: true
                        selectionColor: "blue"
                        selectedTextColor: "white"
                    }

                    Row {
                        spacing: 5
                        Layout.row: 2
                        Layout.column: 1
                        NaviButton {
                            id: saveTcpInfoBtn
                            text: qsTr("保存并连接")

                            onClicked: {
                                TcpInfo.setConfigurationValue("hostAddr",
                                                              tcpAddrTextField.text)
                                TcpInfo.setConfigurationValue(
                                            "port", parseInt(tcpPortTextField.text))
                                TcpInfo.setConfiguration()
                                TcpInfo.saveConfiguration()
                                TcpLink.connectLink()
                            }
                        }

                        NaviButton {
                            id: closeTcpInfoBtn
                            text: qsTr("关闭")

                            onClicked: {

                                TcpLink.disconnectLink()
                                emit: closeControlCenterHeartbeat()
                            }
                        }
                    }
                }
            }

            GroupBox {
                id: ftpSettingGroupBox
                //            anchors.top: tcpSettingGroupBox.bottom
                //            anchors.left: tcpSettingGroupBox.left
                //            anchors.right: tcpSettingGroupBox.right
                title: qsTr("FTP信息设置")
                GridLayout {
                    Text {
                        id:ftpAddrTxt
                        text: qsTr("地址:")
                        Layout.row: 0
                        Layout.column: 0
                    }

                    TextField {
                        id: ftpAddrTextField
                        Layout.row: 0
                        Layout.column: 1
                        text: "192.195.1.1"
                        selectByMouse: true
                        selectionColor: "blue"
                        selectedTextColor: "white"
                    }

                    Text {
                        id:ftpPortTxt
                        text: qsTr("端口:")
                        Layout.row: 1
                        Layout.column: 0
                    }

                    TextField {
                        id: ftpPortTextField
                        Layout.row: 1
                        Layout.column: 1
                        text: "21"
                        selectByMouse: true
                        selectionColor: "blue"
                        selectedTextColor: "white"
                    }

                    Text {
                        id:ftpUserTxt
                        text: qsTr("用户:")
                        Layout.row: 2
                        Layout.column: 0
                    }

                    TextField {
                        id: userTextField
                        Layout.row: 2
                        Layout.column: 1
                        text: "sss"
                        selectByMouse: true
                        selectionColor: "blue"
                        selectedTextColor: "white"
                    }

                    Text {
                        id:ftpPasswordTxt
                        text: qsTr("密码:")
                        Layout.row: 3
                        Layout.column: 0
                    }

                    TextField {
                        id: passwordTextField
                        Layout.row: 3
                        Layout.column: 1
                        text: "1234"
                        echoMode: TextInput.Password
                        selectByMouse: true
                        selectionColor: "blue"
                        selectedTextColor: "white"
                    }

                    NaviButton {
                        id: saveftpInfoBtn
                        text: qsTr("保存")
                        Layout.row: 4
                        Layout.column: 1
                        onClicked: {
                            FtpInfo.setConfigurationValue("host",
                                                          ftpAddrTextField.text)
                            FtpInfo.setConfigurationValue(
                                        "port", parseInt(ftpPortTextField.text, 10))

                            FtpInfo.setConfigurationValue("user",
                                                          userTextField.text)
                            FtpInfo.setConfigurationValue("password",
                                                          passwordTextField.text)

                            FtpInfo.setConfiguration()
                            FtpInfo.saveConfiguration()
                        }
                    }
                }

            }


        }

    }
    Component.onCompleted: {

        setFtpHost(FtpInfo.ftpHost())
        setFtpPort(String("%1").arg(FtpInfo.ftpPort()))

        setFtpUser(FtpInfo.ftpUser())
        setFtpPassword(FtpInfo.ftpPassword())
        setTcpAddr(TcpInfo.tcpAddr())
        setTcpPort(String("%1").arg(TcpInfo.tcpPort()))


    }
    function setFtpHost(host) {
        ftpAddrTextField.text = host
    }

    function setFtpPort(port) {
        ftpPortTextField.text = port
    }

    function setFtpUser(user) {
        userTextField.text = user
    }

    function setFtpPassword(password) {
        passwordTextField.text = password
    }

    function setTcpAddr(addr) {
        tcpAddrTextField.text = addr
    }

    function setTcpPort(port) {
        tcpPortTextField.text = port
    }

    function enableControlCenter(enable)
    {
        tcpSettingGroupBox.enabled=enable
        ftpSettingGroupBox.enabled = enable 
    }

    function controlCenterEnable(enable)
    {
       controlCenterCheckBox.checked =enable
    }

    function retranslate()
    {

        controlCenterCheckBox.text = qsTr("中心通信权限打开")
        tcpSettingGroupBox.title =qsTr("TCP信息设置")
        addrTxt.text = qsTr("地址:")
        portTxt.text = qsTr("端口:")
        saveTcpInfoBtn.text = qsTr("保存并连接")
        closeTcpInfoBtn.text = qsTr("关闭")
        ftpSettingGroupBox.title = qsTr("FTP信息设置")
        ftpAddrTxt.text = qsTr("地址:")
        ftpPortTxt.text =  qsTr("端口:")
        ftpUserTxt.text = qsTr("用户:")
        ftpPasswordTxt.text = qsTr("密码:")
        saveftpInfoBtn.text = qsTr("保存")
    }

}
