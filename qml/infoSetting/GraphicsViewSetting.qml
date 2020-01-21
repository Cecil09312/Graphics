import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import architePlanView 1.0
import Qt.labs.platform 1.0
import sysArchitePlanView 1.0
import architePlanView 1.0
import qmlForJson 1.0
import operatorInfo 1.0


Item {

    width: 800
    height: 640
    property real currentValue: 5
    property url filePath: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
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
                    verticalAlignment: Text.AlignVCenter
                    height: 30
                    Layout.row: 0
                    Layout.column: 0
                }
                SpinBox {
                    id: zoomValueSpinBox
                    Layout.row: 0
                    Layout.column: 1
                    Layout.fillWidth: true
                    width: 200
                    from: 10
                    to: 1000
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
                    verticalAlignment: Text.AlignVCenter
                    height: 30
                    Layout.row: 1
                    Layout.column: 0
                }
                Slider {
                    id: zoomSlider
                    Layout.row: 1
                    Layout.column: 1
                    Layout.fillWidth: true
                    width: 200
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

        GroupBox
        {
            id: itemLimitGroupBox
            title: qsTr("添加、删除、移动图标权限设置")
            Column
            {

                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                CheckBox
                {
                    id:limitOpen
                    text: qsTr("权限开启")
                    onClicked:
                    {
                      ArchitePlanView.setItemLimit(checked)
                    }

                }

                Text
                {
                  color: "red"
                  font
                  {
                    family:qsTr("宋体")
                    pointSize:12

                  }

                text:  qsTr("说明:1.添加图标:双击鼠标左键
2.删除图标:在图标上点击鼠标右键，在弹出的菜单中选中删除
3.移动图标:在图标上点鼠标左键，移动鼠标，图标也会跟着移动，释放鼠标左键图标位置即可确定。
4.选中图标:在图标上点击一下鼠标左键，可以选中图标
5.选中多个图标:按住键盘上的Ctrl键，在每个图标上面点击一下鼠标左键"
                            )
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
                    verticalAlignment: Text.AlignVCenter
                    height: 30
                }
                TextField {
                    id: globalArchitePlanTextFiled
                    readOnly: true
                }
                NaviButton {
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
        folder: filePath
       // folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: {
            OperatorInfo.insertEvent(
                        qsTr("建筑总平面布局图变更"),
                        qsTr(String("%1变更为%2").arg(
                                 globalArchitePlanTextFiled.text).arg(
                                 currentFile.toString())))
            globalArchitePlanTextFiled.text = currentFile.toString()
            filePath = file
            ArchitePlanView.setGlobalArchitePixmap(
                        globalArchitePlanTextFiled.text)
        }
    }

    QmlForJson {
        id: qmlforJson
    }

    Component.onCompleted: {

        var globalArchiteObj = new Object
        var jsonStr = new String
        jsonStr = qmlforJson.readFileToString()
        if (jsonStr.length > 0) {
            globalArchiteObj = JSON.parse(jsonStr)
            var globalArchitePlanStr = JSON.stringify(
                        globalArchiteObj["grobalPlanPicture"].valueOf())
            globalArchitePlanTextFiled.text = globalArchitePlanStr
        }
    }
}
