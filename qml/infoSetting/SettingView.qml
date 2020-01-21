import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import QtQuick.Layouts 1.3
import "qrc:/qml/infoSetting"
import "qrc:/qml/databaseSetting"

import QtQuick.Controls.Styles 1.4
Rectangle
{
    visible: true
    width: 900
    height: 640
    signal controlCenterChecked(bool enable)
    signal heartbeatClose()
    signal initPasswordSetting()
    TabBar {
        id: bar
        width: parent.width
        MyTabButton {
            text: qsTr("串口设置")
        }
        MyTabButton {
            text: qsTr("建筑平面图设置")
        }
        MyTabButton {
            text:  qsTr("密码设置")
        }

        MyTabButton {
            text: qsTr("系统图设置")
        }
        MyTabButton {
            text: qsTr("语音设置")
        }

        MyTabButton
        {
            text:qsTr("维保信息")
        }

        MyTabButton
        {
            text:qsTr("消防控制室管理信息")
        }

        MyTabButton
        {
            text:qsTr("中心通信设置")
        }
    }

    StackLayout {
        width: parent.width
        currentIndex: bar.currentIndex

        SerialPortSetting {
            id: serialPortSetting
        }
        GraphicsViewSetting {
            id: graphicsViewSetting
        }

        PasswordSetting {
            id: passwordSetting

        }

        SysArchitePlanSetting {
            id: sysArchitePlanSetting
        }
        SpeechSetting {
            id: speechSetting
        }

        MaintenanceInfo {
            id: maintenanceInfo
        }
        InfoTransport {
            id: infoTransport
        }
        ControlCenterSetting{
            id: controlCenteSetting

            onControlCenterCheckBoxChecked:
            {
                emit:controlCenterChecked(enable)
                console.log(enable)
            }
            onCloseControlCenterHeartbeat:
            {
                emit:heartbeatClose()
            }
        }
    }
    function setPassword()
    {
        passwordSetting.initSetting()
    }

    function startEnableControlCenter(enable)
    {
       controlCenteSetting.controlCenterEnable(enable)
    }

//    function initPasswordSetting()
//    {
//       settingView.setPassword()
//    }
}

