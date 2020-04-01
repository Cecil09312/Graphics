import QtQuick 2.0
import QtQuick.Controls 2.2
Item {
    height:300
    width: 400


    ListModel
    {
        id:listModel

    }

    Component {
        id: listDelegate
        Text {
            text: value
        }
    }
    ScrollView
    {
        anchors.fill: parent
        anchors.leftMargin: 5
        ListView
        {

            anchors.fill: parent
            model: listModel
            delegate: listDelegate
            clip: true

        }
    }

    function addMainPowerState(extNum,networkNum,powerAddress,curState)
    {
        var curMainPowerState = new String
        if(powerAddress=="")
        {
          curMainPowerState = String(qsTr("主机号:%1,网络号:%2,%3")).arg(extNum).arg(networkNum).arg(curState)
        }
        else
        {
           curMainPowerState = String(qsTr("主机号:%1,网络号:%2,电源地址:%3,%4")).arg(extNum).arg(networkNum).arg(powerAddress).arg(curState)
        }
        if(listModel.count>0)
        {
            var isOk = false
            var curIndex =0
            for(var i=0;i<listModel.count;i++)
            {
                var curValue = new String
                curValue = listModel.get(i)["value"]

                var curStateValue =new String


                  curStateValue =  curValue.substring(0,curValue.length-5)
                   console.log(curStateValue)
                  var currentMainPowerState = curMainPowerState.substring(0,curValue.length-5)
                if(curStateValue===currentMainPowerState)
                {
                    isOk = true
                    curIndex = i
                }
            }
            if(isOk)
            {

                listModel.setProperty(curIndex,"value",curMainPowerState)

            }
            else
            {
                listModel.append({"value":curMainPowerState})

            }
        }
        else
        {
            listModel.append({"value":curMainPowerState})

        }
    }
    function clearMainPowerState()
    {
        listModel.clear()
    }
    function deleteMainPowerState(extNum)
    {
        for(var i=0;i<listModel.count;i++)
        {
            var curValue = new String
            curValue = listModel.get(i)["value"]
            var curStateValue=  curValue.substring(0,5)
            if(curStateValue===String(qsTr("主机号:%1")).arg(extNum))
            {
                 listModel.remove(i)

            }
        }

    }
    function setMainPowerState(index,curState)
    {
        listModel.setProperty(index,"value",curState)
    }



}


