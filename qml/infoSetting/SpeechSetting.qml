import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as Controls1_4
import speechObj 1.0
import controller 1.0
Item {

    width: 800
    height: 640
    ListModel {
        id: engineModel
    }

    ListModel {
        id: languageModel
    }
    Grid {
        columns: 2
        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }
        columnSpacing: 5
        rowSpacing: 5
        Text {
            id:driveTxt
            text: qsTr("驱动")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }

        ComboBox
        {

            id:engineComboBox
            width: 300
            model: engineModel
            onCurrentTextChanged:
            {
                SpeechObj.engineSelected(currentText)
            }
        }

        Text {
            id:languageTxt
            text: qsTr("语言")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }

        ComboBox
        {
            id:languageComboBox
            width: 300
            model: languageModel
            onCurrentTextChanged:
            {
                SpeechObj.setLanguage(currentText)
            }
        }

        Text {
            id:volumeTxt
            text: qsTr("音量")
            height: 40
            verticalAlignment: Text.AlignVCenter
        }

        Slider {
            id: volumeSlider
            from: SpeechObj.volumeMin()
            to: SpeechObj.volumeMax()
            stepSize: SpeechObj.volumeStep()
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
            id:frequencyTxt
            text: qsTr("频率")
            height: 40
            verticalAlignment: Text.AlignVCenter
            visible: !Controller.sysOnLinux()||(Controller.sysOnLinux()&&SpeechObj.isEnglish())
        }

        Slider {
            id: rateSlider
            from: SpeechObj.rateMin()
            to: SpeechObj.rateMax()
            stepSize: SpeechObj.rateStep()
            width: 300
            visible: !Controller.sysOnLinux()||(Controller.sysOnLinux()&&SpeechObj.isEnglish())
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
            id:toneTxt
            text: qsTr("音调")
            height: 40
            verticalAlignment: Text.AlignVCenter
            visible: !Controller.sysOnLinux()
        }

        Slider {
            id: pitchSlider
            from: SpeechObj.pitchMin()
            to: SpeechObj.pitchMax()
            stepSize: SpeechObj.pitchStep()

            width: 300
            visible: !Controller.sysOnLinux()
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

        engineModel.append({value:"default"})
        for(var i=0;i<SpeechObj.engineNameNum();i++)
        {
            engineModel.append({value:SpeechObj.engineName(i)})
        }

        languageModel.clear()
        if(!Controller.sysOnLinux())
        {

            for(var j=0;j<SpeechObj.languageNum();j++)
            {
                languageModel.append({value:SpeechObj.languageName(j)})
            }
        }
        else
        {
            if(!SpeechObj.isEnglish())
            {
                languageModel.append({value:"中文"})

            }
            else
            {
               languageModel.append({value:"English"})

            }
        }

        if(languageComboBox.count>0)
        {
            languageComboBox.currentIndex =0;
        }

        if(engineComboBox.count>0)
        {
            engineComboBox.currentIndex =0;
        }

    }

    function retranslate()
    {
        driveTxt.text = qsTr("驱动")
        languageTxt.text = qsTr("语言")
        volumeTxt.text = qsTr("音量")
        frequencyTxt.text = qsTr("频率")
        toneTxt.text = qsTr("音调")

    }

    function reinitSpeech()
    {

        volumeSlider.from = SpeechObj.volumeMin()
        volumeSlider.to=SpeechObj.volumeMax()
        volumeSlider.stepSize=SpeechObj.volumeStep()
        SpeechObj.volume = volumeSlider.value
        //console.log(volumeSlider.value)
        rateSlider.from=SpeechObj.rateMin()
        rateSlider.to= SpeechObj.rateMax()
        rateSlider.stepSize= SpeechObj.rateStep()
        SpeechObj.rate = rateSlider.value
        // console.log(rateSlider.value)
        pitchSlider.from=SpeechObj.pitchMin()
        pitchSlider.to= SpeechObj.pitchMax()
        pitchSlider.stepSize=SpeechObj.pitchStep()
        SpeechObj.pitch = pitchSlider.value
        //console.log(pitchSlider.value)

        languageModel.clear();
        if(!Controller.sysOnLinux())
        {
            for(var j=0;j<SpeechObj.languageNum();j++)
            {
                languageModel.append({value:SpeechObj.languageName(j)})
            }

        }
        else
        {
            if(!SpeechObj.isEnglish())
            {
                languageModel.append({value:"中文"})
                languageComboBox.model = languageModel
                frequencyTxt.visible = false;
                rateSlider.visible=false;
                pitchSlider.visible=false
                toneTxt.visible =false
            }
            else
            {

                languageModel.append({value:"English"})


                frequencyTxt.visible = true
                rateSlider.visible=true
                pitchSlider.visible=true
                toneTxt.visible =true

            }
        }


        if(languageComboBox.count>0)
        {
            languageComboBox.currentIndex =0;
        }

    }


    //    Connections
    //    {
    //        target: SpeechObj
    //        onLanguageChangeToEnglish:
    //        {

    //            //SpeechObj.setLanguage(languageComboBox.currentText)
    //        }

    //    }
}
