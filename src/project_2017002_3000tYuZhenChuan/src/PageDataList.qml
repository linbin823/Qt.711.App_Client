import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import "../../qml/modules"

Item {
    id: site
    clip: true
    //default property alias将一个属性设置一个别名，且该别名是该Object的默认被赋值属性
    //default property alias content: pane.contentItem
    property var mimicEnterance: [
        "PageMimic_1.qml",
        "PageMimic_2.qml",
        "PageMimic_3.qml",
        "PageMimic_4.qml",
        "PageMimic_5.qml",
        "PageMimic_6.qml"
    ]
    property var mimicTitles: [
        "第一页 电能管理",
        "第二页 左推进器",
        "第三页 右推进器",
        "第四页 1#发电机",
        "第五页 2#发电机",
        "第六页 3#发电机"
    ]
    property int currentIndex : 0
    signal parentGoBack()
    property real controlBarRatio: 0.04//topside control bar

    //topside control bar
    Rectangle{
        id: controlBar
        width: parent.width
        height: parent.height * controlBarRatio
        anchors.top: parent.top
        anchors.left: parent.left
        color: Qt.lighter("#009fe8")
        Image {
            id: backIcon
            source: "qrc:/img/icon-back.png"
            height: controlBar.height * 0.9
            x:parent.width * 0.02
            fillMode: Image.PreserveAspectFit
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    goBack()
                }
            }
        }
    }

    //mimic 弹出页面
    Popup {
        id: mimicPopup
        modal: true
        margins: 0
        width:  app.width
        height: app.height
        x: 0
        y: 0
        ModuleMimicViewer{
            id:mimicViewer
            anchors.fill: parent
        }
        onOpened: {
            currentIndex = 0
            mimicViewer.itemSource = mimicEnterance[currentIndex]
            mimicViewer.mimicTitle = mimicTitles[currentIndex]
            mimicViewer.color = "#00494d"
            //color: "#009fe8"
            mimicViewer.initScaleAndPos()
        }
        Connections{
            target: mimicViewer
            onParentWindowClose: {
                mimicPopup.close()
            }
            onParentNextView: {
                currentIndex ++
                if(currentIndex >5)
                    currentIndex = 0
                mimicViewer.itemSource = mimicEnterance[currentIndex]
                mimicViewer.mimicTitle = mimicTitles[currentIndex]
                mimicViewer.initScaleAndPos()
            }
            onParentPreviousView: {
                currentIndex --
                if(currentIndex <0)
                    currentIndex = 5
                mimicViewer.itemSource = mimicEnterance[currentIndex]
                mimicViewer.mimicTitle = mimicTitles[currentIndex]
                mimicViewer.initScaleAndPos()
            }
        }
    }

    //trend 弹出页面
    Popup {
        id: trendPopup
        modal: true
        margins: 0
        width:  app.width
        height: app.height
        x: 0
        y: 0
        ModuleTrendViewer{
            id:trendViewer
            anchors.fill: parent
        }
        onOpened: {
            trendViewer.color = "#00494d"
            //color: "#009fe8"
        }
        Connections{
            target: trendViewer
            onParentWindowClose: {
                trendPopup.close()
            }
        }
    }

    //数据列表页面
    Flickable {
        id: listPage
        width: parent.width
        height: parent.height * (1-controlBarRatio)
        anchors.top: controlBar.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        clip: true
        contentHeight: pane.implicitHeight
        flickableDirection: Flickable.VerticalFlick
        visible: true
        Rectangle {

            id: pane
            width: parent.width
            anchors.top: parent.top
            anchors.left: parent.left

            Image{
                id:sketch
                height: site.height * 0.2
                fillMode: Image.PreserveAspectFit
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: site.width * 0.02
                source:"qrc:/project_2017001_JingGang1Hao/img/jingang1-2.jpg"
                smooth: true
            }

            TextEdit {
                width: site.width - sketch.width
                height: sketch.height
                anchors.right : parent.right
                anchors.rightMargin: site.width * 0.02

                horizontalAlignment: TextEdit.AlignRight
                font.pointSize: 12
                color: "black"
                readOnly: true
                text: "净港一号\n母港：香港\n长度：70米\t入籍：LR\n\n当前位置\n北纬22.34度\t东经115.22度"

            }
            RowLayout {
                id: pbs
                height: site.height * 0.06
                width: parent.width * 0.96
                anchors.top: sketch.bottom
                anchors.topMargin: site.height * 0.02
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5

                Text{
                    id: selectorId
                    text: "筛选:"
                    Layout.fillHeight: true
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox{
                    id:systemSelector
                    flat: true
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    onCurrentIndexChanged: {
                        dataSource.filter("净港一号", "", dataSource.sysNameList[currentIndex] )
                        dataList.model = dataSource.tagList
                    }
                    Component.onCompleted: {
                        model = dataSource.sysNameList
                    }
                }

                Button {
                    id:pbMimic
                    text:"图形页面"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    //flat: true
                    onClicked: {
                        mimicPopup.open()
                    }
                    background: Rectangle{
                        anchors.fill: parent
                        color: pbMimic.pressed?"grey":Qt.lighter("grey")
                    }
                }
            }
            Rectangle{
                id: pbsBg
                width: parent.width * 0.96
                height: 5
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: pbs.bottom
                anchors.topMargin: site.height * 0.005
                color: Qt.lighter("#009fe8")
            }

            ListView {
                id: dataList
                width: parent.width * 0.96
                height: site.height * 0.72
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: pbs.bottom
                anchors.topMargin: site.height * 0.02
                visible: true
                clip: true
                delegate: ModuleFoldableBar{
                    id:flodableBar
                    //should not restrict height or width,
                    //auto generated from the size of "mainBarContent" and "detailBarContent"
                    property real unitFontSize: dataList.height / 8 * 0.25
                    mainBarContent: Rectangle {
                        width : dataList.width
                        height : dataList.height / 8
                        color: "white"
                        border.color: "darkgray"
                        Text {
                            id: name
                            text: model.modelData.tagName
                            color: "black"
                            width: parent.width * 0.6
                            wrapMode: Text.Wrap
                            font.pixelSize: unitFontSize
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: parent.width * 0.02
                        }
                        Text {
                            id: value
                            text: model.modelData.tagValue.toFixed(2)
                            color: "black"
                            width: parent.width * 0.1
                            wrapMode: Text.Wrap
                            font.pixelSize: unitFontSize
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: name.right
                            anchors.leftMargin: parent.width * 0.04
                        }
                        Text {
                            id: unit
                            text: model.modelData.tagUnit
                            color: "black"
                            width: parent.width * 0.1
                            wrapMode: Text.Wrap
                            font.pixelSize: unitFontSize
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: value.right
                            anchors.rightMargin: parent.width * 0.02
                        }
                        Image {
                            id: img
                            source: flodableBar.isDetailShow? "qrc:/img/icon-up.png" : "qrc:/img/icon-down.png"
                            height: parent.height * 0.2
                            fillMode: Image.PreserveAspectFit
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: unit.right
                            anchors.leftMargin: parent.width * 0.02
                        }
                    }
                    detailBarContent: Rectangle{
                        width : dataList.width
                        height : dataList.height / 4
                        border.color: "green"
                        Column{
                            width: parent.width * 0.6
                            height: parent.height
                            x:parent.width * 0.04
                            y:parent.width * 0.005
                            spacing: parent.width * 0.005
                            Text {
                                id: tid
                                text: "测点编号:  " + model.modelData.tagID.toString(16)
                                color: "black"
                                font.pixelSize: unitFontSize
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                            Text {
                                id: lastUpdateTime
                                text: "最后更新:  " + model.modelData.lastUpdateTime.toUTCString()
                                color: "black"
                                font.pixelSize: unitFontSize
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                            Text {
                                id: sysName
                                text: "所属系统:  " + model.modelData.systemName
                                color: "black"
                                font.pixelSize: unitFontSize
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                            Text {
                                id: siteName
                                text: "现场名称:" + model.modelData.projectName
                                color: "black"
                                font.pixelSize: unitFontSize
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                        Flow{
                            width: parent.width * 0.3
                            height: parent.height
                            anchors.right: parent.right
                            anchors.rightMargin: parent.width * 0.04
                            y:parent.width * 0.005
                            spacing: parent.width * 0.005

                            Button{
                                text: "现在刷新"
                                onClicked: {
                                    dataSource.readOnce(model.modelData.tagID)
                                }
                            }
                            Button{
                                text: "趋势分析"
                                onClicked: {
                                    trendPopup.open()
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
