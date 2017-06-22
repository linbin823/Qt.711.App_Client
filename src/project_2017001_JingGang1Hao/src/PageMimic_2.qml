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
        source: "qrc:/project_2017001_JingGang1Hao/img/no2.png"
    }
    ModuleLights{
        id: steerGB_OilTank_TAH
        lightOnColor: "red"
        lightOffColor: Qt.darker(lightOnColor, 3.0)
        x:relatedX(422)
        y:relatedY(308)
        width:  relatedW(35)
        height: width
        property var dataItem: { 'tagValue' : false}
        lightState: dataItem.tagValue
    }
    ModuleLights{
        id: steerGB_OilTank_LAL
        lightOnColor: "red"
        lightOffColor: Qt.darker(lightOnColor, 3.0)
        x:relatedX(422)
        y:relatedY(426)
        width:  relatedW(35)
        height: width
        property var dataItem: { 'tagValue' : false}
        lightState: dataItem.tagValue
    }

    ModuleLights{
        id: steerGB_ExpansionTank_TAH
        lightOnColor: "red"
        lightOffColor: Qt.darker(lightOnColor, 3.0)
        x:relatedX(650)
        y:relatedY(308)
        width:  relatedW(35)
        height: width
        property var dataItem: { 'tagValue' : false}
        lightState: dataItem.tagValue
    }
    ModuleLights{
        id: steerGB_ExpansionTank_PAL
        lightOnColor: "red"
        lightOffColor: Qt.darker(lightOnColor, 3.0)
        x:relatedX(650)
        y:relatedY(368)
        width:  relatedW(35)
        height: width
        property var dataItem: { 'tagValue' : false}
        lightState: dataItem.tagValue
    }
    ModuleLights{
        id: steerGB_ExpansionTank_LAL
        lightOnColor: "red"
        lightOffColor: Qt.darker(lightOnColor, 3.0)
        x:relatedX(650)
        y:relatedY(426)
        width:  relatedW(35)
        height: width
        property var dataItem: { 'tagValue' : false}
        lightState: dataItem.tagValue
    }

    ModuleLights{
        id: steerGB_GraviyTank_TAH
        lightOnColor: "red"
        lightOffColor: Qt.darker(lightOnColor, 3.0)
        x:relatedX(968)
        y:relatedY(310)
        width:  relatedW(35)
        height: width
        property var dataItem: { 'tagValue' : false}
        lightState: dataItem.tagValue
    }
    ModuleLights{
        id: steerGB_GraviyTank_PAL
        lightOnColor: "red"
        lightOffColor: Qt.darker(lightOnColor, 3.0)
        x:relatedX(968)
        y:relatedY(368)
        width:  relatedW(35)
        height: width
        property var dataItem: { 'tagValue' : false}
        lightState: dataItem.tagValue
    }
    ModuleLights{
        id: steerGB_GraviyTank_LAL
        lightOnColor: "red"
        lightOffColor: Qt.darker(lightOnColor, 3.0)
        x:relatedX(968)
        y:relatedY(428)
        width:  relatedW(35)
        height: width
        property var dataItem: { 'tagValue' : false}
        lightState: dataItem.tagValue
    }

    ModuleLights{
        id: steerGB_HydraulicPump2_run
        lightOnColor: "green"
        lightOffColor: "red"
        opacity:  0.3
        x:relatedX(1121)
        y:relatedY(843)
        width:  relatedW(82)
        height: width
        property var dataItem: { 'tagValue' : false}
        lightState: dataItem.tagValue
    }
    ModuleLights{
        id: steerGB_HydraulicPump1_run
        lightOnColor: "green"
        lightOffColor: "red"
        opacity:  0.3
        x:relatedX(1312)
        y:relatedY(843)
        width:  relatedW(82)
        height: width
        property var dataItem: { 'tagValue' : false}
        lightState: dataItem.tagValue
    }

    ModuleLights{
        id: steerGB_LubOil_TAH
        lightOnColor: "red"
        lightOffColor: Qt.darker(lightOnColor, 3.0)
        x:relatedX(422)
        y:relatedY(810)
        width:  relatedW(35)
        height: width
        property var dataItem: { 'tagValue' : false}
        lightState: dataItem.tagValue
    }
    ModuleLights{
        id: steerGB_LubOil_PAL
        lightOnColor: "red"
        lightOffColor: Qt.darker(lightOnColor, 3.0)
        x:relatedX(422)
        y:relatedY(927)
        width:  relatedW(35)
        height: width
        property var dataItem: { 'tagValue' : false}
        lightState: dataItem.tagValue
    }

    ModuleLights{
        id: steerGB_LubOilPump2_run
        lightOnColor: "green"
        lightOffColor: "red"
        opacity:  0.3
        x:relatedX(343)
        y:relatedY(1257)
        width:  relatedW(82)
        height: width
        property var dataItem: { 'tagValue' : false}
        lightState: dataItem.tagValue
    }
    ModuleLights{
        id: steerGB_LubOilPump1_run
        lightOnColor: "green"
        lightOffColor: "red"
        opacity:  0.3
        x:relatedX(534)
        y:relatedY(1257)
        width:  relatedW(82)
        height: width
        property var dataItem: { 'tagValue' : false}
        lightState:  dataItem.tagValue
    }

    ModuleLights{
        id: steerGB_SteerPump_run
        lightOnColor: "green"
        lightOffColor: "red"
        opacity:  0.3
        x:relatedX(2230)
        y:relatedY(793)
        width:  relatedW(125)
        height: width
        property var dataItem: { 'tagValue' : false}
        lightState: dataItem.tagValue
    }
    Text{
        id:tempU
        x:relatedX(2570)
        y:relatedY(307)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x0200015C)
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:tempV
        x:relatedX(2570)
        y:relatedY(467)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x0200015D)
        text: dataItem.tagValue.toFixed(1)
    }
    Text{
        id:tempW
        x:relatedX(2570)
        y:relatedY(631)
        width:  relatedW(259)
        height: relatedH(90)
        font.pixelSize: bg.height * 0.06
        color: "white"

        property var dataItem: dataSource.getValue(0x0200015E)
        text: dataItem.tagValue.toFixed(1)
    }
}
