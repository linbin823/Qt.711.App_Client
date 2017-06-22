#ifndef RTDBCONNECTION_H
#define RTDBCONNECTION_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QHostInfo>
#include <QUdpSocket>

//real time database connection
//for big data APP of csic711
//created in 2017.02.08 by linbin
class RTDBconnection : public QObject
{
    Q_OBJECT
public:
    explicit RTDBconnection(QObject *parent = 0);
    ~RTDBconnection();

    bool eventFilter(QObject *, QEvent *);

private:
    void _initialSocket();
    QByteArray _packHeartBeats();
    QByteArray _packSubsribe(const QList<quint32>& tagIDList = QList<quint32>());
    QByteArray _packRefresh();


    void _unpackHeartBeats();
    void _unpackData(QByteArray &data);
signals:

public slots:

private slots:

    void _heartBeatsProcess();
    void _watchDogTimeOverProcess();
    void _lookedUp(QHostInfo host);

    void _receiveProcess();
public:
    bool   socketState;
    bool   connectState;
    QTime  lastCounnection;

private:
    static quint8 _clientID;
    QTimer* _heartBeatsTimer;
    QTimer* _watchDogTimer;
    QHostAddress _serverAddr;
    QUdpSocket* _socket;
};

#endif // RTDBCONNECTION_H
