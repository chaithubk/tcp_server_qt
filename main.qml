import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0

import Tcp 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    minimumWidth: layout.implicitWidth
    minimumHeight: layout.implicitHeight

    title: qsTr("TCP Server")

    ColumnLayout {
        id: layout

        anchors.centerIn: parent
        Layout.minimumHeight: 150
        Layout.minimumWidth: 200

        spacing: 20

        Text {
            height: 40
            text: "Local IP: " + TCPServer.localIP
            color: "black"
            font.pixelSize: 25
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        Text {
            height: 40
            text: TCPServer.serverStatus
            color: "black"
            font.pixelSize: 25
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        Text {
            height: 40
            text: TCPServer.connectionStatus
            color: "black"
            font.pixelSize: 25
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        Text {
            height: 40
            text: TCPServer.writeStatus
            color: "black"
            font.pixelSize: 25
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }
    }

}
