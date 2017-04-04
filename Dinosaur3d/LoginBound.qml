import QtQuick 2.4
import QtGraphicalEffects 1.0

Item
{
    id: loginBoundItem

    property int x_mou: 0
    property int y_mou: 0
    property bool isMax: false

    //bound image
    Image
    {
        id: boundImage
        source: "qrc:/image/image/login/timg.jpg"
        anchors.centerIn: parent
        width: parent.width-30
        height: parent.height-30
        smooth: true

        //Move Title
        Rectangle
        {
            id: titleRec1
            width: parent.width-70
            height: 25
            color: "transparent"
            x: 0; y:0
        }
        Rectangle
        {
            id: titleRec2
            width: parent.width
            height: 55
            color: "transparent"
            anchors
            {
                left: titleRec1.left
                top: titleRec1.bottom
            }
        }
        MouseArea
        {
            anchors.fill: titleRec1
            hoverEnabled: true
            onPressed:
            {
                x_mou = mouseX
                y_mou = mouseY
            }
            onPositionChanged:
            {
                if(x_mou!=0 && y_mou!=0)
                {
                    mainApp.setWindowPosition(mainApp.getWindowX() + ( mouseX - x_mou), mainApp.getWindowY() + ( mouseY - y_mou));
                }
            }
            onReleased:
            {
                x_mou = 0;
                y_mou = 0;
            }
        }
        MouseArea
        {
            anchors.fill: titleRec2
            hoverEnabled: true
            onPressed:
            {
                x_mou = mouseX
                y_mou = mouseY
            }
            onPositionChanged:
            {
                if(x_mou!=0 && y_mou!=0)
                {
                    mainApp.setWindowPosition(mainApp.getWindowX() + ( mouseX - x_mou), mainApp.getWindowY() + ( mouseY - y_mou));
                }
            }
            onReleased:
            {
                x_mou = 0;
                y_mou = 0;
            }
        }

        //Min Icon
        Image
        {
            id: minImage
            source: "qrc:/image/image/login/min.png"
            sourceSize.width: 15
            sourceSize.height: 15
            opacity: minArea.containsMouse ? 1 : 0.5
            anchors
            {
                left: titleRec1.right
                leftMargin: 5
                top: titleRec1.top
                topMargin: 10
            }
            MouseArea
            {
                id: minArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: mainApp.setWindowMin();
            }
        }

        //Max Icon
        Image
        {
            id: maxImage
            source: "qrc:/image/image/login/max.png"
            sourceSize.width: 15
            sourceSize.height: 15
            opacity: maxArea.containsMouse ? 1 : 0.5
            anchors
            {
                left: minImage.right
                leftMargin: 5
                top: titleRec1.top
                topMargin: 10
            }
            MouseArea
            {
                id: maxArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked:
                {
                    mainApp.setWindowMax(loginBoundItem.isMax)
                    loginBoundItem.isMax = !loginBoundItem.isMax;
                }
            }
        }

        //Close Icon
        Image
        {
            id: closeImage
            source: "qrc:/image/image/login/close.png"
            sourceSize.width: 15
            sourceSize.height: 15
            opacity: closeArea.containsMouse ? 1 : 0.5
            anchors
            {
                left: maxImage.right
                leftMargin: 5
                top: titleRec1.top
                topMargin: 10
            }
            MouseArea
            {
                id: closeArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked:
                {
                    //mainApp.setWindowClose();
                    mainApp.setWindowTray();
                }
            }
        }

        //Net Text
        Text
        {
            id: netText
            text: "登陆异常?点此检测"
            color: textArea.containsMouse ? "LightSteelBlue":"white"
            font
            {
                family: "微软雅黑"
                pointSize: 9
                underline: textArea.pressed ? true : false
            }
            anchors
            {
                horizontalCenter: boundImage.horizontalCenter
                bottom: boundImage.bottom
                bottomMargin: 8
            }
            MouseArea
            {
                id: textArea
                anchors.fill: parent
                hoverEnabled: true
            }
        }
    }

    //Shadow
    DropShadow
    {
        anchors.fill: boundImage
        horizontalOffset: 6
        verticalOffset: 6
        radius: 12.0
        samples: 17
        color: "#80000000"
        source: boundImage
    }
}
