import QtQuick 2.9
import QtQuick.Controls 2.2
import Qt.labs.platform 1.0
import itemIconInfoToJson 1.0

Rectangle {
    width: 640
    height: 240

    signal saveItemInfoToJson()
    ListModel {
        id: listModel
        ListElement {
            deviceName: qsTr("报警装置")
            imagePath: "qrc:/images/fireAlarm.png"
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
                Text {
                    id: deviceNameTxt
                    height: 40
                    text: qsTr("设备类型")
                    verticalAlignment: Text.AlignVCenter
                }

                TextField {
                    id: deviceNameTextFild
                    width: 150
                    height: 40
                    text: deviceName

                    onTextChanged: {
                        deviceName = deviceNameTextFild.text
                    }
                }
                Text {
                    id: pathTxt
                    height: 40
                    text: qsTr("图标路径")
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: imagePathTextField
                    width: 150
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
                            //var indexStr = new String
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

    ListView {
        id: listView
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: buttons.top
        anchors.right: parent.right
        anchors.margins: 10
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
        if (listModel.count === 0) {
            listModel.append({
                                 deviceName: qsTr("报警装置"),
                                 imagePath: "qrc:/images/fireAlarm.png"
                             })
        }
    }
}
