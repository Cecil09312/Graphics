import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import ftpConfigurationManager 1.0
import tcpConfigurationManager 1.0
import tcpLink 1.0

Item {

    Column {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10
        GroupBox {
            id: tcpSettingGroupBox
            title: qsTr("TCP信息设置")
            // anchors.verticalCenter: parent.verticalCenter
            // anchors.horizontalCenter: parent.horizontalCenter
            GridLayout {
                Text {
                    text: qsTr("地址:")
                    Layout.row: 0
                    Layout.column: 0
                }

                TextField {
                    id: tcpAddrTextField
                    Layout.row: 0
                    Layout.column: 1
                }

                Text {
                    text: qsTr("端口:")
                    Layout.row: 1
                    Layout.column: 0
                }

                TextField {
                    id: tcpPortTextField
                    Layout.row: 1
                    Layout.column: 1
                }

                Row {
                    spacing: 5
                    Layout.row: 2
                    Layout.column: 1
                    Button {
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

                    Button {
                        id: closeTcpInfoBtn
                        text: qsTr("关闭")

                        onClicked: {

                            TcpLink.disconnectLink()
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
                    text: qsTr("地址:")
                    Layout.row: 0
                    Layout.column: 0
                }

                TextField {
                    id: ftpAddrTextField
                    Layout.row: 0
                    Layout.column: 1
                    text: "127.0.0.1"
                }

                Text {
                    text: qsTr("端口:")
                    Layout.row: 1
                    Layout.column: 0
                }

                TextField {
                    id: ftpPortTextField
                    Layout.row: 1
                    Layout.column: 1
                    text: "21"
                }

                Text {
                    text: qsTr("用户:")
                    Layout.row: 2
                    Layout.column: 0
                }

                TextField {
                    id: userTextField
                    Layout.row: 2
                    Layout.column: 1
                    text: "sss"
                }

                Text {
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
                }

                Button {
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

        Component.onCompleted: {

            setFtpHost(FtpInfo.ftpHost())
            setFtpPort(String("%1").arg(FtpInfo.ftpPort()))

            setFtpUser(FtpInfo.ftpUser())
            setFtpPassword(FtpInfo.ftpPassword())
            setTcpAddr(TcpInfo.tcpAddr())
            setTcpPort(String("%1").arg(TcpInfo.tcpPort()))
        }
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
}
