import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import QtQuick.Layouts 1.3
import "qrc:/qml/infoSetting"
import QtQuick.Controls.Styles 1.4
Rectangle
{
    visible: true
    width: 980
    height: 720
    signal controlCenterChecked(bool enable)
    signal heartbeatClose()
    signal initPasswordSetting()
    TabBar {
        id: bar
        width: parent.width
        MyTabButton {
            id:serialSettingBtn
            text: qsTr("串口设置")
        }
        MyTabButton {
            id:architePlanBtn
            text: qsTr("建筑平面图设置")
        }
        MyTabButton {
            id:passwordSettingBtn
            text:  qsTr("密码设置")
        }

        MyTabButton {
            id:sysDiagramSettingBtn
            text: qsTr("系统图设置")
        }
        MyTabButton {
            id:voiceSettingBtn
            text: qsTr("语音设置")
        }

        MyTabButton
        {
            id:maintenInfoBtn
            text:qsTr("维保信息")
        }

        MyTabButton
        {
            id:managementInfoBtn
            text:qsTr("消防控制室管理信息")
        }

        MyTabButton
        {
            id:monitoringBtn
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
    function setSysArchiteInfo()
    {
        sysArchitePlanSetting.setSysArchitePlan()

    }

    //    function initPasswordSetting()
    //    {
    //       settingView.setPassword()
    //    }

    function retranslate()
    {
        serialSettingBtn.text = qsTr("串口设置")
        architePlanBtn.text = qsTr("建筑平面图设置")
        passwordSettingBtn.text = qsTr("密码设置")
        sysDiagramSettingBtn.text = qsTr("系统图设置")
        voiceSettingBtn.text =  qsTr("语音设置")
        maintenInfoBtn.text = qsTr("维保信息")
        managementInfoBtn.text = qsTr("消防控制室管理信息")
        monitoringBtn.text = qsTr("中心通信设置")
        serialPortSetting.retranslate()
        graphicsViewSetting.retranslate()
        passwordSetting.retranslate()
        sysArchitePlanSetting.retranslate()
        speechSetting.retranslate()
        maintenanceInfo.retranslate()
        controlCenteSetting.retranslate()
        infoTransport.retranslate()
        speechSetting.reinitSpeech()
    }

    function initSpeechSetting()
    {
      speechSetting.reinitSpeech()

    }
}

