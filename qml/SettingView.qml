import QtQuick 2.9
import QtQuick.Controls 2.4

PathView {
    id: pathView
    signal launched(string page)
    readonly property int cX: width / 2
    readonly property int cY: height / 2
    readonly property int itemSize: size / 4
    readonly property int size: Math.min(width - 80, height)
    readonly property int radius: size / 2 - itemSize / 3
    snapMode: PathView.SnapToItem
    model: ListModel {
        ListElement {
            title: qsTr("串口设置")
            page: "qrc:/qml/SerialPortSetting.qml"
            icon: "communication"
            iconSource: "qrc:/images/communication.png"
        }

        ListElement {
            title: qsTr("建筑平面图设置")
            page: "qrc:/qml/GraphicsViewSetting.qml"
            icon: "geoGraphics"
            iconSource: "qrc:/images/geoGraphics.png"
        }

        ListElement {
            title: qsTr("密码设置")
            page: "qrc:/qml/PasswordSetting.qml"
            icon: "passwordSetting"
            iconSource: "qrc:/images/passwordSetting.png"
        }

        ListElement {
            title: qsTr("信息传输")
            page: "qrc:/qml/InfoTransport.qml"
            icon: "infoSend"
            iconSource: "qrc:/images/send.png"
        }

        ListElement {
            title: qsTr("其它设置")
            page: "qrc:/qml/ItemIconSetting.qml"
            icon: "build"
            iconSource: "qrc:/images/build.png"
        }

        ListElement {
            title: qsTr("信息查询")
            page: "qrc:/qml/InfoQuery.qml"
            icon: "search"
            iconSource: "qrc:/images/search.png"
        }

        ListElement {
            title: qsTr("系统图设置")
            page: "qrc:/qml/SysArchitePlanSetting.qml"
            icon: "appOther"
            iconSource: "qrc:/images/appOther.png"
        }
    }

    delegate: RoundButton {
        width: pathView.itemSize
        height: pathView.itemSize

        property string title: model.title
        BorderImage {
            anchors.fill: parent
            anchors.margins: 15
            source: model.iconSource
        }
        opacity: PathView.itemOpacity
        padding: 12

        background: Rectangle {
            radius: width / 2
            border.width: 3
            border.color: parent.PathView.isCurrentItem ? "#41cd52" : "#53586b"
        }

        onClicked: {
            if (PathView.isCurrentItem)
                pathView.launched(Qt.resolvedUrl(page))
            else
                pathView.currentIndex = index
        }
    }

    path: Path {
        startX: pathView.cX
        startY: pathView.cY
        PathAttribute {
            name: "itemOpacity"
            value: 1.0
        }
        PathLine {
            x: pathView.cX + pathView.radius
            y: pathView.cY
        }
        PathAttribute {
            name: "itemOpacity"
            value: 0.7
        }
        PathArc {
            x: pathView.cX - pathView.radius
            y: pathView.cY
            radiusX: pathView.radius
            radiusY: pathView.radius
            useLargeArc: true
            direction: PathArc.Clockwise
        }
        PathAttribute {
            name: "itemOpacity"
            value: 0.5
        }
        PathArc {
            x: pathView.cX + pathView.radius
            y: pathView.cY
            radiusX: pathView.radius
            radiusY: pathView.radius
            useLargeArc: true
            direction: PathArc.Clockwise
        }
        PathAttribute {
            name: "itemOpacity"
            value: 0.3
        }
    }

    Text {
        id: appTitle

        property Item currentItem: pathView.currentItem

        visible: currentItem ? currentItem.PathView.itemOpacity === 1.0 : 0

        text: currentItem ? currentItem.title : ""
        anchors.centerIn: parent
        anchors.verticalCenterOffset: (pathView.itemSize + height) / 2

        font.bold: true
        font.pixelSize: pathView.itemSize / 3
        font.letterSpacing: 1
        //   color: "#222840"
    }
}
