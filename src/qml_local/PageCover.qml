import QtQuick 2.2
import QtQuick.Controls 2.1
Popup{
    id: coverRoot
    signal finished

    SwipeView{
        id : coverView
        currentIndex: 0
        anchors.fill: parent

        onCurrentIndexChanged: {
            if(currentIndex === 5)
                coverRoot.finished()
        }

        Item{
            Image{
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source:"qrc:/images/opening/1.png"
                smooth: true
            }
        }
        Item{
            Image{
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source:"qrc:/images/opening/2.png"
                smooth: true
            }
        }
        Item{
            Image{
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source:"qrc:/images/opening/3.png"
                smooth: true
            }
        }
        Item{
            Image{
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source:"qrc:/images/opening/4.png"
                smooth: true
            }
        }
        Item{
            Image{
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source:"qrc:/images/opening/5.png"
                smooth: true
            }
        }
        Item{
            opacity: 0
        }

    }//end of swipview
}//end of popup
