import QtQuick 2.7
import QtQuick.Extras 1.4
import "../../qml/modules"

/*****************************************mimic standard v0.1
//import "../modules"
//Rectangle{
//  id:pageRoot
//  color: "transparent"
//  property alias implicitHeight: bg.sourceSize.width
//  property alias implicitWidth:  bg.sourceSize.height
//  Component.onCompleted: {}
//  Component.onDestruction: {}
//  function relatedX( x ){
//       return bg.width * ( x / bg.sourceSize.width)
//  }
//  function relatedY( y ){
//      return bg.height* ( y /bg.sourceSize.height)
//  }
//  function relatedW( w ){
//      return relatedX( w )
//  }
//  function relatedH( h ){
//      return relatedY( h )
//  }
//  Image{
//      id:bg
//      x: 0
//      y: 0
//      width: pageRoot.width
//      height: pageRoot.height
//      fillMode: Image.PreserveAspectFit
//      source: "xxxxxxxxxxx.png"
//  }
//}
*************************************************************/
Rectangle{
    id:pageRoot
    color: "transparent"

    property alias implicitHeight: bg.sourceSize.width
    property alias implicitWidth:  bg.sourceSize.height

    Component.onCompleted: {

    }
    Component.onDestruction: {

    }

    //transfer source picture pos & size into actual picture pos & size
    function relatedX( x ){
        return bg.width * ( x / bg.sourceSize.width)
    }
    function relatedY( y ){
        return bg.height* ( y /bg.sourceSize.height)
    }
    function relatedW( w ){
        return relatedX( w )
    }
    function relatedH( h ){
        return relatedY( h )
    }
    Image{
        id:bg
        width: pageRoot.width
        x: 0
        y: 0
        fillMode: Image.PreserveAspectFit
        source: "qrc:/project_2017001_JingGang1Hao/img/no3.png"
    }
    Text{
        id:gensetLoad
        x:relatedX(155)
        y:relatedY(855)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x0200000B)
        text: dataItem.tagValue.toFixed(2)
    }
    Text{
        id:gensetFreq
        x:relatedX(155)
        y:relatedY(995)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x0200000A)
        text: dataItem.tagValue.toFixed(0)
    }
    Text{
        id:gensetVoltage
        x:relatedX(155)
        y:relatedY(1140)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x0200011E)
        text: dataItem.tagValue.toFixed(1)
    }

    Text{
        id:gensetCoolingWaterTemp
        x:relatedX(1043)
        y:relatedY(1330)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x0200000F)
        text: dataItem.tagValue.toFixed(1)
    }
    Gauge {
        id:gensetCoolingWaterTempGauge
        x:relatedX(1043)
        y:relatedY(827)
        width:  relatedW(259)
        height: relatedH(364)
        maximumValue: 100
        minimumValue: 0
        tickmarkStepSize: 20
        font.pixelSize: bg.width * 0.01

        property var dataItem : dataSource.getValue(0x0200000F)
        value: dataItem.tagValue
    }
    Text{
        id:gensetLubOilPress
        x:relatedX(1394)
        y:relatedY(1330)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: {'tagValue' : 0.00 }
        text: dataItem.tagValue.toFixed(2)
    }
    Gauge {
        id:gensetLubOilPressGauge
        x:relatedX(1395)
        y:relatedY(827)
        width:  relatedW(259)
        height: relatedH(364)
        maximumValue: 10
        minimumValue: 0
        tickmarkStepSize: 2
        font.pixelSize: bg.width * 0.01

        property var dataItem: {'tagValue' : 0.00 }
        value: dataItem.tagValue
    }
    Text{
        id:gensetTurboPress
        x:relatedX(1744)
        y:relatedY(1330)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: {'tagValue' : 0.00 }
        text: dataItem.tagValue.toFixed(2)
    }
    Gauge {
        id:gensetTurboPressGauge
        x:relatedX(1745)
        y:relatedY(827)
        width:  relatedW(259)
        height: relatedH(364)
        maximumValue: 10
        minimumValue: 0
        tickmarkStepSize: 2
        font.pixelSize: bg.width * 0.01

        property var dataItem: {'tagValue' : 0.00 }
        value: dataItem.tagValue
    }
    Text{
        id:gensetFOPress
        x:relatedX(2095)
        y:relatedY(1330)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: {'tagValue' : 0.00 }
        text: dataItem.tagValue.toFixed(2)
    }
    Gauge {
        id:gensetFOPressGauge
        x:relatedX(2096)
        y:relatedY(827)
        width:  relatedW(259)
        height: relatedH(364)
        maximumValue: 10
        minimumValue: 0
        tickmarkStepSize: 2
        font.pixelSize: bg.width * 0.01

        property var dataItem: {'tagValue' : 0.00 }
        value: dataItem.tagValue
    }
    Text{
        id:gensetFOTemp
        x:relatedX(2446)
        y:relatedY(1330)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: {'tagValue' : 0.00 }
        text: dataItem.tagValue.toFixed(2)
    }
    Gauge {
        id:gensetFOTempGauge
        x:relatedX(2446)
        y:relatedY(827)
        width:  relatedW(259)
        height: relatedH(364)
        maximumValue: 100
        minimumValue: 0
        tickmarkStepSize: 20
        font.pixelSize: bg.width * 0.01

        property var dataItem: {'tagValue' : 0.00 }
        value: dataItem.tagValue
    }
}
