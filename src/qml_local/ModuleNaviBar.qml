import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3

Item {
    signal keyPressed(string target)

    function setActiveButton(target) {
        if(target === "home_page"){
            pbHome.isActive =true;
            pbGIS.isActive = false;
            pbData.isActive =false;
            pbUtility.isActive = false;
        }
        else if(target === "gis_page"){
            pbHome.isActive =false;
            pbGIS.isActive = true;
            pbData.isActive =false;
            pbUtility.isActive = false;
        }
        else if(target === "data_page"){
            pbHome.isActive =false;
            pbGIS.isActive = false;
            pbData.isActive =true;
            pbUtility.isActive = false;
        }
        else if(target === "utility_page"){
            pbHome.isActive =false;
            pbGIS.isActive = false;
            pbData.isActive =false;
            pbUtility.isActive = true;
        }
    }

    Rectangle {
        color: "white"
        border.color: "grey"
        border.width: 1
        width: parent.width
        height: parent.height
        anchors.bottom:  parent.bottom
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Button {
            id: pbHome
            property bool isActive :true
            Layout.fillWidth: true
            Layout.fillHeight: true
            flat:true
            background.opacity: 0
            Image {
                source: pbHome.isActive ? "qrc:/img/HOME2.png" : "qrc:/img/HOME1.png"
                width: parent.width
                height: parent.height * 0.85
                fillMode: Image.PreserveAspectFit
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            onClicked: {
                keyPressed("home_page");
            }
        }
        Button {
            id: pbGIS
            property bool isActive :false
            Layout.fillWidth: true
            Layout.fillHeight: true
            flat:true
            background.opacity: 0
            Image {
                source: pbGIS.isActive ? "qrc:/img/GIS2.png" : "qrc:/img/GIS1.png"
                width: parent.width
                height: parent.height * 0.85
                fillMode: Image.PreserveAspectFit
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            onClicked: {
                keyPressed("gis_page");
            }
        }
        Button {
            id: pbData
            property bool isActive :false
            Layout.fillWidth: true
            Layout.fillHeight: true
            flat:true
            background.opacity: 0
            Image {
                source: pbData.isActive ? "qrc:/img/DATA2.png" : "qrc:/img/DATA1.png"
                width: parent.width
                height: parent.height * 0.85
                fillMode: Image.PreserveAspectFit
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            onClicked: {
                keyPressed("data_page");
            }
        }
        Button {
            id:pbUtility
            property bool isActive :false
            Layout.fillWidth: true
            Layout.fillHeight: true
            flat:true
            background.opacity: 0
            Image {
                source: pbUtility.isActive ? "qrc:/img/ME2.png" : "qrc:/img/ME1.png"
                width: parent.width
                height: parent.height * 0.85
                fillMode: Image.PreserveAspectFit
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            onClicked: {
                keyPressed("utility_page");
            }
        }
    }
}
