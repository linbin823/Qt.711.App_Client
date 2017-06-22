import QtQuick 2.6
import QtQuick.Controls 2.1
import QtWebView 1.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2

StackView {
    id: dataViewRoot
    property var projectName:      [
        "请选择船舶",
        "净港一号",
        "渔政3000t"
    ]
    property var projectEnterance: [
        "",
        "../../project_2017001_JingGang1Hao/src/PageDataList.qml",
        "../../project_2017002_3000tYuZhenChuan/src/PageDataList.qml"
    ]

    property bool canGoBack: dataViewRoot.depth >1
    function goBack(){
        if(dataViewRoot.depth >1){
            dataViewRoot.pop()
            search.popup.close()
            search.currentIndex = 0
        }
    }

    initialItem: Rectangle{
        id: searchPage
        width: dataViewRoot.width
        height: dataViewRoot.height
        x:0
        y:0

        ComboBox {
            id: search
            width: parent.width * 0.8
            height: parent.height * 0.2
            flat: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            model: projectName
            font.pixelSize: 50
            onCurrentIndexChanged: {
                if(currentIndex == 0) return
                projectLoader.setSource(projectEnterance[currentIndex])
                dataViewRoot.push(projectLoader)
            }
        }
    }
    Connections {
        target: projectLoader.item
        onParentGoBack:{
            goBack()
        }
    }

    Loader {
        id: projectLoader
        width: dataViewRoot.width
        height: dataViewRoot.height
    }
}
