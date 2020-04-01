import QtQuick 2.0

import QtQuick.Controls 2.2
Item {

    height:300
    width: 240


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

    function addHandOrAutoState(extNum,networkNum,curState)
    {
        var curHandOrAutoState= new String
             curHandOrAutoState=   String(qsTr("主机号:%1,网络号:%2,%3")).arg(extNum).arg(networkNum).arg(curState)
        if(listModel.count>0)
        {
            var isOk = false
            var curIndex =0
            for(var i=0;i<listModel.count;i++)
            {
                var curValue = new String
                curValue = listModel.get(i)["value"]
                var curStateValue=  curValue.substring(0,curValue.length-3)
                var currentHandOrAuto = curHandOrAutoState.substring(0,curValue.length-3)
                if(curStateValue===currentHandOrAuto)
                {
                    isOk = true
                    curIndex = i
                }
            }
            if(isOk)
            {

                listModel.setProperty(curIndex,"value",curHandOrAutoState)

            }
            else
            {
                listModel.append({"value":curHandOrAutoState})

            }
        }
        else
        {
            listModel.append({"value":curHandOrAutoState})

        }
    }
    function clearHandOrAutoState()
    {
        listModel.clear()
    }

    function deleteHandOrAutoState(extNum)
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
    function setHandOrAutoState(index,curState)
    {
      listModel.setProperty(index,"value",curState)
    }

}
