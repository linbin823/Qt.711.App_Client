import QtQuick 2.2
import QtQuick.Controls 2.1
import QtWebView 1.0
import "../modules/"

Item {
    property alias canGoBack : homeView.canGoBack
    function goBack(){
       //homeView.goBack();
    }

    visible: true

//    WebView {
//        id: homeView
//        anchors.fill: parent
//        url: "http://www.csic-711.com/ch/main.asp"
//    }

     ModuleTrendViewer{
         id: homeView
         property bool canGoBack
         anchors.fill: parent
     }
}
