import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.0
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1

Rectangle{
    Image{
        id:companyLogo
        sourceSize.width: 1080
        sourceSize.height: 715
        width: parent.width / 1.5
        fillMode: Image.PreserveAspectFit
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -20
        source:"qrc:/img/CSIC.png"
        anchors.horizontalCenter: parent.horizontalCenter
        smooth: true

    }
    Text{
        id:test
        width: parent.width / 2
        anchors.horizontalCenter: companyLogo.horizontalCenter
        anchors.top: companyLogo.bottom
        anchors.topMargin: 500
        text: "船舶数据中心"
        font.pointSize: 18
        horizontalAlignment: Text.AlignHCenter
    }
}
