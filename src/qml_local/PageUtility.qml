import QtQuick 2.6
import QtQuick.Controls 2.1

Item {

    property bool canGoBack : false
    function goBack(){
    }
    StackView {
        id: utilityView
        anchors.fill: parent

        initialItem: ListView {
            anchors.fill : parent
            model: ListModel {
                ListElement {
                    title: "用户登录"
                }
                ListElement {
                    title: "软件升级"
                }
            }

            delegate: Item{
                width: utilityView.width
                height: utilityView.height / 10.0
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if(title == "软件升级"){
                            checkNewVersionPopup.open()
                        }
                    }
                }
                Rectangle{
                    anchors.fill: parent
                    color: "white"
                    border.color: "darkgray"
                    border.width: 1
                }
                Text {
                    id: description
                    text: title
                    color: "black"
                    font.pixelSize: parent.height * 0.25
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                }
                Image {
                    source: "qrc:/img/icon-go.png"
                    width: Math.min(sourceSize.width, height)
                    height: description.implicitHeight
                    fillMode: Image.PreserveAspectFit
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                }
            }
        }
    }

    Popup{
        id:checkNewVersionPopup
        modal: true
        margins: 0
        x: (parent.width - width) / 2
        y: parent.height / 6
        width: parent.width / 4 * 3
        height: parent.height / 4 * 2

        background: Rectangle{
            color: Qt.darker("white")
            radius: 30
            anchors.fill: parent
            Rectangle{
                x: 0
                y: parent.height *0.2
                height: parent.height * 0.6
                width: parent.width
                color: "white"
            }
            Label {
                id: updateIntro
                width: parent.width
                height: parent.height *0.2
                anchors.top: parent.top
                anchors.left: parent.left
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "船舶数据中心移动客户端\n软件升级"
                wrapMode: Text.Wrap
                font.pixelSize: height*0.3
                color: "white"
            }
            Label {
                id: lbVersionMsg
                width: parent.width * 0.9
                height: parent.height *0.1
                anchors.top: updateIntro.bottom
                anchors.topMargin: parent.height* 0.02
                anchors.right: parent.right
                anchors.rightMargin: parent.width * 0.05
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignTop
                wrapMode: Text.Wrap
                font.pixelSize: height*0.3
                color: "black"
            }
            Label {
                id: lbErrorMsg
                width: parent.width * 0.9
                height: parent.height *0.1
                anchors.top: lbVersionMsg.bottom
                anchors.topMargin: parent.height* 0.02
                anchors.right: parent.right
                anchors.rightMargin: parent.width * 0.05
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignTop
                wrapMode: Text.Wrap
                font.pixelSize: height*0.3
                color: "black"
                text: settings.errMsg
            }
            ProgressBar{
                id: pgbDownloadProgress
                from: 0
                to: 100
                visible: false
                value: settings.updateProgress
                width: parent.width * 0.8
                height: parent.height *0.1
                anchors.top: lbErrorMsg.bottom
                anchors.topMargin: parent.height* 0.02
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Label {
                id: lbDownloadProgress
                visible: false
                width: parent.width * 0.9
                height: parent.height *0.1
                anchors.top: pgbDownloadProgress.bottom
                anchors.topMargin: parent.height* 0.02
                anchors.right: parent.right
                anchors.rightMargin: parent.width * 0.05
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignTop
                wrapMode: Text.Wrap
                font.pixelSize: height*0.3
                color: "black"
            }
            Button{
                id: pbOk
                width: parent.width / 2
                height: parent.height * 0.2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                flat: true
                background.opacity: 0.3
                Text{
                    id: pbOKText
                    anchors.fill: parent
                    text:"查询新版本"
                    font.pixelSize: height*0.3
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                onClicked: {
                    if( settings.hasNewVersion ){
                        settings.startDownload()
                    }
                    else{
                        settings.checkNewVersion()
                    }
                }
            }
            Button{
                id: pbCancel
                width: parent.width / 2
                height: parent.height * 0.2
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                flat: true
                background.opacity: 0.3
                Text{
                    id: pbCancelText
                    anchors.fill: parent
                    text:"取消"
                    font.pixelSize: height*0.3
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                onClicked: {
                    checkNewVersionPopup.close()
                }
            }
            Connections{
                target: settings
                onAccsessConfigChanged:{
                    if(settings.accsessConfig === true){
                        //start to check new version
                        pbOk.enabled = false
                    }
                    else{
                        //stop to check new version
                        pbOk.enabled = true
                        if(settings.hasNewVersion === true){
                            //find new version
                            lbVersionMsg.text = "当前版本号：" + settings.localVersion() + "\n最新版本号：" + settings.serverVersion()
                            pbOKText.text = "下载"
                        }
                        else{
                            //can't find new version
                            lbVersionMsg.text = "当前版本号：" + settings.localVersion() + "\n已经是最新版本"
                            pbOKText.text = "查询新版本"
                        }
                    }
                }
                onAccsessAppChanged:{
                    if(settings.accsessApp === true){
                        //start to download app
                        pbOk.enabled = false
                        pgbDownloadProgress.visible = true
                        pgbDownloadProgress.value = 0
                        lbDownloadProgress.visible = true
                        lbDownloadProgress.text = ""
                    }
                    else{
                        //download app finished
                        pbOk.enabled = true
                    }
                }
                onUpdateProgressChanged:{
                    lbDownloadProgress.text = settings.sizeFinishedKB() + " / " + settings.sizeTotalKB()
                }
            }
        }//end of popup's background
        onOpened: {
            pbOk.enabled = true
            pbOKText.text = "查询新版本"
            lbVersionMsg.text ="当前版本号：" + settings.localVersion()
            pgbDownloadProgress.visible = false
            lbDownloadProgress.visible = false
        }
        onClosed: {
            settings.abortDownload()
        }
    }//end of popup


}
