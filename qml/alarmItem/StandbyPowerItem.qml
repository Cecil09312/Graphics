import QtQuick 2.0
import QtQuick.Controls 2.2
Item {

    height:300
    width: 500


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

    function addStandbyPowerState(extNum,networkNum,powerAddr,curState)
    {
        var curStandbyPowerState= new String
        if(powerAddr=="")
        {
        curStandbyPowerState=String(qsTr("主机号:%1,网络号:%2,%3")).arg(extNum).arg(networkNum).arg(curState)
        }
        else
        {
        curStandbyPowerState=String(qsTr("主机号:%1,网络号:%2,电源地址:%3,%4")).arg(extNum).arg(networkNum).arg(powerAddr).arg(curState)
        }
        if(listModel.count>0)
        {
            var isOk = false
            var curIndex =0
            for(var i=0;i<listModel.count;i++)
            {
                var curValue = new String
                curValue = listModel.get(i)["value"]
                var curStateValue=  curValue.substring(0,curValue.length-5)
                var currentStandbyPowerState = curStandbyPowerState.substring(0,curValue.length-5)
                if(curStateValue===currentStandbyPowerState)
                {
                    isOk = true
                    curIndex = i
                }
            }
            if(isOk)
            {

                listModel.setProperty(curIndex,"value",curStandbyPowerState)

            }
            else
            {
                listModel.append({"value":curStandbyPowerState})

            }
        }
        else
        {
            listModel.append({"value":curStandbyPowerState})

        }
    }
    function clearStandbyPowerState()
    {
        listModel.clear()
    }

    function deleteStandbyPowerState(extNum,network)
    {
        for(var i=0;i<listModel.count;i++)
        {
            var curValue = new String
            curValue = listModel.get(i)["value"]

            if(curValue.match(String(qsTr("主机号:%1,网络号:%2")).arg(extNum).arg(network))!=null)
            {
                 listModel.remove(i)

            }
        }

    }
    function setStandbyPowerState(index,curState)
    {
      listModel.setProperty(index,"value",curState)
    }

}
