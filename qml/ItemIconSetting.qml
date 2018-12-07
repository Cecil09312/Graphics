import QtQuick 2.9
import QtQuick.Controls 2.4
import Qt.labs.platform 1.0
import itemIconInfoToJson 1.0

Rectangle {
    width: 640
    height: 240
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
                            listModel.remove(index)
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
        anchors.margins: 10
        spacing: 20
        Button {
            id: addItemBtn
            text: qsTr("增加项目")
            onClicked: {
                listModel.append({
                                     imagePath: "qrc:/images/fireAlarm.png",
                                     deviceName: qsTr("报警装置")
                                 })
            }
        }

        Button {
            id: clearBtn
            text: qsTr("清空")
            onClicked: {
                listModel.clear()
            }
        }
    }

    ItemIconInfoToJson {
        id: itemIconInfo
    }

    function saveInfo() {

        console.log(listModel.count)
        for (var i = 0; i < listModel.count; i++) {
            var obj = new Object
            obj = listModel.get(i)

            itemIconInfo.saveItemIconInfo(String("%1").arg(i), "deviceName",
                                          obj["deviceName"].toString())
            itemIconInfo.saveItemIconInfo(String("%1").arg(i), "imagePath",
                                          obj["imagePath"].toString())
        }
        itemIconInfo.itemIconInfoToJson()
    }
    function readInfo() {
        if (listModel.count > 0) {
            listModel.clear()
        }
        var size = itemIconInfo.sizeOfHash()
        var itemIconInfoStr = itemIconInfo.readFileFromJson()
        for (var i = 0; i < size; i++) {
            var index = String("%1").arg(i)
            var obj = JSON.parse(itemIconInfoStr)[index]
            listModel.append(obj)
        }
    }
    Component.onDestruction: {
        saveInfo()
    }

    Component.onCompleted: {
        readInfo()
    }
}
