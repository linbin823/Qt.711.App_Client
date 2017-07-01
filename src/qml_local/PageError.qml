import QtQuick 2.0

Item {
    property bool canGoBack : false
    function goBack(){
        return false
    }

    Text{
        width: parent.width * 0.9
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        text:"索引页面无法打开，请检查网络连接以及服务器设置！"
        color: "grey"
        font.pixelSize: parent.height / 18
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment:  Text.AlignVCenter
        wrapMode : Text.WordWrap
    }
}
