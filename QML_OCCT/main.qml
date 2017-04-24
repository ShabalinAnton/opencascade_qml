import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.2
import OCCTUnderQML 1.0

Item {
    visible: true
    width: 640
    height: 480   

    OCCTView {
        id: occtview
    }

    MouseArea {
         acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MidButton | Qt.WheelFocus
         //acceptedButtons: Qt.AllButtons
         anchors.fill: parent
         hoverEnabled: true
         onPositionChanged: occtview.mouseMove(mouse.x, mouse.y)
         onWheel: {
             occtview.onWheel(wheel.angleDelta.y)
         }

         onPressed: {
             if (mouse.button & Qt.RightButton) {
                 occtview.onRightButtonPressed(mouse.x, mouse.y)
             }
             if (mouse.button & Qt.MidButton) {
                 occtview.onMiddleButtonPressed(mouse.x, mouse.y)
             }
         }
         onReleased: {
             if (mouse.button & Qt.RightButton) {
                 occtview.onRightButtonReleased(mouse.x, mouse.y)
             }
             if (mouse.button & Qt.MidButton) {
                 occtview.onMiddleButtonReleased(mouse.x, mouse.y)
             }
         }
     }


    Rectangle {
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 10
        border.width: 1
        border.color: "white"
        anchors.fill: label
        anchors.margins: -10
    }

    Text {
        id: label
        color: "black"
        wrapMode: Text.WordWrap
        text: "The background here is a OpenCASCADE rendered with raw OpenGL using the 'beforeRender()' signal in QQuickWindow. This text label and its border is rendered using QML"
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
    }   
}
