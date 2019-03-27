import QtQuick 2.9
import QtQuick.Controls 2.2
import Qt.labs.platform 1.0
import itemIconInfoToJson 1.0

Rectangle {
    width: 680
    height: 480

    signal saveItemInfoToJson
    ListModel {
        id: listModel
        ListElement {
            deviceName: qsTr("报警装置")
            imagePath: "qrc:/images/fireAlarm.png"
            manufacturers: qsTr("北京利达华信电子有限公司")
            periodOfvalidity: ""
        }
    }

    Component {
        id: listDelegate
        Item {
            id: delegateItem
            width: listView.width
            height: 50
            clip: true
            Row {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                spacing: 5
                Image {
                    id: image
                    width: 40
                    height: 40
                    source: imagePath
                }

                TextField {
                    id: deviceNameTextFild
                    width: 160
                    height: 40
                    text: deviceName

                    onTextChanged: {
                        deviceName = deviceNameTextFild.text
                    }
                }

                TextField {
                    id: manufacturersTextField
                    width: 160
                    height: 40
                    text: manufacturers

                    onTextChanged: {
                        manufacturers = manufacturersTextField.text
                    }
                }

                TextField {
                    id: periodTextField
                    width: 160
                    height: 40
                    text: periodOfvalidity
                    placeholderText: qsTr("失效时间(如:2050/01/01)")
                    onTextChanged: {
                        periodOfvalidity = periodTextField.text
                    }
                }

                TextField {
                    id: imagePathTextField
                    width: 160
                    height: 40
                    text: imagePath

                    onTextChanged: {
                        imagePath = imagePathTextField.text
                    }
                }

                Button {
                    id: imageSettingBtn
                    width: 80
                    text: qsTr("选择图标")
                    onClicked: {
                        fileDialog.open()
                    }
                }

                Image {
                    id: deleteImage
                    width: 30
                    height: 30
                    source: "qrc:/images/delete.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            listModel.remove(index)
                            itemIconInfo.removeIconInfo(String("%1").arg(index))
                            itemIconInfo.clearIconInfo()
                            saveInfo()
                        }
                    }
                }
            }

            FileDialog {
                id: fileDialog
                title: "Please choose a file"
                folder: StandardPaths.writableLocation(
                            StandardPaths.DocumentsLocation)
                onAccepted: {
                    imagePath = Qt.resolvedUrl(decodeURI(
                                                   currentFile.toString()))
                }
            }
        }
    }

    Row {
        id: titleRow
        anchors.top: parent.top
        anchors.margins: 10
        spacing: 5
        Text {
            id: iconTxt
            width: 40
            // height: 40
            text: qsTr("图标")
            font.family: "Times New Roman"
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        Text {
            id: deviceNameTxt
            font.family: "Times New Roman"
            font.pixelSize: 14
            // height: 40
            width: 150
            text: qsTr("设备")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        Text {
            id: manufacturersTxt
            width: 160
            // height: 40
            text: qsTr("制造商")
            font.family: "Times New Roman"
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: periodTxt
            width: 150
            // height: 40
            text: qsTr("有效期")
            font.family: "Times New Roman"
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: pathTxt
            font.family: "Times New Roman"
            font.pixelSize: 14
            width: 150
            text: qsTr("图标路径")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

    ListView {
        id: listView
        anchors.top: titleRow.bottom
        anchors.left: parent.left
        anchors.bottom: buttons.top
        anchors.right: parent.right
        //anchors.margins: 10
        model: listModel
        delegate: listDelegate
    }
    Row {
        id: buttons
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: 40
        anchors.margins: 10
        spacing: 20
        Button {
            id: addItemBtn
            text: qsTr("增加项目")
            onClicked: {

                var obj = new Object
                obj["imagePath"] = Qt.resolvedUrl(
                            decodeURI("qrc:/images/fireAlarm.png"))
                obj["deviceName"] = String(qsTr("报警装置%1").arg(listModel.count))
                obj["manufacturers"] = qsTr("北京利达华信电子有限公司")
                obj["periodOfvalidity"] = ""
                listModel.append(obj)
                saveInfo()
            }
        }

        Button {
            id: saveBtn
            text: qsTr("保存")
            onClicked: {
                saveInfo()
            }
        }

        Button {
            id: clearBtn
            text: qsTr("清空")
            onClicked: {
                listModel.clear()
                itemIconInfo.clearIconInfo()
                itemIconInfo.setCurrentIconIndex(-1)
                saveInfo()
            }
        }
    }

    ItemIconInfoToJson {
        id: itemIconInfo
    }

    function saveInfo() {

        for (var i = 0; i < listModel.count; i++) {
            var obj = new Object
            obj = listModel.get(i)

            itemIconInfo.saveItemIconInfo(String("%1").arg(i), "deviceName",
                                          obj["deviceName"].toString())
            itemIconInfo.saveItemIconInfo(String("%1").arg(i), "imagePath",
                                          obj["imagePath"].toString())

            itemIconInfo.saveItemIconInfo(String("%1").arg(i), "manufacturers",
                                          obj["manufacturers"].toString())
            itemIconInfo.saveItemIconInfo(String("%1").arg(i),
                                          "periodOfvalidity",
                                          obj["periodOfvalidity"].toString())
        }
        itemIconInfo.itemIconInfoToJson()
        emit: saveItemInfoToJson()
    }
    function readInfo() {

        var size = itemIconInfo.sizeOfHash()
        if (size === 0) {
            listModel.clear()
            return
        }
        var itemIconInfoStr = new String
        itemIconInfoStr = itemIconInfo.readFileFromJson()
        if (itemIconInfoStr.length > 0) {
            if (listModel.count > 0) {
                if (size >= listModel.count) {
                    for (var currentIndex = 0; currentIndex < listModel.count; currentIndex++) {
                        var currentObj = new Object
                        currentObj = JSON.parse(itemIconInfoStr)[currentIndex]

                        listModel.set(currentIndex, currentObj)
                    }

                    for (var i = listModel.count; i < size; i++) {
                        var index = String("%1").arg(i)
                        var obj = new Object
                        obj = JSON.parse(itemIconInfoStr)[index]
                        listModel.append(obj)
                    }
                }
            } else {
                for (var j = 0; j < size; j++) {
                    var index2 = String("%1").arg(j)
                    var obj2 = new Object
                    obj = JSON.parse(itemIconInfoStr)[index2]
                    listModel.append(obj2)
                }
            }
        }
    }
    Component.onDestruction: {
        saveInfo()
    }

    Component.onCompleted: {
        readInfo()
    }
}
