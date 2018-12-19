import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4 as Controls1_4
import qmlTableModel 1.0
import "../qmlTable"

Item {
    SwipeView {
        id: swipView
        currentIndex: 0
        anchors.fill: parent

        AlarmInfoTableItem {
            id: alarmInfo
        }
        DeviceStateTableItem {
            id: deviceState
        }

        OperaEventTableItem {
            id: operaEvent
        }
    }

    PageIndicator {
        id: indicator
        count: swipView.count
        currentIndex: swipView.currentIndex
        anchors.bottom: swipView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
