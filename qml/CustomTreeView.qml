import QtQuick 2.0
import QtQuick.Controls 1.4 as Contrals1_4
import QtQuick.Controls 2.2

Contrals1_4.TreeView {
    id: treeView
    width: 180
    height: 500
    Contrals1_4.TableViewColumn {
        role: "architePlan"
        title: qsTr("建筑平面图")
        width: treeView.width
        elideMode: Text.ElideMiddle
    }

    itemDelegate: Item {
        Text {
            anchors.verticalCenter: parent.verticalCenter
            color: styleData.textColor
            elide: styleData.elideMode
            text: styleData.value
        }
    }
}
