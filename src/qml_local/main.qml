import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Universal 2.1
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1
import Qt.labs.settings 1.0

ApplicationWindow {
    id: appWindow
    visible: true
    width: 640
    height: 480
    color: "white"
    title: qsTr("QYY")

    property ApplicationWindow rootWindow : appWindow
    property real naviBarRatio : 0.07

    Settings {
        id: settings
        property bool isFirstTime: true
        property bool isAutoLogInActive: false
        property date lastManualLogin
        property string serverIP : "127.0.0.1"
        property int serverPort  : 8080
        property string usrName  : ""
        property string usrPswd  : ""
        property string currentProjectName: qsTr("3000T渔政机舱监测报警")
    }
    function defaultSettings(){
        settings.isFirstTime = true
        settings.isAutoLogInActive = false
        settings.serverIP = "127.0.0.1"
        settings.usrName = ""
        settings.usrPswd = ""
        settings.lastManualLogin = new Date()
        settings.currentProjectName = qsTr("3000T渔政机舱监测报警")
    }
    function logIn(){
        if(settings.usrName=="" || settings.usrPswd=="" ) return;
        var xhr = new XMLHttpRequest
        var url = "http://" + settings.serverIP;
        url += ":" + settings.serverPort;
        url += encodeURI("/admin.php?m=user&f=login");
        var data={}
        data.account = settings.usrName
        data.password = settings.usrPswd
        data.referer = ""
        data.Fingerprint = ""
        var dataString = JSON.stringify(data)
        console.log(url)
        console.log(dataString)
        xhr.open("POST", url, true);
        // 设置POST请求的请求头
        xhr.setRequestHeader("Content-Length",dataString.lenght);
        xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded;");
        xhr.onreadystatechange = function() {
//            if (xhr.readyState === XMLHttpRequest.LOADING || xhr.readyState === XMLHttpRequest.DONE) {
//                var records = xhr.responseText.split('\n');

//                for (;i < records.length; i++ ) {
//                    var r = records[i].split(',');
//                    if (r.length === 7)
//                        model.append(createStockPrice(r));
//                }

//                if (xhr.readyState === XMLHttpRequest.DONE) {
//                    if (model.count > 0) {
//                        model.ready = true;
//                        model.stockPrice = model.get(0).adjusted;
//                        model.stockPriceChanged = model.count > 1 ? (Math.round((model.stockPrice - model.get(1).close) * 100) / 100) : 0;
//                    } else {
//                        model.stockPrice = 0;
//                        model.stockPriceChanged = 0;
//                    }
//                    model.dataReady(); // emit signal - model.ready indicates whether the data is valid
//                }
//            }
        }
        xhr.send( dataString )
    }

    Component.onCompleted: {
        //show cover
        if(settings.isFirstTime || !settings.isAutoLogInActive){
            visibleContent.opacity = 0
            cover.open();
        }
        //init Loader

        //auto log-in
        if(settings.isAutoLogInActive){
            logIn()
        }
    }
    Keys.onReleased : {
        if(event.key === Qt.Key_Back)
            if( content.currentItem.canGoBack ){
                content.currentItem.goBack()
                event.accepted=true;
            }
    }
    PageCover{
        id: cover
        visible: settings.isFirstTime
        width: rootWindow.width
        height: rootWindow.height
        x:0
        y:0
        opacity: 1
        onFinished: {
            contentFadeIn.start()
            //第二次不启动
            settings.isFirstTime = false;
        }
    }
    OpacityAnimator {
        id:contentFadeIn
        running: false
        from: 0;
        to: 1;
        duration: 200
        target: visibleContent
        onStarted: {
            cover.close();
        }
    }

    Rectangle{
        id:visibleContent
        anchors.fill: parent

        ToolBar {
            position: ToolBar.Header
            width: parent.width
            Material.foreground: "white"

            RowLayout {
                spacing: 20
                anchors.fill: parent

                Label {
                    id: titleLabel
                    text:settings.currentProjectName
                    font.pixelSize: 20
                    elide: Label.ElideRight
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    Layout.fillWidth: true
                }

                ToolButton {
                    contentItem: Image {
                        fillMode: Image.Pad
                        horizontalAlignment: Image.AlignHCenter
                        verticalAlignment: Image.AlignVCenter
                        source: "qrc:/images/icons/menu.png"
                    }
                    onClicked: optionsMenu.open()

                    Menu {
                        id: optionsMenu
                        x: parent.width - width
                        transformOrigin: Menu.TopRight

                        MenuItem {
                            text: qsTr("服务器设置")
                            onTriggered: settingsDialog.open()
                        }
                        MenuItem {
                            text: qsTr("用户登录")
                            onTriggered: accountDialog.open()
                        }
                    }
                }//end of ToolButton
            }//end of RowLayout
        }//end of ToolBar


        Loader{

        }
    }//end of visibleContent




    Dialog {
        id: settingsDialog
        x: Math.round((rootWindow.width - width) / 2)
        y: Math.round(rootWindow.height / 6)
        width: Math.round(Math.min(rootWindow.width, rootWindow.height) / 3 * 2)
        contentHeight: settingsColumn.height
        modal: true
        focus: true
        title: qsTr("服务器设置")

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            settings.serverIP = serverIP.displayText.replace(/\s+/g, "");
            settings.serverPort = serverPort.displayText;
            settingsDialog.close();
        }
        onRejected: {
            settingsDialog.close();
        }

        ColumnLayout {
            id: settingsColumn
            spacing: 20

            RowLayout {
                spacing: 5

                Label {
                    text: qsTr("服务器地址:")
                }
                TextInput{
                    id: serverIP
                    text : settings.serverIP
                    inputMask: "000.000.000.000"
                }

            }
            RowLayout {
                spacing: 5

                Label {
                    text: qsTr("服务器端口:")
                }
                TextInput{
                    id: serverPort
                    text : settings.serverPort
                    inputMask: "00000"
                }

            }
        }
    }//end of settings dialog

    Dialog {
        id: accountDialog
        x: (rootWindow.width - width) / 2
        y: rootWindow.height / 6
        width: Math.min(rootWindow.width, rootWindow.height) / 3 * 2
        contentHeight: accountColumn.height
        modal: true
        focus: true
        title: qsTr("用户登录")

        onOpened: {
            usrName.text = settings.usrName
            usrPswd.text = settings.usrPswd
        }

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            settings.usrName = usrName.text;
            if(settings.usrPswd != usrPswd.text){
                settings.usrPswd = Qt.md5(  Qt.md5(usrPswd.text)+usrName.text );
                console.log( usrPswd.text )
                console.log( Qt.md5(usrPswd.text) )
                console.log( Qt.md5(usrPswd.text)+usrName.text )
                console.log( Qt.md5(  Qt.md5(usrPswd.text)+usrName.text ) )
            }
            logIn();
            accountDialog.close()
        }
        onRejected: {
            accountDialog.close()
        }

        ColumnLayout {
            id: accountColumn
            spacing: 20
            width: accountDialog.width

            RowLayout {
                spacing: 5

                Label {
                    text: qsTr("用户名:")
                }
                TextInput{
                    id: usrName
                    y:parent.width / 3
                    width:  accountColumn.width / 3 *2
                }

            }
            RowLayout {
                spacing: 5

                Label {
                    text: qsTr("密码:")
                }
                TextInput{
                    id: usrPswd
                    y:parent.width / 3
                    width:  accountColumn.width / 3 *2
                    echoMode: TextInput.PasswordEchoOnEdit
                }

            }
            RowLayout {
                spacing: 5

                CheckBox{
                    id: isAutoLogInFor15Days
                    checked: false
                }
                Label {
                    text: qsTr("自动登录(15天)")
                }
            }
        }
    }//end of dialog account
}
