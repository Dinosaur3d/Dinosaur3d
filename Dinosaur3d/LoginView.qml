import QtQuick 2.4
import QtGraphicalEffects 1.0
import Qt.labs.controls 1.0

Item
{
    id: loginViewItem
    width: 380
    height: 350

    //bound Image
    Image
    {
        id: skyImage
        source: "qrc:/image/image/login/sky.png"
        sourceSize: Qt.size(parent.width, parent.height)
        anchors.centerIn: parent
        opacity: 0.8
        smooth: true
    }

    //Two Button
    Item
    {
        id: twoButtonItem
        width: parent.width
        height: 50
        anchors.left: parent.left
        anchors.top: parent.top
        Text
        {
            z: 0.2
            text: "扫码登陆"
            color: leftButton.visible ? "gray" : "black"
            font
            {
                family: "微软雅黑 Light"
                pointSize: 11
            }
            anchors.centerIn: leftButton
        }
        Text
        {
            z: 0.1
            text: "密码登陆"
            color: rightButton.visible ? "gray" : "black"
            font
            {
                family: "微软雅黑 Light"
                pointSize: 11
            }
            anchors.centerIn: rightButton
        }
        Image
        {
            id: leftButton
            z: 0.1
            source: "qrc:/image/image/login/button_01.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            anchors.left: parent.left
            anchors.top: parent.top
            visible: true
            MouseArea
            {
                id: leftButtonArea
                anchors.fill: parent
                onClicked:
                {
                    if( leftButton.visible === true &&  rightButton.visible === false)
                    {
                        leftButton.visible = !leftButton.visible;
                        rightButton.visible = !rightButton.visible;

                        headImage.visible = false;
                        loginButton.visible = false;
                        psw.visible = false;
                        user.visible = false;

                        qrCodeIamge.visible = true;
                        codeText.visible = true;
                    }
                }
            }
        }

        Image
        {
            id: rightButton
            source: "qrc:/image/image/login/button_01.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            anchors.right: parent.right
            anchors.top: parent.top
            visible: false
            MouseArea
            {
                id: rightButtonArea
                anchors.fill: parent
                onClicked:
                {
                    if( leftButton.visible === false &&  rightButton.visible === true)
                    {
                        leftButton.visible = !leftButton.visible;
                        rightButton.visible = !rightButton.visible;

                        headImage.visible = true;
                        loginButton.visible = true;
                        psw.visible = true;
                        user.visible = true;

                        qrCodeIamge.visible = false;
                        codeText.visible = false;
                    }
                }
            }
        }
    }

    //User Input
    Item
    {
        id: user
        z: 0.2
        width: 320
        height: 42
        anchors.centerIn: parent

        Rectangle
        {
            anchors.fill: parent
            color: "#99FFFFFF"
            border.width: 1
            border.color: "LightSteelBlue"
            radius: 3
        }

        Image
        {
            id: userImage
            width: parent.height/3
            height: parent.height/3
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: parent.height/2
            source: "qrc:/image/image/login/user.png"
            smooth: true
        }

        Text
        {
            text: "/"
            font.pointSize: 11
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: userImage.right
            anchors.leftMargin: userImage.height/2
        }

        Item
        {
            id: userItemInput
            clip: true
            width: parent.width-parent.height
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: parent.height*1.2

            TextInput
            {
                id: userTextInput
                width: parent.width-10
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                maximumLength: 30
                font.family: "微软雅黑 Light"
                font.pointSize: 11
            }
        }
    }

    //Psw Input
    Item
    {
        id: psw
        z: 0.2
        width: 320
        height: 42
        anchors
        {
            left: user.left
            top: user.bottom
            topMargin: 5
        }

        Rectangle
        {
            anchors.fill: parent
            color: "#99FFFFFF"
            border.width: 1
            border.color: "LightSteelBlue"
            radius: 3
        }

        Image
        {
            id: pswImage
            width: parent.height/3
            height: parent.height/3
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: parent.height/2
            source: "qrc:/image/image/login/psw.png"
            smooth: true
        }

        Text
        {
            text: "/"
            font.pointSize: 11
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: pswImage.right
            anchors.leftMargin: pswImage.height/2
        }

        Item
        {
            clip: true
            id: pswItemInput
            width: parent.width-parent.height
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: parent.height*1.2

            TextInput
            {
                id: pswTextInput
                width: parent.width-10
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                maximumLength: 23
                font.family: "微软雅黑 Light"
                font.pointSize: 11
                echoMode: TextInput.Password
            }
        }
    }

    //Login Button
    Rectangle
    {
        id: loginButton
        z: 0.2
        width: 320
        height: 42
        anchors.left: psw.left
        anchors.top: psw.bottom
        anchors.topMargin: 20
        radius: 3
        color:
        {
            if( userTextInput.text != "" && pswTextInput.text != "")
            {
                if( loginArea.containsPress )
                    return "#1a86d2"
                if( loginArea.containsMouse )
                    return "#46b2fe"
                return "#008cee";
            }
            else
            {
                return "#c7c7cc"
            }
        }

        MouseArea {
            id: loginArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked:
            {
                console.log(userTextInput.text)
                console.log(pswTextInput.text)
                //TODO
            }
        }

        Text {
            text: qsTr("登        陆")
            color: "white"
            font.family: "微软雅黑"
            font.pointSize: 11
            anchors.centerIn: parent
        }
    }

    //Head Image
    Item
    {
        id: headImage
        z: 0.2
        width: 60
        height: 60
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: user.top
        anchors.bottomMargin: 20

        Rectangle
        {
            id: nRound
            anchors.centerIn: headImage
            width: 66
            height: 66
            radius: width/2
            color: "white"
        }
        Rectangle
        {
            id: wRound
            anchors.centerIn: parent
            width: 64
            height: 64
            radius: width/2
            color: "#FF00AA99"
            border.width: 1
            border.color: "white"
        }

        Image
        {
            id: bug
            source: "qrc:/image/image/login/head04.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
            anchors.centerIn: parent
        }

        Image
        {
            id: mask
            source: "qrc:/image/image/login/dcvse.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
            anchors.centerIn: parent
        }

        OpacityMask
        {
            anchors.fill: bug
            source: bug
            maskSource: mask
        }
    }

    //Auto Login
    //TODO
    //New User
    //TODO

    //RCode
    Image
    {
        id: qrCodeIamge
        visible: false
        source: "qrc:/image/image/login/QRcode.jpg"
        sourceSize: Qt.size(200,200)
        anchors.centerIn: parent
    }
    Text
    {
        id: codeText
        visible: false
        text: "扫描二维码联系作者微信"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: qrCodeIamge.bottom
        anchors.topMargin: 10
        font.family: "微软雅黑"
        font.pointSize: 11
        color: "gray"
    }
}
