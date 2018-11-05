import QtQuick 2.9
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Rectangle
{
    width: 150
    Rectangle
    {
        id:alarmBtnRec
        height: 100;
        width: parent.width;
        RowLayout
        {
            anchors.fill: parent;
            Button
            {
                Layout.alignment: Qt.AlignLeft;
                text: qsTr("首火警")
                Layout.fillWidth: true;
                onClicked:
                {
                  CrtWidget.toFirstFireAlarm();
                }
            }

            Button
            {
                Layout.alignment: Qt.AlignRight;
                text: qsTr("末火警");
                Layout.fillWidth: true;
                onClicked:
                {
                   CrtWidget.toLastFireAlarm();
                }
            }

        }

    }
    GridLayout
    {
        id:alarmIndicatorLayout
        anchors.top: alarmBtnRec.bottom;
        anchors.horizontalCenter: parent.horizontalCenter;
        Layout.fillWidth: true
        StatusIndicator
        {
            id:fireAlarmStatusIndicator;
            Layout.column: 0;
            Layout.row: 0;
            color:"green";
            active: true;

            ColorAnimation on color
            {
                from: "red"
                to: "black"
                duration: 1000;
                loops: Animation.Infinite;
                running: false;
            }
        }
        Text
        {
            Layout.column: 1;
            Layout.row: 0;
            text: qsTr("火警 0")

        }

        StatusIndicator
        {
            id:linkageStatusIndicator;
            Layout.column: 0;
            Layout.row: 1;
            color: "green";
            active: true;
            ColorAnimation on color
            {
                from: "red"
                to: "black"
                duration: 1000;
                loops: Animation.Infinite;
                running: false;
            }

        }
        Text
        {
            Layout.column: 1;
            Layout.row: 1;
            text: qsTr("联动 0")
        }

        StatusIndicator
        {
            id:superviseStatusIndicator;
            Layout.column: 0;
            Layout.row: 2;
            color: "green";
            active: true;
            ColorAnimation on color
            {
                from: "red"
                to: "black"
                duration: 1000;
                loops: Animation.Infinite;
                running: false;
            }
        }
        Text
        {
            Layout.column: 1;
            Layout.row: 2;
            text: qsTr("监管 0")
        }

        StatusIndicator
        {
            id:faultStatusIndicator;
            Layout.column:0;
            Layout.row: 3;
            color: "green";
            active: true;
            ColorAnimation on color
            {
                from: "red"
                to: "black"
                duration: 1000;
                loops: Animation.Infinite;
                running: false;
            }
        }
        Text
        {
            Layout.column: 1;
            Layout.row: 3;
            text: qsTr("故障 0")
        }

        StatusIndicator
        {
            id:feedbackStatusIndicator;
            Layout.column:0;
            Layout.row: 4;
            color: "green";
            active: true;
            ColorAnimation on color
            {

                from: "red"
                to: "black"
                duration: 1000;
                loops: Animation.Infinite;
                running: false;
            }
        }
        Text
        {
            Layout.column: 1;
            Layout.row: 4;
            text: qsTr("反馈 0")
        }

        StatusIndicator
        {
            id:shieldStatusIndicator;
            Layout.column:0;
            Layout.row: 5;
            color: "green";
            active: true;
            ColorAnimation on color
            {
                from: "red"
                to: "black"
                duration: 1000;
                loops: Animation.Infinite;
                running: false;
            }
        }
        Text
        {
            Layout.column: 1;
            Layout.row: 5;
            text: qsTr("屏蔽 0")
        }


        StatusIndicator
        {
            Layout.column:0;
            Layout.row: 6;
            color: "green";
            active: true;
            ColorAnimation on color
            {
                from: "red"
                to: "black"
                duration: 1000;
                loops: Animation.Infinite;
                running: false;
            }
        }
        Text
        {
            Layout.column: 1;
            Layout.row: 6;
            text: qsTr("主机通信")
        }

    }


    Column
    {

        anchors.top: alarmIndicatorLayout.bottom;

        width: parent.width-40;
        spacing: 5;
        anchors.left: parent.left;
        anchors.leftMargin: 20;
        anchors.rightMargin: 20;
        anchors.verticalCenter: parent.verticalCenter;

        Repeater
        {
            id:repeater;
            Layout.fillHeight: true;

            model: ListModel
            {
                ListElement
                {
                    name: qsTr("消音");

                }
                ListElement
                {
                    name: qsTr("复位")

                }
                ListElement
                {
                    name: qsTr("模拟火警")

                }

            }

            Button
            {

                text: name;
                anchors.leftMargin: 20;
                width: parent.width;
                //height: 30;
//                background: Rectangle
//                {
//                   border.width:1;
//                   border.color:"green"
//                   color:"blue";
//                }
               // down: true;


            }

//            Rectangle
//            {
//                id:rec;

//                anchors.leftMargin: 20;
//                width: parent.width;
//                height: 30;
//                radius: 5;
//                border.color: "blue";
//                border.width: 1;
//                color: "green"
//                Text
//                {

//                    text: name+number;
//                }
//                MouseArea
//                {
//                    anchors.fill: parent;
//                    onClicked:
//                    {

//                        var count = repeater.count;
//                        for(var i=0;i<count;i++)
//                        {
//                            if(repeater.itemAt(i)===rec)
//                            {
//                                console.log(i);
//                            }
//                        }
//                    }

//                }

//            }
        }


    }

}
