import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3

Item {
    //    width: 420
    //    height: 360
    GridLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        columnSpacing: 5

        Text {
            id: zoomSettingTxt
            text: qsTr("缩放系数:")
            Layout.row: 0
            Layout.column: 0
        }
        SpinBox {
            id: zoomValue
            Layout.row: 0
            Layout.column: 1
            Layout.fillWidth: true
        }

        Text {
            id: zoomTxt
            text: qsTr("缩放:")
            Layout.row: 1
            Layout.column: 0
        }
        Slider {
            id: zoomSlider
            Layout.row: 1
            Layout.column: 1
            Layout.fillWidth: true
        }
    }
}
