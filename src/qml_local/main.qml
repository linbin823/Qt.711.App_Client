import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1
import Qt.labs.settings 1.0

ApplicationWindow {
    id: appWindow
    property ApplicationWindow app : appWindow

    visible: true
    property real naviBarRatio : 0.07

    color: "white"
    title: qsTr("QYY船舶数据中心")
    width: 640
    height: 480

    Keys.onReleased : {
        if(event.key === Qt.Key_Back)
            if( content.currentItem.canGoBack ){
                content.currentItem.goBack()
                event.accepted=true;
            }
    }

    PageCover{
        id: cover
        visible: true
        width: appWindow.width
        anchors.top: parent.top
        height: appWindow.height
        opacity: 0
        OpacityAnimator {
            id:cover_appear
            target: cover
            from:0
            to:1
            duration: 1000
            running: true
        }
        OpacityAnimator {
            id:cover_fade
            target: cover
            from:1
            to:0
            duration: 1000
            running: false
            onStopped: {
                content.visible = true
                navi_bar.visible = true
                cover.visible = false
            }
        }
        Timer {
            id:cover_timer
            repeat: false
            running: true
            interval: 2000
            onTriggered:{
                cover_fade.start()
            }
        }
    }
    SwipeView {
        id: content
        visible: false
        currentIndex: 0
        width: appWindow.width
        anchors.top: parent.top
        height: appWindow.height * (1-naviBarRatio)
        interactive:  false

        PageHome {
            width:  content.width
            height: content.height
            objectName: "home_page"
        }
        PageGIS{
            width:  content.width
            height: content.height
            objectName: "gis_page"
        }
        PageData {
            width:  content.width
            height: content.height
            objectName: "data_page"
        }
        PageUtility {
            width:  content.width
            height: content.height
            objectName: "utility_page"
        }
    }
    footer: ModuleNaviBar {
        id: navi_bar
        visible: false
        height: appWindow.height * naviBarRatio
        onKeyPressed: {
            if(target === "home_page" && target !== content.currentItem.objectName){
//                debug_view.log(content.currentItem.objectName)
                content.setCurrentIndex(0)
                navi_bar.setActiveButton( target )
            }
            else if(target === "gis_page" && target !== content.currentItem.objectName){
//                debug_view.log(content.currentItem.objectName)
                content.setCurrentIndex(1)
                navi_bar.setActiveButton( target )
            }
            else if(target === "data_page" && target !== content.currentItem.objectName){
//                debug_view.log(content.currentItem.objectName)
                content.setCurrentIndex(2)
                navi_bar.setActiveButton( target )
            }
            else if(target === "utility_page" && target !== content.currentItem.objectName){
//                debug_view.log(content.currentItem.objectName)
                content.setCurrentIndex(3)
                navi_bar.setActiveButton( target )
            }
        }
    }


}
