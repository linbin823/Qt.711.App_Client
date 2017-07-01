import QtQuick 2.2
import QtQuick.Controls 2.1
Image{
    id:companyLogo
    sourceSize.width: 720
    sourceSize.height: 1280
    anchors.fill: parent
    fillMode: Image.PreserveAspectCrop
    source:"qrc:/images/splash/splash.png"
    smooth: true

}
