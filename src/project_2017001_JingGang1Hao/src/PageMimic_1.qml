import QtQuick 2.7
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
        x: 0
        y: 0
        width: pageRoot.width
        fillMode: Image.PreserveAspectFit
        source: "qrc:/project_2017001_JingGang1Hao/img/no1.png"
    }
    Text{
        id:busbarAV
        x:relatedX(253)
        y:relatedY(142)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x0200011D)
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:busbarAHz
        x:relatedX(260)
        y:relatedY(252)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: { 'tagValue' : 50.1}
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:g1kW
        x:relatedX(912)
        y:relatedY(142)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x020000ED)
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:g1Current
        x:relatedX(912)
        y:relatedY(252)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: { 'tagValue' : 50.1}
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:g2kW
        x:relatedX(1559)
        y:relatedY(142)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x020000FB)
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:g2Current
        x:relatedX(1559)
        y:relatedY(252)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: { 'tagValue' : 0.1}
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:g3kW
        x:relatedX(2224)
        y:relatedY(142)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x02000109)
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:g3Current
        x:relatedX(2224)
        y:relatedY(252)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: { 'tagValue' : 0.1}
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:busbarBV
        x:relatedX(2629)
        y:relatedY(142)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x0200011E)
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:busbarBHz
        x:relatedX(2629)
        y:relatedY(252)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: { 'tagValue' : 50.1}
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:mt1kw
        x:relatedX(832)
        y:relatedY(1304)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x0200017E)
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:mt1Current
        x:relatedX(832)
        y:relatedY(1416)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x02000181)
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:btkw
        x:relatedX(1476)
        y:relatedY(1304)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x020001C5)
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:btCurrent
        x:relatedX(1476)
        y:relatedY(1416)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x020001C8)
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:mt2kw
        x:relatedX(2174)
        y:relatedY(1304)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x020001FF)
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:mt2Current
        x:relatedX(2174)
        y:relatedY(1416)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x02000202)
        text: dataItem.tagValue.toFixed(1)
    }
}
