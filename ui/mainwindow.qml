import QtQuick 2.0
import QtQuick.Controls 2.13

Item {
    Button {
        id: button
        x: 208
        y: 179
        width: 140
        height: 122
        text: qsTr("Button")
        focusPolicy: Qt.WheelFocus
        checked: false
        display: AbstractButton.TextBesideIcon
        font.family: "Arial"
        font.pointSize: 19
        scale: 1
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
