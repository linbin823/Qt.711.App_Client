import QtQuick 2.7
import QtQuick.Controls 2.1
import QtWebView 1.0


Item {
    property alias canGoBack : gisView.canGoBack
    function goBack() {
        gisView.goBack();
    }

    visible: true
    //anchors.fill: parent

    WebView {
        id: gisView
        anchors.fill: parent
        url: "http://app.csic711.net/qydl2.html"
    }
}
