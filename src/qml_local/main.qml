import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1
import QtWebView 1.0
import Qt.labs.settings 1.0

ApplicationWindow {
    id: appWindow
    visible: true
    width: 640
    height: 480
    color: "white"
    title: qsTr("QYY")

    property ApplicationWindow rootWindow : appWindow
    property real rowHeight : appWindow.height * 0.07
    property bool isLogin : false
    property string currenTitle: qsTr("未连接")

    Settings {
        id: settings
        property bool isFirstTime: true
        property bool isAutoLogInActive: false
        property bool isPageAndDataSame:true
        property date lastManualLogin
        property string pageServerUrl : "http://127.0.0.1"
        property string dataServerUrl: "http://127.0.0.1"
        property int pageServerPort  : 8080
        property int dataServerPort  : 8080
        property string usrName  : ""
        property string usrPswd  : ""
    }
    function genWholePageUrl(){
        var url = settings.pageServerUrl;
        url    += ":" + settings.pageServerPort;
        return url;
    }
    function genWholeDataUrl(){
        var url = settings.dataServerUrl;
        url    += ":" + settings.dataServerPort;
        return url;
    }

    function autoFixUrl(oriental){
        var str = oriental.replace(/\s+/g, "");
        str = str.replace("\\", "/");
        if( str.indexOf("http://") !== 0){
            str = "http://" + str
        }
        return str;
    }
    function errorPage(){
        ctxLoader.setSource("qrc:/qml_local/PageError.qml")
    }

    function initNaviPage(){
        ctxLoader.setSource( "" )
        var url = genWholePageUrl()+"/qml_remote/NaviPage.qml"
        ctxLoader.setSource( url ,
                            {"wholeDataUrl":genWholeDataUrl()}
                            )
    }

    function defaultSettings(){
        settings.isFirstTime = true
        settings.isAutoLogInActive = false
        settings.pageServerUrl = "127.0.0.1"
        settings.dataServerUrl = "127.0.0.1"
        settings.pageServerPort = 8080
        settings.dataServerPort = 8080
        settings.usrName = ""
        settings.usrPswd = ""
        settings.lastManualLogin = new Date()
    }

    function sendLogInRequest(){
        if(settings.usrName=="" ) return;
        var xhr = new XMLHttpRequest()
        var url = genWholeDataUrl()
        url += encodeURI("/admin.php?m=user&f=login");
        //console.log(url)
        var data={}
        data.account = settings.usrName
        data.password = settings.usrPswd
        data.referer = encodeURI(url);
        data.Fingerprint = ""
        var dataString = JSON.stringify(data)
        xhr.open("POST", url, true);
        xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        xhr.send( dataString );
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.HEADERS_RECEIVED){
                console.log(xhr.getAllResponseHeaders())
            }
            if (xhr.readyState === XMLHttpRequest.DONE) {
                console.log(xhr.responseText)
            }
            isLogin = true;
        }
    }

    function goBack(){
        if(ctxLoader.item.canGoBack){
            ctxLoader.item.goBack();
            return true
        }
        return false
    }

    onClosing: {
        var done = goBack()
        if(done){
            close.accepted = false
        }
    }

    Component.onCompleted: {
        //auto log-in
        if(settings.isAutoLogInActive){
            logIn()
        }

        //init Navigate Page
        initNaviPage()
    }

    PageCover{
        id: cover
        visible: true
        anchors.fill: parent
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
                visibleContent.visible = true
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

    Rectangle{
        id:visibleContent
        anchors.fill: parent
        visible: false

        ToolBar {
            id:tools
            x: 0
            y: 0
            width: parent.width
            height: rowHeight

            ProgressBar{
                id: loadingProgressBar
                width: tools.width
                height: tools.height / 16
                visible: false
                x:0
                y:tools.height - height
                z: 2
                indeterminate: true
            }
            RowLayout {
                spacing: 10
                width: tools.width
                height: tools.height

                ToolButton {
                    contentItem: Image {
                        fillMode: Image.PreserveAspectFit
                        horizontalAlignment: Image.AlignHCenter
                        verticalAlignment: Image.AlignVCenter
                        source: "qrc:/images/icons/back@4x.png"
                    }
                    onClicked: goBack();

                }//end of ToolButton

                Label {
                    id: titleLabel
                    text:currenTitle
                    font.pixelSize: tools.height*0.5
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: true
                }

                ToolButton {

                    contentItem: Image {
                        fillMode: Image.PreserveAspectFit
                        horizontalAlignment: Image.AlignHCenter
                        verticalAlignment: Image.AlignVCenter
                        source: "qrc:/images/icons/menu@4x.png"
                    }
                    onClicked: optionsMenu.open()

                }//end of ToolButton
            }//end of RowLayout
        }//end of ToolBar
        Loader{
            id: ctxLoader
            anchors.top: tools.bottom
            anchors.left: tools.left
            width: parent.width
            anchors.bottom: parent.bottom
            onStatusChanged: {
                if(status === Loader.Error){
                    loadingProgressBar.visible = false
                    errorPage();
                }
                if(status === Loader.Loading){
                    loadingProgressBar.visible = true
                }
                if(status === Loader.Ready){
                    loadingProgressBar.visible = false
                }
            }
        }

        Connections{
            target:ctxLoader.item
            onTitleChanged: {
                currenTitle = title
            }
        }
    }//end of visibleContent


    Menu {
        id: optionsMenu
        x: appWindow.width - width
        y: tools.height
        transformOrigin: Menu.TopRight
        background: Rectangle {
            implicitWidth: appWindow.width * 0.4
            implicitHeight: rowHeight * 4
            color: "#ffffff"
            border.color: "#353637"
        }
        MenuItem {
            text: qsTr("刷新")
            onTriggered: {
                initNaviPage();
            }
        }

        MenuItem {
            text: qsTr("服务器设置")
            onTriggered: settingsDialog.open()
        }
        MenuItem {
            text: qsTr("用户登录")
            onTriggered: accountDialog.open()
        }

        MenuItem {
            text: qsTr("版本更新")
        }

    }

    Dialog {
        id: settingsDialog
        x: Math.round((rootWindow.width - width) / 2)
        y: Math.round(rootWindow.height / 6)
        width: Math.min(rootWindow.width, rootWindow.height) * 0.8
        contentHeight: settingsColumn.height
        modal: true;
        focus: true;
        title: qsTr("服务器设置");

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            settings.isPageAndDataSame = isPageAndDataSame.checked
            if(isPageAndDataSame.checked){
                settings.pageServerUrl = autoFixUrl(pageServerUrl.displayText);
                settings.pageServerPort = pageServerPort.displayText;
                settings.dataServerUrl = autoFixUrl(pageServerUrl.displayText);
                settings.dataServerPort = pageServerPort.displayText;
            }else{
                settings.pageServerUrl = autoFixUrl(pageServerUrl.displayText);
                settings.pageServerPort = pageServerPort.displayText;
                settings.dataServerUrl = autoFixUrl(dataServerUrl.displayText);
                settings.dataServerPort = dataServerPort.displayText;
            }
            settingsDialog.close();
            initNaviPage()
        }
        onRejected: {
            settingsDialog.close();
        }
        ColumnLayout {
            id: settingsColumn
            spacing: 20
            RowLayout {
                spacing: 10
                height: rowHeight
                CheckBox{
                    id: isPageAndDataSame
                    Layout.fillHeight: true
                    font.pixelSize: rowHeight * 0.4
                    Component.onCompleted: {
                        //initial the form
                        checked = settings.isPageAndDataSame
                        onCheckedChanged()
                    }
                    onCheckedChanged: {
                        if(checked){
                            dataServerPortSettings.visible = false
                            dataServerUrlSettings.visible = false
                        }else{
                            dataServerPortSettings.visible = true
                            dataServerUrlSettings.visible = true
                        }
                    }
                }
                Label {
                    text: qsTr("页面服务器和数据服务器地址相同")
                    Layout.fillHeight: true
                    font.pixelSize: rowHeight * 0.4
                    verticalAlignment: Text.AlignVCenter

                }
            }
            RowLayout {
                spacing: 10
                height: rowHeight
                CheckBox{
                    id: isIP
                    Layout.fillHeight: true
                    font.pixelSize: rowHeight * 0.4
                    checked: false
                    onCheckedChanged: {
                        var pageOld = pageServerUrl.displayText
                        var dataOld = dataServerUrl.displayText
                        if(checked){
                            pageServerUrl.inputMethodHints = Qt.ImhPreferNumbers
                            pageServerUrl.inputMask = "000.000.000.000"
                            pageServerUrl.text = pageOld

                            dataServerUrl.inputMethodHints = Qt.ImhPreferNumbers
                            dataServerUrl.inputMask = "000.000.000.000"
                            dataServerUrl.text = dataOld
                        }else{
                            pageServerUrl.inputMethodHints = Qt.ImhUrlCharactersOnly
                            pageServerUrl.inputMask = ""
                            pageServerUrl.text = pageOld

                            dataServerUrl.inputMethodHints = Qt.ImhUrlCharactersOnly
                            dataServerUrl.inputMask = ""
                            dataServerUrl.text = dataOld
                        }
                    }
                }
                Label {
                    text: qsTr("IP地址")
                    Layout.fillHeight: true
                    font.pixelSize: rowHeight * 0.4
                    verticalAlignment: Text.AlignVCenter

                }
            }
            RowLayout {
                id: pageServerUrlSettings
                spacing: 10
                height: rowHeight

                Label {
                    text: qsTr("页面服务器地址:")
                    Layout.fillHeight: true
                    font.pixelSize: rowHeight * 0.4
                    verticalAlignment: Text.AlignVCenter
                }
                TextInput{
                    id: pageServerUrl
                    text : settings.pageServerUrl
                    Layout.fillHeight: true
                    Layout.fillWidth:  true
                    font.pixelSize: rowHeight * 0.4
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: TextInput.WrapAnywhere
                }
            }
            RowLayout {
                id: pageServerPortSettings
                spacing: 10
                height: rowHeight

                Label {
                    text: qsTr("页面服务器端口:")
                    Layout.fillHeight: true
                    font.pixelSize: rowHeight * 0.4
                    verticalAlignment: Text.AlignVCenter
                }
                TextInput{
                    id: pageServerPort
                    text : settings.pageServerPort
                    inputMask: "00000"
                    Layout.fillHeight: true
                    Layout.fillWidth:  true
                    font.pixelSize: rowHeight * 0.4
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: TextInput.WrapAnywhere
                    inputMethodHints : Qt.ImhPreferNumbers
                }
            }
            RowLayout {
                id: dataServerUrlSettings
                spacing: 10
                height: rowHeight

                Label {
                    text: qsTr("数据服务器地址:")
                    Layout.fillHeight: true
                    font.pixelSize: rowHeight * 0.4
                    verticalAlignment: Text.AlignVCenter
                }
                TextInput{
                    id: dataServerUrl
                    text : settings.dataServerUrl
                    Layout.fillHeight: true
                    Layout.fillWidth:  true
                    font.pixelSize: rowHeight * 0.4
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: TextInput.WrapAnywhere
                }
            }
            RowLayout {
                id: dataServerPortSettings
                spacing: 10
                height: rowHeight

                Label {
                    text: qsTr("数据服务器端口:")
                    Layout.fillHeight: true
                    font.pixelSize: rowHeight * 0.4
                    verticalAlignment: Text.AlignVCenter
                }
                TextInput{
                    id: dataServerPort
                    text : settings.dataServerPort
                    inputMask: "00000"
                    Layout.fillHeight: true
                    Layout.fillWidth:  true
                    font.pixelSize: rowHeight * 0.4
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: TextInput.WrapAnywhere
                    inputMethodHints : Qt.ImhPreferNumbers
                }
            }
        }
    }//end of settings dialog

    Dialog {
        id: accountDialog
        x: (rootWindow.width - width) / 2
        y: rootWindow.height / 6
        width: Math.min(rootWindow.width, rootWindow.height) * 0.8
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
            }
            sendLogInRequest();
            accountDialog.close()
        }
        onRejected: {
            accountDialog.close()
        }

        ColumnLayout {
            id: accountColumn
            spacing: 20
            width: parent.width

            RowLayout {
                spacing: 10
                height: rowHeight

                Label {
                    text: qsTr("用户名:")
                    Layout.fillHeight: true
                    font.pixelSize: rowHeight * 0.5
                    verticalAlignment: Text.AlignVCenter
                }
                TextInput{
                    id: usrName
                    Layout.fillHeight: true
                    Layout.fillWidth:  true
                    font.pixelSize: rowHeight * 0.5
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: TextInput.WrapAnywhere
                }

            }
            RowLayout {
                spacing: 10
                height: rowHeight

                Label {
                    text: qsTr("密码:")
                    Layout.fillHeight: true
                    font.pixelSize: rowHeight * 0.5
                    verticalAlignment: Text.AlignVCenter
                }
                TextInput{
                    id: usrPswd
                    echoMode: TextInput.PasswordEchoOnEdit
                    Layout.fillHeight: true
                    Layout.fillWidth:  true
                    font.pixelSize: rowHeight * 0.5
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: TextInput.WrapAnywhere
                }

            }
            RowLayout {
                spacing: 10
                height: rowHeight

                CheckBox{
                    id: isAutoLogInFor15Days
                    checked: false
                    Layout.fillHeight: true
                    font.pixelSize: rowHeight * 0.5
                }
                Label {
                    text: qsTr("自动登录(15天)")
                    Layout.fillHeight: true
                    font.pixelSize: rowHeight * 0.5
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }//end of dialog account
}
