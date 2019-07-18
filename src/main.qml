import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import OpenCascade 7.3

ApplicationWindow {
	id: root_window
    visible: true
    width: 640
    height: 480   

	Item {
		id: root_item
		anchors.fill: parent

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
}
