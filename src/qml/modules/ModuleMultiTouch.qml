import QtQuick 2.7

MultiPointTouchArea {
    id: root
    signal clicked
    signal pressed
    signal released
    signal doubleClicked
    signal updateChanging

    property real addRatio: 1.0
    property real addOffsetX: 0
    property real addOffsetY: 0

    property int  state

    property real originX
    property real originY

    property real doubleTapRatio: 2.0

    property var  enumStates : {"stop":0, "moving":1, "ratioing":2, "doubleTapRatioing":3}

    property alias p1: point1
    property alias p2: point2
    Item{
        id: __
        property real lastXPos
        property real lastYPos
        property real lastDistance

        property real beginLength
        property real beginPosX
        property real beginPosY

        property real ratioAddStep

        property real firstTapX
        property real firstTapY
        property real threshold
    }

    function controlledItemUpdateChanging(targetItem){
        targetItem.x += addOffsetX
        targetItem.y += addOffsetY

        //scale origin: TopLeft
        targetItem.x = targetItem.x - ( originX - targetItem.x ) * (addRatio - 1)
        targetItem.y = targetItem.y - ( originY - targetItem.y ) * (addRatio - 1)
        targetItem.scale  *= addRatio
    }

    Component.onCompleted:{
        state = enumStates.stop

        __.threshold = 10//(root.width + root.height ) / 20
    }

    touchPoints:[
        TouchPoint{id:point1},
        TouchPoint{id:point2}
    ]


    onTouchUpdated:{
        switch(touchPoints.length){
        case 0:
            //exit ratioing
            //exit moving
            if( state === enumStates.moving || state === enumStates.ratioing )
                state = enumStates.stop
            break
        case 1:
            if(state === enumStates.ratioing){
                //exit ratioing
                state = enumStates.stop
            }
            else if(state === enumStates.moving){
                //continue moving
                addOffsetX = p1.x - __.lastXPos
                addOffsetY = p1.y - __.lastYPos
                addRatio = 1.0
                __.lastXPos = p1.x
                __.lastYPos = p1.y
                updateChanging()
            }
            else if(state === enumStates.stop){
                //start doubleTapRatioing
                if(tapDelay.running && !tapJitterFilter.running &&
                        ( (Math.abs(point1.x - __.firstTapX) + Math.abs(point1.y - __.firstTapY)) < __.threshold ) )
                {
                    state = enumStates.doubleTapRatioing
                    originX = point1.x
                    originY = point1.y
                    __.ratioAddStep = Math.pow( doubleTapRatio, 1/25)
                    zoomInWorker.start()
                    doubleTapRatioingTimer.start()
                    return
                }
                //start tap delay
                __.firstTapX = point1.x
                __.firstTapY = point1.y
                tapDelay.start()
                tapJitterFilter.start()
                //start moving
                __.lastXPos = p1.x
                __.lastYPos = p1.y
                addOffsetX = 0
                addOffsetY = 0
                addRatio = 1.0
                state = enumStates.moving
            }
            break
        case 2:
            var distance = dist()
            if(state === enumStates.ratioing){
                //continue ratioing
                addRatio = distance / __.lastDistance
                __.lastDistance = distance
                addOffsetX = 0
                addOffsetY = 0
                updateChanging()
            }
            else{
                //start ratioing
                if( distance <= __.threshold)
                    return
                __.lastDistance =  distance
                originX = ( point1.x + point2.x ) / 2
                originY = ( point1.y + point2.y ) / 2
                state = enumStates.ratioing
                addOffsetX = 0
                addOffsetY = 0
                addRatio = 1.0
                updateChanging()
            }
            break
        }
    }

    function dist(){
        return Math.sqrt(Math.pow( (point1.x - point2.x),2) + Math.pow( (point1.y - point2.y), 2) )
    }

    Timer{
        id: tapDelay
        interval: 800
        repeat: false
    }
    Timer{
        id: tapJitterFilter
        interval: 200
        repeat: false
    }

    Timer{
        id: doubleTapRatioingTimer
        interval: 500
        repeat: false
        onTriggered:  {
            //exit doubleTapRatioing
            state = enumStates.stop
            zoomInWorker.stop()
        }
    }
    Timer{
        id: zoomInWorker
        interval: 20
        repeat: true
        onTriggered: {
            //continue doubleTapRatioing
            addRatio = __.ratioAddStep
            addOffsetX = 0
            addOffsetY = 0
            updateChanging()
        }
    }

    onStateChanged: {
        if(state === enumStates.stop){
            released()
            clicked()
        }
        else if(state === enumStates.moving){
            pressed()
        }
        else if(state === enumStates.ratioing){
            doubleClicked()
        }
    }

}
