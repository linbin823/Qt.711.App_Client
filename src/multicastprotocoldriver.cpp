#include <QtNetwork>
#include <QDateTime>
#include "multicastprotocoldriver.h"
#include "loadsaveprocessorxml.h"

MulticastProtocolDriver::MulticastProtocolDriver(QObject *parent) : QObject(parent)
{
    _pUdpSocket = nullptr;
    QString address;
    int port;
    loadSaveProcessorXml* _pXmlProcessor = new loadSaveProcessorXml(this,false);
    _pXmlProcessor->loadFile("settings/address.xml");
    _pXmlProcessor->readValue("groupAddress",address);
    _pXmlProcessor->readValue("groupPort",port);
    delete _pXmlProcessor;
    if(QHostAddress(address).isMulticast()){
        //saved address
        _groupAddress = QHostAddress(address);
        _port = port;
    }else{
        //create an default address
        setGroupAddress(QHostAddress("225.11.1.2") , 41002);
    }

    _pWatchDogTimer = new QTimer(this);
    _pWatchDogTimer->setSingleShot(true);
    _pWatchDogTimer->setInterval(10 * 1000);
    connect(_pWatchDogTimer,SIGNAL(timeout()),this,SLOT(_watchDogTimeOver()));

    _setState(STAT_STOP);
}

void MulticastProtocolDriver::_setState(enum_states state){
    if(_state != state){
        _state = state;
        emit stateChanged(_state);
    }
}

void MulticastProtocolDriver::_watchDogTimeOver(){
    _setState(STAT_ERR);
}

int MulticastProtocolDriver::start(){
    if(_pUdpSocket != nullptr){
        _pUdpSocket->close();
        _pUdpSocket->deleteLater();
    }
    if( _groupAddress.isMulticast() ){
        _pUdpSocket = new QUdpSocket(this);
        bool ret = _pUdpSocket->bind(QHostAddress::AnyIPv4, _port, QUdpSocket::ShareAddress);
        if(!ret){
            _setState( STAT_ERR );
            qDebug()<<"socket binding failed!"<<_pUdpSocket->errorString();
            return -1;
        }
        QList<QNetworkInterface> allInterfaces = QNetworkInterface::allInterfaces();
        int i=0;
        for(;i<allInterfaces.size();i++){
            if(allInterfaces.at(i).flags().testFlag(QNetworkInterface::CanMulticast) ){
                ret = _pUdpSocket->joinMulticastGroup(_groupAddress);
                if(ret){
                    connect(_pUdpSocket, SIGNAL(readyRead()), this, SLOT(_processPendingDatagrams()));
                    _setState(STAT_RUN);
                    qDebug()<<"joinMulticastGroup successed!"<<allInterfaces.at(i).name();
                    return 0;
                }
                else{
                    qDebug()<<"joinMulticastGroup failed!"<<allInterfaces.at(i).name()<<_pUdpSocket->state();
                    continue;
                }
            }
            qDebug()<<"not support multicast!"<<allInterfaces.at(i).name();
            _setState( STAT_ERR );
            continue;
        }
    }
    return -1;
}

int MulticastProtocolDriver::stop(){
    if(_pUdpSocket != nullptr){
        if( _pUdpSocket->isOpen() ){
            _pUdpSocket->close();
            _setState(STAT_STOP);
        }
    }
    return 0;
}

int MulticastProtocolDriver::restart(){
    stop();
    return start();
}

void MulticastProtocolDriver::setGroupAddress(const QHostAddress& addr, int port){
    if( addr.isMulticast() && addr != _groupAddress ){
        _groupAddress = addr;
        _port = port;
        loadSaveProcessorXml* _pXmlProcessor = new loadSaveProcessorXml(this,false);
        _pXmlProcessor->init();
        _pXmlProcessor->writeValue("groupAddress",_groupAddress);
        _pXmlProcessor->writeValue("groupPort",_port);
        _pXmlProcessor->saveFile("settings/address.xml");
        delete _pXmlProcessor;
        restart();
    }
}


void MulticastProtocolDriver::setAllTags(QList<TagInfo *> *pAll){
    _pAllTags = pAll;
}

void MulticastProtocolDriver::_processPendingDatagrams()
{
    while (_pUdpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_pUdpSocket->pendingDatagramSize());
        int count = _pUdpSocket->readDatagram(datagram.data(), datagram.size());

///extension alarming system protocol
//        int group,offset,num;

//        group	= (datagram[16]&0xff) + ((datagram[15]&0xff) << 8);
//        offset	= (datagram[18]&0xff) + ((datagram[17]&0xff) << 8);
//        num		= (datagram[19]&0xff);

//        if(num > 80)//data num is not right
//            return;

//        if((offset+num)>513)//data length is not right
//            return;

//        int year= (datagram[8]&0xff) + ( (datagram[9]&0xff)<<8 );
//        int month = (datagram[10]&0xff);
//        int day = (datagram[11]&0xff);
//        int hour =(datagram[12]&0xff);
//        int min = (datagram[13]&0xff);
//        int sec = (datagram[14]&0xff);
//        QDateTime updateDateTime;
//        updateDateTime.setDate( QDate(year,month,day) );
//        updateDateTime.setTime( QTime(hour,min,sec) );

//        int tagID;

//        for(int i=0; i<num; i++){
//            tagID = offset + i;
//            foreach(TagInfo*t, *_pAllTags){
//                if(t->tagID() == tagID){
//                    int value = datagram[20 + i*6] + (datagram[20 + i*6 + 1] << 8);
//                    t->updateTagValue(QString::number(value), updateDateTime);
//                }
//            }
//        }

///real time database receiving protocol
        if ((datagram[0] == (char)0x53)
                && (count >= 16)
                && (datagram[count - 1] == (char)0x0D)
                && (datagram[7] * 256 + datagram[6] + 7 == count)){
            //以太网帧校验
            char check = 0;
            for (int i = 8; i < count - 2; i++)
                check = (char)(check + datagram[i]);
            if (check != datagram[count - 2])
            {
                return;
            }

            quint32 ip_i = (quint32)(datagram[1]<<24) + (quint32)(datagram[2]<<16) + (quint32)(datagram[3]<<8) + (quint32)datagram[4];
            QHostAddress ip;
            ip.setAddress(ip_i);//数据来源 IP 地址


            int dt = datagram[5];//数据类型
            int p = 8;
            QDateTime updateDateTime;

            switch (dt)
            {
                //case 0x64://网络确认请求
                //    if (OnComm != null)
                //        OnComm(DateTime.Now, host.ToString(), 0);
                //    break;
                //case 0x65://网络确认响应
                    ////if (OnComm != null)
                    ////    OnComm(DateTime.Now, host.ToString(), 1);
                    //break;
                //case 0x12://网络校时
                //    if (_getTime(updateDateTime, datagram[p] + datagram[p + 1]<<8, datagram[p + 2], datagram[p + 3], datagram[p + 4], datagram[p + 5], datagram[p + 6]))
                //    {
                //        if (OnTime != null)
                //            OnTime(time);
                //    }
                //    break;

                case 0x11://广播数据,老的程序是11H
                    if (_getTime(updateDateTime, datagram[p] + datagram[p + 1] * 100, datagram[p + 2], datagram[p + 3], datagram[p + 4], datagram[p + 5], datagram[p + 6]))
                    {
                        p = p + 7;
                        while ((p + 12) < count)
                        {
                            //unwrap
                            //project id, bit28 -- bit16, together 13 bits. using byte 0(lower 5 bits ) and byte 1(hole bits)
                            int projectID = (((quint8)datagram[p]) << 8) + (quint8)datagram[p + 1];
                            //station id, bit15 -- bit9, together 7 bits(0~127). using byte 2(higher 7 bits)
                            int stationID = ((quint8)datagram[p + 2]) >> 1;
                            //module id, bit8 -- bit5, together 4 bits(according to manual 0~15). using byte2(lower 1bit) and byte 3(higher 3bits)
                            int moduleID  = (((quint8)datagram[p + 2] & 0x01 )<<3 ) + (( (quint8)datagram[p + 3]) >>5 );
                            //point id, bit4 -- bit0, together 5 bits(according to manual 0~31). using byte3(lower 5bits)
                            int pointID   = ((quint8)datagram[p + 3] & 0x1F ) + 1;
                            int value =     ((quint8)datagram[p + 6] << 8) + (quint8)datagram[p + 5];
                            int alarmType = ((quint8)datagram[p + 8] << 8) + (quint8)datagram[p + 7];
                            int limitValue =((quint8)datagram[p + 10] << 8) +(quint8)datagram[p + 9];

                            int tagIDAuto = (projectID << 16) + (stationID << 9) + (moduleID << 5) + pointID;

                            //qDebug()<<Q_FUNC_INFO<<tagIDAuto<<stationID<<moduleID<<pointID<<value;
                            if(stationID == 2 && moduleID == 5 && pointID ==1){
                                qDebug()<<Q_FUNC_INFO<<stationID<<moduleID<<pointID<<"ss";
                            }
//                            if ((value == -32767) && ((alarmType & 0x0008) == 0x0008))
//                                alarmType = (qint16)( (qint32)(alarmType) | 0x1000); //短路，第12位


//                            if ((value == -32767) && ((alarmType & 0x0800) == 0x0800))
//                                alarmType = (qint16)( (qint32)(alarmType) | 0x1000); //短路，第12位

                            TagInfo* t;
                            int lastSpan = (*_pAllTags).size() /2;
                            int pos = (*_pAllTags).size() / 2;
                            while(lastSpan != 0){
                                t = (*_pAllTags).at(pos);
                                if( t->tagIDAuto() == tagIDAuto ){
                                    //found!
                                    t->updateTagValue(QString::number(value), updateDateTime);
                                    break;
                                }else if( t->tagIDAuto() > tagIDAuto ){
                                    lastSpan /= 2;
                                    pos -= lastSpan;
                                }else if( t->tagIDAuto() < tagIDAuto){
                                    lastSpan /= 2;
                                    pos += lastSpan;
                                }
                            }

                            p = p + 12;
                         }
                    }
                    break;

//                case 0x13://车令
//                    if (_getTime(updateDateTime, datagram[p] + datagram[p + 1]<<8, datagram[p + 2], datagram[p + 3], datagram[p + 4], datagram[p + 5], datagram[p + 6]))
//                    {
//                        UInt16 shij = (UInt16)((datagram[p + 10] << 8) + datagram[p + 9]);
//                        Int16 jiasbwz = (Int16)((datagram[p + 12] << 8) + datagram[p + 11]);
//                        Int16 jisbwz = (Int16)((datagram[p + 14] << 8) + datagram[p + 13]);
//                        Int16 luoj = (Int16)((datagram[p + 16] << 8) + datagram[p + 15]);
//                        Int16 zhouxzs = (Int16)((datagram[p + 18] << 8) + datagram[p + 17]);
//                        unsigned char jiasbwzJm = datagram[p + 19];
//                        unsigned char jisbwzJm = datagram[p + 20];
//                        unsigned char luojJm = datagram[p + 21];
//                        unsigned char zhouxzsJm = datagram[p + 22];

//                        RecvCmdBlock cmdBlock = new RecvCmdBlock(time, shij, jiasbwz, jisbwz, luoj, zhouxzs, jiasbwzJm, jisbwzJm, luojJm, zhouxzsJm);
//                        //车令测试，测试完成后删除
//                        if (OnCmd != null)
//                            OnCmd(cmdBlock);
//                    }
//                    break;

//                case 0x20://cep
//                    if (_getTime(updateDateTime, datagram[p] + datagram[p + 1]<<8, datagram[p + 2], datagram[p + 3], datagram[p + 4], datagram[p + 5], datagram[p + 6]))
//                    {
//                        p = p + 7;
//                        while ((p + 16) < count)
//                        {
//                            //时戳
//                            DateTime time1 = DBManager.GetTime((datagram[p + 0] << 24) + (datagram[p + 1] << 16) + (datagram[p + 2] << 8) + datagram[p + 3]);
//                            UInt16 msec = (UInt16)((datagram[p + 4] << 24) + (datagram[p + 5] << 16) + (datagram[p + 6] << 8) + datagram[p + 7]);
//                            int cepno = (int)((datagram[p + 8] << 24) + (datagram[p + 9] << 16) + (datagram[p + 10] << 8) + datagram[p + 11]);
//                            //int cepno = 0xABCD;
//                            Int16 cepvalue = (Int16)((datagram[p + 13] << 8) + datagram[p + 12]);
//                            Int16 cepalarmflag = (Int16)((datagram[p + 15] << 8) + datagram[p + 14]);
//                            RecvCepBlock cepBlock = new RecvCepBlock(time1, msec, cepvalue, cepno,cepalarmflag);
//                            if (OnCep != null)
//                                OnCep(cepBlock);//存cep数据库

//                            RecvDataBlock recvBlock1 = new RecvDataBlock(time1);
//                            recvBlock1.Add(new DataValue(cepno, cepvalue, cepalarmflag,0));
//                            if (OnRecv != null)
//                                OnRecv(recvBlock1);//存data数据库
//                            p = p + 16;
//                        }
//                     }
//                    break;
            }
        }
    }
}


bool MulticastProtocolDriver::_getTime(QDateTime& t, int year, int month, int day, int hour, int minute, int second, int millisecond){
    if(!QDate::isValid(year,month,day))
        return false;
    if(!QTime::isValid(hour,minute,second,millisecond))
        return false;
    t.setDate( QDate(year,month,day) );
    t.setTime( QTime(hour,minute,second,millisecond) );
    return true;
}
