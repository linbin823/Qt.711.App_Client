#ifndef TAGINFO_H
#define TAGINFO_H

#include <QVariant>
#include <QObject>
#include <QDateTime>
#include <QDataStream>

class TagInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int       tagID          READ tagID          NOTIFY tagIDChanged)
    Q_PROPERTY(int       tagType        READ tagType        NOTIFY tagTypeChanged)
    Q_PROPERTY(QVariant  tagValue       READ tagValue       WRITE setTagValue       NOTIFY tagValueChanged)
    Q_PROPERTY(QString   tagName        READ tagName        NOTIFY tagNameChanged)
    Q_PROPERTY(QString   tagUnit        READ tagUnit        NOTIFY tagUnitChanged)
    Q_PROPERTY(int       projectID      READ projectID      NOTIFY projectIDChanged)
    Q_PROPERTY(QString   projectName    READ projectName    NOTIFY projectNameChanged)
    Q_PROPERTY(int       stationID      READ stationID      NOTIFY stationIDChanged)
    Q_PROPERTY(QString   stationName    READ stationName    NOTIFY stationNameChanged)
    Q_PROPERTY(int       moduleID       READ moduleID       NOTIFY moduleIDChanged)
    Q_PROPERTY(int       pointID        READ pointID        NOTIFY pointIDChanged)
    Q_PROPERTY(QString   systemName     READ systemName     NOTIFY systemNameChanged)
    Q_PROPERTY(QDateTime lastUpdateTime READ lastUpdateTime NOTIFY lastUpdateTimeChanged)
    Q_PROPERTY(int       tagRWStrategy  READ tagRWStrategy  WRITE setTagRWStrategy  NOTIFY tagRWStrategyChanged)

public:

    explicit TagInfo(int newID, QObject *parent = 0);

    enum __enumTagRWStrategyCode{
        RWS_DISABLE,
        RWS_READ,
        RWS_READ_ONCE,
        RWS_WRITE,
        RWS_WRITE_ONCE,
        RWS_READ_WRITE
    };
    enum __enumTagTypeCode{
        TYPE_DEFAULT=0x00,
        TYPE_BOOL,
        TYPE_BYTE,
        TYPE_WORD,
        TYPE_INT8,
        TYPE_INT16,
        TYPE_INT32,
        TYPE_INT64,
        TYPE_UINT8,
        TYPE_UINT16,
        TYPE_UINT32,
        TYPE_UINT64,
        TYPE_FLOAT,
        TYPE_DOUBLE,
        TYPE_STRING,
        TYPE_DATETIME,
        TYPE_TIME,
        TYPE_DATE
    };

    int tagID()const;
    void setTagID(int id);
    int tagIDAuto()const;
    void setTagIDAuto(int id);
    int tagType()const;
    void setTagType(int type);
    QVariant tagValue()const;
    void setTagValue(QVariant value);
    QString tagName()const;
    void setTagName(const QString& name);
    QString tagUnit()const;
    void setTagUnit(const QString& newUnit);
    int projectID()const;
    void setProjectID(int id);
    QString projectName()const;
    void setProjectName(const QString& newProjectName);
    int stationID()const;
    void setStationID(int id);
    QString stationName()const;
    void setStationName(const QString& newStationName);
    int moduleID()const;
    void setModuleID(int id);
    int pointID()const;
    void setPointID(int id);
    QString systemName()const;
    void setSystemName(const QString& newSystemName);
    void setTagRWStrategy(int RWS);
    int tagRWStrategy()const;

    void updateTagValue(const QString &value, const QDateTime &time = QDateTime::currentDateTime() );//for driver
    QDateTime lastUpdateTime()const;
    int tagRankCode()const;
    void setTagRankCode(int newCode);

    friend QDataStream& operator<<(QDataStream& out, const TagInfo& t);
    friend QDataStream& operator>>(QDataStream& in, TagInfo& t);
    friend bool operator<(const TagInfo& t1, const TagInfo& t2);
    friend bool operator>(const TagInfo& t1, const TagInfo& t2);
    friend bool operator==(const TagInfo& t1, const TagInfo& t2);
    friend bool operator>=(const TagInfo& t1, const TagInfo& t2);
    friend bool operator<=(const TagInfo& t1, const TagInfo& t2);

private:
    int         _tagID,_tagIDAuto, _tagRankCode, _tagType, _tagRWStrategy,_projectID,_stationID,_moduleID,_pointID;
    QString     _tagName, _tagUnit, _projectName, _stationName, _systemName;
    QVariant    _tagValue;
    QDateTime   _lastUpdateTime;
signals:
    void tagIDChanged();
    void tagTypeChanged();
    void tagValueChanged();
    void tagNameChanged();
    void tagUnitChanged();
    void tagRankCodeChanged();
    void projectIDChanged();
    void projectNameChanged();
    void stationIDChanged();
    void stationNameChanged();
    void moduleIDChanged();
    void pointIDChanged();
    void systemNameChanged();
    void lastUpdateTimeChanged();
    void tagRWStrategyChanged(TagInfo*);
};
#endif // TAGINFO_H
