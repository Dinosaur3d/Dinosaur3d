import QtQuick 2.4

Item
{
    id: item

    LoginBound
    {
        id: loginBound
        anchors.fill: parent
    }

    LoginView
    {
        id: loginView
        anchors.centerIn: parent
    }
}
