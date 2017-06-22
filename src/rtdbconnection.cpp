#include <QGuiApplication>
#include <QEvent>
#include <QTime>
#include <QByteArray>
#include "rtdbconnection.h"
#include "utility.h"

#define SERVER_ADDR "csic711.net"
#define SERVER_PORT 1657
#define CLIENT_PORT 1658

quint8 RTDBconnection::_clientID;

RTDBconnection::RTDBconnection(QObject *parent) : QObject(parent)
{
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    _clientID = (quint8)qrand();

    _socket = nullptr;

    socketState = false;//not ready
    connectState = false;//not ready

    _watchDogTimer = new QTimer(this);
    _watchDogTimer->setSingleShot(true);//triger once
    _watchDogTimer->setInterval(30*1000);//30sec timeover
    connect(_watchDogTimer,SIGNAL(timeout()),this,SLOT(_watchDogTimeOverProcess()) );

    _heartBeatsTimer = new QTimer(this);
    _heartBeatsTimer->setSingleShot(false);
    _heartBeatsTimer->setInterval(5*1000);//5sec heartBeat sending
    connect(_heartBeatsTimer,SIGNAL(timeout()),this,SLOT(_heartBeatsProcess()) );

    _heartBeatsProcess();

    QHostInfo::lookupHost(SERVER_ADDR, this, SLOT(_lookedUp(QHostInfo)));
}

RTDBconnection::~RTDBconnection(){
    if(_socket != nullptr){
        _socket->deleteLater();
    }
    _watchDogTimer->deleteLater();
    _heartBeatsTimer->deleteLater();
}

bool RTDBconnection::eventFilter(QObject *obj, QEvent *event){
    if (event->type() == QEvent::ApplicationStateChange){
        if (QGuiApplication::applicationState() == Qt::ApplicationActive){
            //start heartbeats and watchdog
            _heartBeatsTimer->start();
            _watchDogTimer->start();
            qDebug()<<"Active";
        }
        else if (QGuiApplication::applicationState() == Qt::ApplicationHidden){
            qDebug()<<"Hidden";
        }
        else if (QGuiApplication::applicationState() == Qt::ApplicationInactive){
            //stop heatbeats and watchdog
            _heartBeatsTimer->stop();
            _watchDogTimer->stop();
            connectState =false;//should re-subscrib
            qDebug()<<"Inactive";
        }
        else if (QGuiApplication::applicationState() == Qt::ApplicationSuspended){
            socketState =false;//should re-initial socket
            qDebug()<<"Suspended";
        }
    }
    return false;
}

void RTDBconnection::_initialSocket(){
    socketState = false;
    if(_socket != nullptr){
        disconnect(_socket, SIGNAL(readyRead()), this, SLOT(_receiveProcess()) );
        _socket->deleteLater();
    }
    _socket = new QUdpSocket(this);

    if(_socket->bind(QHostAddress::AnyIPv4, CLIENT_PORT ,QUdpSocket::ShareAddress) ){
        connect(_socket, SIGNAL(readyRead()), this, SLOT(_receiveProcess()) );
        socketState = true;
    }
}

QByteArray RTDBconnection::_packHeartBeats(){
    QByteArray data;
    data.resize(7);
    data[0] = 0x53;
    data[1] = 0x01;
    data[2] = _clientID;
    data[3] = 0x00;
    data[4] = 0x00;
    data[5] = daiMaHe( (unsigned char*)(data.data() +1), data.length()-3 );
    data[6] = 0x0d;

    return data;
}

QByteArray RTDBconnection::_packSubsribe(const QList<quint32> &tagIDList){
    int dlen = tagIDList.size()*4;
    quint32 temp;

    QByteArray data;
    data.resize(7+dlen);
    data[0] = 0x53;
    data[1] = 0x12;
    data[2] = _clientID;
    data[3] = (unsigned char)(dlen >> 8);
    data[4] = (unsigned char)(dlen);
    for(int i=0; i< tagIDList.size(); i++){
        temp = tagIDList.at(i);
        data[8+4*i] = (unsigned char)temp;
        temp = temp >> 8;
        data[7+4*i] = (unsigned char)temp;
        temp = temp >> 8;
        data[6+4*i] = (unsigned char)temp;
        temp = temp >> 8;
        data[5+4*i] = (unsigned char)temp;
    }
    data[7 + dlen-2] = daiMaHe( (unsigned char*)(data.data() +1), data.length()-3 );
    data[7 + dlen-1] = 0x0d;

    return data;
}

QByteArray RTDBconnection::_packRefresh(){
    QByteArray data;
    data.resize(7);
    data[0] = 0x53;
    data[1] = 0x13;
    data[2] = _clientID;
    data[3] = 0;
    data[4] = 0;
    data[5] = daiMaHe( (unsigned char*)(data.data() +1), data.length()-3 );
    data[6] = 0x0d;
    return data;
}

void RTDBconnection::_unpackHeartBeats(){
    _watchDogTimer->start();
    connectState = true;
    lastCounnection = QTime::currentTime();
}

//tagID   len     data
//4       1       len字节
void RTDBconnection::_unpackData(QByteArray& data){
    quint32 tagID;
    quint8 tagLen;
    QByteArray tagValue;
    while(!data.isEmpty() ){
        tagID = data[0]<<24;
        tagID += data[1]<<16;
        tagID += data[2]<<8;
        tagID += data[3];

        tagLen = data[4];

        tagValue = data.mid(5,tagLen);



        data.remove(0,tagLen + 5);
    }
}

void RTDBconnection::_heartBeatsProcess(){
    QByteArray data;
    if(socketState == false){
        _initialSocket();
    }
    if(socketState){
        //socket OK!
        //but connection notOK! subsribe
        if(connectState == false){
            data = _packSubsribe();
            qint64 ret = _socket->writeDatagram(data, data.length(), _serverAddr, SERVER_PORT);
            if(ret <=0) return;

            qtSleep(100);

            data = _packRefresh();
            ret = _socket->writeDatagram(data, data.length(), _serverAddr, SERVER_PORT);
            if(ret <=0) return;
        }
        else{
            //connection OK! send hart beats
            data = _packHeartBeats();
            _socket->writeDatagram(data, data.length(), _serverAddr, SERVER_PORT);
        }

    }
}

void RTDBconnection::_watchDogTimeOverProcess(){
    connectState = false;
}

void RTDBconnection::_lookedUp(QHostInfo host){
    if (host.error() != QHostInfo::NoError)
    {
         qDebug()<< "Lookup failed:" << host.errorString();
         return;
    }
    _serverAddr = host.addresses()[0];
}

void RTDBconnection::_receiveProcess(){
    QByteArray data;
    QHostAddress serverAddr;
    quint16 serverPort;
    _socket->readDatagram(data.data(),1024,&serverAddr,&serverPort);
    if(serverAddr != _serverAddr) return;

    //不是自己的数据不解析
    if( (0x53 != data[0]) | (_clientID != data[2]) ) return;

    //和校验check
    unsigned char sum = daiMaHe( (unsigned char*)(data.data()+1), data.length()-3 );
    if (sum != data[data.length() - 2]) return;

    if( 0x14 == data[1] ){
        //data value update;
        int actLen = data[3];
        actLen = (actLen<<8) + data[4];
        if(actLen != data.length()-7 ) return;
        data.remove(0,5).chop(2);
        _unpackData( data );
    }

    else if( 0x01 == data[1] ){
        //server heart beats
        _unpackHeartBeats();
    }

}
