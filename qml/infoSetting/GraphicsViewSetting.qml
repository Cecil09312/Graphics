import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import architePlanView 1.0
import Qt.labs.platform 1.0
import sysArchitePlanView 1.0
import architePlanView 1.0
import qmlForJson 1.0

Item {

    //    width: 420
    //    height: 360
    property real currentValue: 5
    ColumnLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        GroupBox {
            id: architePlanGroupBox
            title: qsTr("图纸缩放设置(建筑平面图/系统图)")
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
                    id: zoomValueSpinBox
                    Layout.row: 0
                    Layout.column: 1
                    Layout.fillWidth: true
                    from: 10
                    to: 100
                    value: 15
                    property int decimals: 1
                    property real realValue: value / 10

                    validator: DoubleValidator {
                        bottom: Math.min(zoomValueSpinBox.from,
                                         zoomValueSpinBox.to)
                        top: Math.max(zoomValueSpinBox.from,
                                      zoomValueSpinBox.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 10.0).toLocaleString(
                                    locale, 'f', zoomValueSpinBox.decimals)
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }
                    onValueChanged: {
                        SysArchitePlanView.setGraphicsViewScale(value / 10.0)
                    }
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
                    from: 0
                    to: 10
                    stepSize: 0.5
                    value: 5

                    onMoved: {

                        if (value > currentValue) {
                            SysArchitePlanView.currentGraphicsViewZoom(true)
                            ArchitePlanView.currentGraphicsViewZoom(true)
                        } else if (value < currentValue) {
                            SysArchitePlanView.currentGraphicsViewZoom(false)
                            ArchitePlanView.currentGraphicsViewZoom(false)
                        }
                        currentValue = value
                    }
                }
            }
        }

        GroupBox {
            id: globalArchitePlanGroupBox
            title: qsTr("建筑总平面布局图")
            RowLayout {

                Text {
                    id: globalBackground
                    text: qsTr("背景图")
                }
                TextField {
                    id: globalArchitePlanTextFiled
                }
                Button {
                    id: globalArchitePlanBtn
                    text: qsTr("选择路径")
                    onClicked: {
                        fileDialog.open()
                    }
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: {

            globalArchitePlanTextFiled.text = currentFile.toString()
            ArchitePlanView.setGlobalArchitePixmap(
                        globalArchitePlanTextFiled.text)
        }
    }

    QmlForJson {
        id: qmlforJson
    }

    Component.onCompleted: {

        var globalArchiteObj  = new Object
       globalArchiteObj = JSON.parse(qmlforJson.readFileToString( ))
        var globalArchitePlanStr = JSON.stringify(globalArchiteObj["grobalArchitePlan"].valueOf())
        globalArchitePlanTextFiled.text = globalArchitePlanStr
    }
}
