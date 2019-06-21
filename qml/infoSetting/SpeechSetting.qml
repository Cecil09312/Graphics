import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import speechObj 1.0
Item {

    ListModel {
        id: voiceNameModel
    }
    Grid {
        columns: 2
        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }
        columnSpacing: 5
        Text {
            text: qsTr("音量")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }

        Slider {
            id: volumeSlider
            from: 0
            to: 1.0
            stepSize: 0.1
            width: 300
            ToolTip {
                parent: volumeSlider.handle
                visible: volumeSlider.pressed | volumeSlider.hovered
                text: volumeSlider.value.toFixed(1)
            }

            onMoved: {
                SpeechObj.volume = value
            }
        }


        Text {
            text: qsTr("频率")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }

        Slider {
            id: rateSlider
            from: -1.0
            to: 1.0
            stepSize: 0.1
            width: 300
            ToolTip {
                parent: rateSlider.handle
                visible: rateSlider.pressed | rateSlider.hovered
                text: rateSlider.value.toFixed(1)
            }

            onMoved: {

                SpeechObj.rate = value
            }
        }

        Text {
            text: qsTr("音调")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }

        Slider {
            id: pitchSlider
            from: -1.0
            to: 1.0
            stepSize: 0.1

            width: 300
            ToolTip {
                parent: pitchSlider.handle
                visible: pitchSlider.pressed | pitchSlider.hovered
                text: pitchSlider.value.toFixed(1)
            }
            onMoved: {
                SpeechObj.pitch = value
            }
        }

    }

    Component.onCompleted: {

        volumeSlider.value = SpeechObj.volume
        rateSlider.value = SpeechObj.rate
        pitchSlider.value = SpeechObj.pitch
    }
}
