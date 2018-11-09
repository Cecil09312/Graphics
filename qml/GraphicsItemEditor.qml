import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0

Rectangle {
    width: 480
    height: 320
    signal selectColor(color selectColor)
    signal setSize(real size)
    signal setText(string txt)
    signal setGeoInfo(string geoInfo)
    signal setIcon(string icon)
    signal setTypeName(string name)
    signal setIsUseIcon(bool isUseIcon)
    GridLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 5
        Text {
            id: typeLabel
            text: qsTr("类型:")
            Layout.row: 0
            Layout.column: 0
        }
        ComboBox {
            id: typeComboBox
            Layout.row: 0
            Layout.column: 1
            Layout.fillWidth: true
            model: [qsTr("火警"), qsTr("联动"), qsTr("监管"), qsTr("故障"), qsTr(
                    "反馈"), qsTr("屏蔽")]
            onCurrentTextChanged: {
                emit: setTypeName(currentText)
            }
        }

        Text {
            id: num
            text: qsTr("编号:")
            Layout.row: 1
            Layout.column: 0
        }
        TextField {
            id: numTextField
            Layout.row: 1
            Layout.column: 1
            Layout.fillWidth: true
            onTextEdited: {
                emit: setText(text)
            }
        }

        Text {
            id: geoInfo
            text: qsTr("地理信息:")
            Layout.row: 2
            Layout.column: 0
        }
        TextField {
            id: geoInfoTextField
            Layout.row: 2
            Layout.column: 1
            Layout.fillWidth: true
            onTextEdited: {
                emit: setGeoInfo(text)
            }
        }

        Text {
            id: colorLabel
            text: qsTr("颜色:")
            Layout.row: 3
            Layout.column: 0
        }
        Rectangle {
            id: colorRec
            width: 150
            height: 40
            color: "red"
            Layout.row: 3
            Layout.column: 1
            Layout.fillWidth: true
        }

        Button {
            id: colorSelectBtn
            text: qsTr("选择颜色")
            Layout.row: 3
            Layout.column: 2

            onClicked: {
                colorDialog.open()
            }
        }

        Text {
            id: sizeLabel
            text: qsTr("大小:")
            Layout.row: 4
            Layout.column: 0
        }
        SpinBox {
            id: sizeSpinBox
            Layout.row: 4
            Layout.column: 1
            Layout.fillWidth: true
            from: 5
            to: 100
            stepSize: 1
            onValueChanged: {
                emit: setSize(value)
            }
        }
        CheckBox {
            id: useIconCheckBox
            text: qsTr("使用图标")
            Layout.row: 5
            Layout.column: 0
            onCheckStateChanged: {
                iconAddr.clear()
                if (checkState == Qt.Checked) {
                    iconAddr.enabled = true
                    iconSelectBtn.enabled = true
                } else {
                    iconAddr.enabled = false
                    iconSelectBtn.enabled = false
                }

                emit: setIsUseIcon(iconSelectBtn.enabled)
            }
        }
        TextField {
            id: iconAddr
            Layout.row: 5
            Layout.column: 1
            Layout.fillWidth: true
            enabled: false
        }

        Button {
            id: iconSelectBtn
            text: qsTr("选择图标")
            Layout.row: 5
            Layout.column: 2
            enabled: false
            onClicked: {
                fileDialog.open()
            }
        }

        //        RowLayout {
        //            Layout.row: 6
        //            Layout.column: 1
        //            spacing: 5
        //            Layout.topMargin: 10
        //            Button {
        //                id: saveBtn
        //                text: qsTr("保存")
        //            }

        //            Button {
        //                id: quitBtn
        //                text: qsTr("退出")
        //            }
        //        }
    }
    ColorDialog {
        id: colorDialog
        onAccepted: {
            colorRec.color = color
            emit: selectColor(color)
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: {
            console.log(currentFile.toString())
            iconAddr.text = currentFile.toString()
            emit: setIcon(iconAddr.text)
        }
    }
    function setSelectColor(selectColor) {
        colorRec.color = selectColor
    }

    function setIconName(iconName) {
        iconAddr.text = iconName
    }

    function setItemText(txt) {
        numTextField.text = txt
        //colorRec.color = selectColor;
    }

    function setItemType(type) {
        typeComboBox.currentIndex = typeComboBox.find(type)
        // iconAddr.text = iconName;
    }

    function setItemSize(size) {
        sizeSpinBox.value = size
        //colorRec.color = selectColor;
    }

    function setUseIcon(isUsIcon) {

        if (isUsIcon) {
            useIconCheckBox.checkState = Qt.Checked
        } else {
            useIconCheckBox.checkState = Qt.Unchecked
        }

        // iconAddr.text = iconName;
    }

    function setItemGeoInfo(geoInfo) {
        geoInfoTextField.text = geoInfo
    }
}
