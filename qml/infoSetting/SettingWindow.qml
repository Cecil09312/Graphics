import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Contral_1_4

ApplicationWindow {
    id: window
    visible: true
    width: 840
    height: 640

    SettingView {
        id: settingView
        anchors.fill: parent
    }
}
