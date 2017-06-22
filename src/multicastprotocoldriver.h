#ifndef MULTICASTPROTOCOLDRIVER_H
#define MULTICASTPROTOCOLDRIVER_H

#include <QObject>
#include <QHostAddress>
#include <QTimer>
#include "taginfo.h"


QT_BEGIN_NAMESPACE
class QUdpSocket;
QT_END_NAMESPACE

class MulticastProtocolDriver : public QObject
{
    Q_OBJECT

enum enum_states{
    STAT_RUN,
    STAT_STOP,
    STAT_ERR
};

public:
    explicit MulticastProtocolDriver(QObject *parent = 0);

signals:
    void stateChanged(int);

public slots:

    int start();
    int stop();
    int restart();
    int getState(){
        return _state;
    }

    void setGroupAddress(const QHostAddress& addr, int port);
    void setAllTags(QList<TagInfo *> *pAll);
    void tagRWSChanged(TagInfo*one){ Q_UNUSED(one) }

private slots:
    void _processPendingDatagrams();
    void _setState(enum_states state);
    void _watchDogTimeOver();
    bool _getTime(QDateTime& t, int year, int month, int day, int hour, int minute, int second, int millisecond = 0);

private:
    QUdpSocket * _pUdpSocket;
    QHostAddress _groupAddress;
    enum_states _state;
    int _port;

    QTimer *_pWatchDogTimer;
    QList<TagInfo*>* _pAllTags;
};

#endif // MULTICASTPROTOCOLDRIVER_H
