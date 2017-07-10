import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1
import QtWebView 1.0
import Qt.labs.settings 1.0
import "userMgr.js" as UserManagerFunctions

ApplicationWindow {
    id: appWindow
    visible: true
    color: "#e2ebed"
    title: qsTr("QYY")

    property ApplicationWindow rootWindow : appWindow
    property bool   isLogin         : false
    property string currenTitle     : qsTr("数据中心")

    property int    originPixelWidth: 720
    property int    originPixelHeight: 1280
    width:  originPixelWidth
    height: originPixelHeight
    //quick function to caculate origin pixel x or origin pixel width to actual x or actual width
    function actualX(absX){
        return appWindow.width/originPixelWidth*absX
    }
    //quick function to caculate origin pixel y or origin pixel height to actual y or actual height
    function actualY(absY){
        return appWindow.height/originPixelHeight*absY
    }

    function logIn(){
        UserManagerFunctions.logIn( settings.pageServerUrl,
                    settings.usrName,  settings.usrPswd,  "99999",  settings.pageServerUrl,
                    function(data){
                        //get string and verify whether log in is successful.
                    } )
    }

    Settings {
        id: settings
        property bool   isFirstTime         :true
        property bool   isAutoLogInActive   :false
        property bool   isPageAndDataSame   :true
        property date   lastManualLogin
        property string pageServerUrl       : "http://127.0.0.1:8080"
        property string dataServerUrl       : "http://127.0.0.1:8080"
        property string usrName             : ""
        property string usrPswd             : ""
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
        currenTitle = qsTr("未连接")
    }

    function naviPage(){
        ctxLoader.setSource( "" )
        var url = settings.pageServerUrl +"/qml_remote/NaviPage.qml"
        ctxLoader.setSource( url ,
                            {"wholeDataUrl":settings.dataServerUrl}
                            )
        currenTitle = qsTr("数据中心")
    }

    function defaultSettings(){
        settings.isFirstTime = true
        settings.isAutoLogInActive = false
        settings.pageServerUrl = "127.0.0.1"
        settings.dataServerUrl = "127.0.0.1"
        settings.usrName = ""
        settings.usrPswd = ""
        settings.lastManualLogin = new Date()
    }

    //后退
    function goBack(){
        if(ctxLoader.item.canGoBack){
            ctxLoader.item.goBack();
            return true
        }
        return false
    }

    //退出时，转为后退
    onClosing: {
        var done = goBack()
        if(done){
            close.accepted = false
        }
    }

    Component.onCompleted: {
        //auto log-in
        if(settings.isAutoLogInActive){
            logIn();
        }

        //init Navigate Page
        naviPage()
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
            height: actualY(96)
            Rectangle{
                anchors.fill: parent
                color: "#045699"
            }

            ProgressBar{
                id: loadingProgressBar
                width: parent.width
                height: parent.height / 16
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                visible: false
                z: 2
                indeterminate: true
            }
            Image {
                anchors.left: parent.left
                anchors.leftMargin: actualX(20)
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height * 0.4
                width: parent.height
                fillMode: Image.PreserveAspectFit
                horizontalAlignment: Image.AlignLeft
                verticalAlignment: Image.AlignVCenter
                source: "qrc:/images/icons/back.png"
//                visible: ctxLoader.item.canGoBack
                MouseArea{
                    anchors.fill: parent
                    onClicked:{
                        goBack();
                    }
                }
            }
            Label {
                id: labelTitle
                text:currenTitle
                color:"#ffffff"
                font.pixelSize: actualY(32)
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Image {
                anchors.right: parent.right
                anchors.rightMargin: actualX(20)
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height * 0.4
                width:  parent.height
                fillMode: Image.PreserveAspectFit
                horizontalAlignment: Image.AlignRight
                verticalAlignment: Image.AlignVCenter
                source: "qrc:/images/icons/menu.png"
                MouseArea{
                    anchors.fill: parent
                    onClicked:{
                        optionsMenu.open()
                    }
                }
            }//end of ToolButton
        }//end of ToolBar
        Loader{
            id: ctxLoader
            anchors.top: tools.bottom
            anchors.left: tools.left
            anchors.right: tools.right
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


    Popup {
        id: optionsMenu
        x:  appWindow.width - width - actualX(12)
        y:  tools.height + actualY(12)
        width: actualX(280)
        height:actualY(359)
        transformOrigin: Item.TopRight
        background: Image {
            anchors.fill: parent
            source: "qrc:/images/icons/menuBg.png"
            fillMode: Image.Stretch
        }
        ListView{
            x: actualX(-12+5)
            y: actualY(-12+19)
            width: actualX(270)
            height:actualY(330)
            model:ListModel{
                ListElement{
                    name: qsTr("刷新")
                    img: "qrc:/images/icons/menuRefresh.png"
                    onclicked: "naviPage()"
                    separatorBelow: true
                }
                ListElement{
                    name: qsTr("服务器设置")
                    img: "qrc:/images/icons/menuSetting.png"
                    onclicked: "settingsDialog.open()"
                    separatorBelow: true
                }
                ListElement{
                    name: qsTr("用户登录")
                    img: "qrc:/images/icons/menuLogIn.png"
                    onclicked: "accountDialog.open()"
                    separatorBelow: true
                }
                ListElement{
                    name: qsTr("版本更新")
                    img: "qrc:/images/icons/menuUpdate.png"
                    onclicked: ""
                    separatorBelow: false
                }
            }
            delegate: Rectangle{
                width: actualX(270)
                height: actualY(80)
                color:  "transparent"
                Image{
                    width: parent.height * 0.5
                    height: parent.height
                    anchors.left: parent.left
                    anchors.leftMargin: actualY(24)
                    fillMode: Image.PreserveAspectFit
                    verticalAlignment: Image.AlignVCenter
                    source: img
                }
                Text{
                    text: name
                    height: parent.height
                    color:"#ffffff"
                    font.pixelSize: actualX(26)
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                    anchors.leftMargin: parent.height
                }
                Rectangle{
                    visible: separatorBelow
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "#898989"
                    width: actualX(212)
                    height: 1
                }
                MouseArea{
                    anchors.fill: parent
                    onReleased: { eval(onclicked); optionsMenu.close(); }
                }
            }//end of delegate
        }//end of ListView
    }//end of Popup/Menu

    Popup {
        id: settingsDialog
        x: Math.round((rootWindow.width - width) / 2)
        y: Math.round(rootWindow.height / 6)
        width: actualX(620)
        contentHeight: settingsContent.height
        modal: true;
        focus: true;
        background: Rectangle {
            anchors.fill: parent
            border.width: 0
            color: "#ffffff"
            radius: actualX(30)
        }
        ColumnLayout {
            id: settingsContent
            x: actualX(-12)
            y: actualY(-12)
            width: actualX(620)
            spacing: 0
            Text{
                anchors.horizontalCenter: parent.horizontalCenter
                width: actualX(620)
                height: actualY(40)
                color: "#333333"
                font.pixelSize: actualX(36)
                text: qsTr("服务器设置");
                Layout.topMargin: actualY(40)
            }
            RowLayout {
                height: actualY(40)
                Layout.topMargin: actualY(40)
                Layout.leftMargin: actualX(40)
                Layout.rightMargin: actualX(40)
                CheckBox{
                    id: isIP
                    Layout.fillHeight: true
                    checked: false
                    onCheckedChanged: {
                        var pageOld = pageServerUrl.displayText
                        var dataOld = dataServerUrl.displayText
                        if(checked){
                            pageServerUrl.inputMethodHints = Qt.ImhPreferNumbers
                            pageServerUrl.inputMask = "000.000.000.000:0000"
                            pageServerUrl.text = pageOld

                            dataServerUrl.inputMethodHints = Qt.ImhPreferNumbers
                            dataServerUrl.inputMask = "000.000.000.000:0000"
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
                    font.pixelSize: actualX(32)
                    color: "#333333"
                    horizontalAlignment: Text.AlignLeft
                }
            }
            Rectangle{
                height: 1
                color: "#333333"
                Layout.topMargin: actualY(20)
                Layout.leftMargin: actualX(40)
                Layout.rightMargin: actualX(40)
                Layout.fillWidth: true
            }
            RowLayout {
                height: actualY(40)
                Layout.leftMargin: actualX(40)
                Layout.rightMargin: actualX(40)
                Layout.topMargin: actualY(20)
                CheckBox{
                    id: isPageAndDataSame
                    Layout.fillHeight: true
                    Component.onCompleted: {
                        //initial the form
                        checked = settings.isPageAndDataSame
                        onCheckedChanged()
                    }
                    onCheckedChanged: {
                        if(checked){
                            dataServerUrlLabel.visible = false
                            dataServerUrl.visible = false
                        }else{
                            dataServerUrlLabel.visible = true
                            dataServerUrl.visible = true
                        }
                    }
                }
                Label {
                    text: qsTr("页面服务器和数据服务器地址相同")
                    Layout.fillHeight: true
                    font.pixelSize: actualX(32)
                    color: "#333333"
                    horizontalAlignment: Text.AlignLeft
                }
            }
            Rectangle{
                height: 1
                color: "#333333"
                Layout.topMargin: actualY(20)
                Layout.leftMargin: actualX(40)
                Layout.rightMargin: actualX(40)
                Layout.fillWidth: true
            }
            Label {
                text: qsTr("页面服务器地址:")
                height: actualY(40)
                Layout.leftMargin: actualX(40)
                Layout.rightMargin: actualX(40)
                Layout.topMargin: actualY(20)
                Layout.fillWidth:  true
                font.pixelSize: actualX(32)
                color: "#333333"
                horizontalAlignment: Text.AlignLeft
            }
            TextInput{
                id: pageServerUrl
                clip: true
                text : settings.pageServerUrl
                height: actualY(40)
                Layout.leftMargin: actualX(40)
                Layout.rightMargin: actualX(40)
                Layout.topMargin: actualY(20)
                Layout.fillWidth:  true
                font.pixelSize: actualX(32)
                color: "#333333"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                wrapMode: TextInput.WrapAnywhere
            }
            Rectangle{
                height: 1
                color: "#333333"
                Layout.topMargin: actualY(20)
                Layout.leftMargin: actualX(40)
                Layout.rightMargin: actualX(40)
                Layout.fillWidth: true
            }
            Label {
                id: dataServerUrlLabel
                text: qsTr("数据服务器地址:")
                height: actualY(40)
                Layout.leftMargin: actualX(40)
                Layout.rightMargin: actualX(40)
                Layout.topMargin: actualY(20)
                Layout.fillWidth:  true
                font.pixelSize: actualX(32)
                color: "#333333"
                horizontalAlignment: Text.AlignLeft
            }
            TextInput{
                id: dataServerUrl
                clip: true
                text : settings.dataServerUrl
                height: actualY(40)
                Layout.leftMargin: actualX(40)
                Layout.rightMargin: actualX(40)
                Layout.topMargin: actualY(20)
                Layout.fillWidth:  true
                font.pixelSize: actualX(32)
                color: "#333333"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                wrapMode: TextInput.WrapAnywhere
            }
            RowLayout{
                id:settingButtons
                spacing: actualX(40)
                Layout.leftMargin: actualX(20)
                Layout.rightMargin: actualX(20)
                Layout.topMargin: actualY(40)
                Layout.bottomMargin: actualY(20)
                Rectangle{
                    Layout.fillWidth: true
                    height: actualY(85)
                    radius: actualY(85)
                    color: "#045699"
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: actualX(36)
                        color: "#ffffff"
                        text: qsTr("确认")
                    }
                    MouseArea{
                        anchors.fill: parent
                        onReleased: {
                            settings.isPageAndDataSame = isPageAndDataSame.checked
                            if(isPageAndDataSame.checked){
                                settings.pageServerUrl = autoFixUrl(pageServerUrl.displayText);
                                settings.dataServerUrl = autoFixUrl(pageServerUrl.displayText);
                            }else{
                                settings.pageServerUrl = autoFixUrl(pageServerUrl.displayText);
                                settings.dataServerUrl = autoFixUrl(dataServerUrl.displayText);
                            }
                            settingsDialog.close();
                            naviPage()
                        }
                    }
                }

                Rectangle{
                    Layout.fillWidth: true
                    height: actualY(85)
                    radius: actualY(85)
                    color: "#dcdddd"
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: actualX(36)
                        color: "#333333"
                        text: qsTr("取消")
                    }
                    MouseArea{
                        anchors.fill: parent
                        onReleased: {
                            settingsDialog.close();
                        }
                    }
                }
            }//end of settingButtons RowLayout
        }//end of ColumnLayout
    }//end of settings dialog

    Popup {
        id: accountDialog
        x: Math.round((rootWindow.width - width) / 2)
        y: Math.round(rootWindow.height / 6)
        width: actualX(620)
        contentHeight: accountColumn.height
        modal: true
        focus: true
        background: Rectangle {
            anchors.fill: parent
            border.width: 0
            color: "#ffffff"
            radius: actualX(30)
        }
        onOpened: {
            //////for testing
            //usrName.text = settings.usrName
            //usrPswd.text = settings.usrPswd
            //////end testing
        }
        ColumnLayout {
            id: accountColumn
            x: actualX(-12)
            y: actualY(-12)
            width: actualX(620)
            spacing: 0
            Text{
                height: actualY(40)
                color: "#333333"
                font.pixelSize: actualX(36)
                text: qsTr("用户登录")
                horizontalAlignment: Text.AlignHCenter
                Layout.topMargin: actualY(40)
                Layout.fillWidth: true
            }
            Rectangle{
                height: actualY(85)
                radius: actualY(85)
                Layout.topMargin: actualY(40)
                Layout.leftMargin: actualX(20)
                Layout.rightMargin: actualX(20)
                Layout.fillWidth: true
                border.color: "#333333"
                border.width: 1
                clip: true
                TextInput{
                    id: usrName
                    color: "#333333"
                    font.pixelSize: actualX(32)
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                    anchors.leftMargin: actualX(40)
                    anchors.right:parent.right
                    anchors.rightMargin: actualX(80)
                    anchors.verticalCenter: parent.verticalCenter
                    wrapMode: TextInput.NoWrap
                    Label {
                        visible: usrName.text===""
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("请输入用户名")
                        color: "#999999"
                        font.pixelSize: actualX(25)
                    }
                }
                Image {
                    anchors.right: parent.right
                    anchors.rightMargin: actualX(40)
                    anchors.verticalCenter: parent.verticalCenter
                    height: parent.height * 0.5
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/images/icons/usrNameTip.png"
                }
            }//end of Rectangle usrName
            Rectangle{
                height: actualY(85)
                radius: actualY(85)
                Layout.topMargin: actualY(55)
                Layout.leftMargin: actualX(20)
                Layout.rightMargin: actualX(20)
                Layout.fillWidth: true
                border.color: "#333333"
                border.width: 1
                clip: true
                TextInput{
                    id: usrPswd
                    color: "#333333"
                    font.pixelSize: actualX(32)
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                    anchors.leftMargin: actualX(40)
                    anchors.right:parent.right
                    anchors.rightMargin: actualX(80)
                    anchors.verticalCenter: parent.verticalCenter
                    wrapMode: TextInput.NoWrap
                    echoMode: TextInput.PasswordEchoOnEdit
                    Label {
                        visible: usrPswd.text===""
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("请输入密码")
                        color: "#999999"
                        font.pixelSize: actualX(25)
                    }
                }
                Image {
                    anchors.right: parent.right
                    anchors.rightMargin: actualX(40)
                    anchors.verticalCenter: parent.verticalCenter
                    height: parent.height * 0.5
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/images/icons/pswdTip.png"
                }
            }//end of Rectangle usrPswd
            RowLayout {
                height: actualY(40)
                Layout.leftMargin: actualX(20)
                Layout.topMargin: actualY(40)
                CheckBox{
                    id: isAutoLogInFor15Days
                    Layout.fillHeight: true
                    Component.onCompleted: {
                        //initial the form
                        checked = settings.isPageAndDataSame
                        onCheckedChanged()
                    }
                }
                Label {
                    text: qsTr("自动登录(15天)")
                    Layout.fillHeight: true
                    font.pixelSize: actualX(32)
                    color: "#333333"
                    horizontalAlignment: Text.AlignLeft
                }
            }
            RowLayout{
                id:accountButtons
                spacing: actualX(40)
                Layout.leftMargin: actualX(20)
                Layout.rightMargin: actualX(20)
                Layout.topMargin: actualY(40)
                Layout.bottomMargin: actualY(20)
                Rectangle{
                    Layout.fillWidth: true
                    height: actualY(85)
                    radius: actualY(85)
                    color: "#045699"
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: actualX(36)
                        color: "#ffffff"
                        text: qsTr("确认")
                    }
                    MouseArea{
                        anchors.fill: parent
                        onReleased: {
                            settings.usrName = usrName.text;
                            if(settings.usrPswd != usrPswd.text){
                                settings.usrPswd = Qt.md5( Qt.md5(  Qt.md5(usrPswd.text)+usrName.text ) + "1234" );
                            }
                            logIn();
                            accountDialog.close()
                        }
                    }
                }

                Rectangle{
                    Layout.fillWidth: true
                    height: actualY(85)
                    radius: actualY(85)
                    color: "#dcdddd"
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: actualX(36)
                        color: "#333333"
                        text: qsTr("取消")
                    }
                    MouseArea{
                        anchors.fill: parent
                        onReleased: {
                            accountDialog.close()
                        }
                    }
                }
            }//end of accountButtons RowLayout
        }//end of accountColumn
    }//end of dialog account
}
