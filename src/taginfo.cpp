#include "taginfo.h"

TagInfo::TagInfo(int newID, QObject *parent)
    :QObject(parent)
{
    setTagIDAuto( newID );
    setTagValue( QVariant(0) );
    setTagRankCode(0);
    setTagType(TYPE_DEFAULT);

    setTagRWStrategy(RWS_READ);
}

int TagInfo::tagID()const{
    return _tagID;
}
void TagInfo::setTagID(int id){
    if(_tagID != id){
        _tagID = id;
        emit tagIDChanged();
    }
}

int TagInfo::tagIDAuto()const{
    return _tagIDAuto;
}

void TagInfo::setTagIDAuto(int id){
    if(_tagIDAuto != id){
        _tagIDAuto = id;
    }
}

int TagInfo::tagType()const{
    return _tagType;
}

void TagInfo::setTagType(int type){
    if(_tagType != type){
        _tagType = type;
        emit tagTypeChanged();
    }
}

QVariant TagInfo::tagValue()const{
    if(_tagType == TYPE_BOOL || _tagType == TYPE_DATE || _tagType == TYPE_DATETIME
            || _tagType == TYPE_STRING || _tagType == TYPE_TIME
            || _tagType == TYPE_BYTE ||  _tagType == TYPE_WORD ){
        return _tagValue;
    }

    else if(_tagType == TYPE_INT8 || _tagType == TYPE_INT16 || _tagType == TYPE_DEFAULT
            || _tagType == TYPE_INT32|| _tagType == TYPE_UINT8
            || _tagType == TYPE_UINT16 || _tagType == TYPE_UINT32 ){
        switch (_tagRankCode){
        case -5:
            return QVariant(_tagValue.toUInt() * 100000);
        case -4:
            return QVariant(_tagValue.toUInt() * 10000);
        case -3:
            return QVariant(_tagValue.toUInt() * 1000);
        case -2:
            return QVariant(_tagValue.toUInt() * 100);
        case -1:
            return QVariant(_tagValue.toUInt() * 10);
        case 1:
            return QVariant(_tagValue.toUInt() / 10);
        case 2:
            return QVariant(_tagValue.toUInt() / 100);
        case 3:
            return QVariant(_tagValue.toUInt() / 1000);
        case 4:
            return QVariant(_tagValue.toUInt() / 10000);
        case 5:
            return QVariant(_tagValue.toUInt() / 100000);
        case 6:
            return QVariant(_tagValue.toUInt() / 1000000);
        case 7:
            return QVariant(_tagValue.toUInt() / 10000000);
        case 8:
            return QVariant(_tagValue.toUInt() / 100000000);
        case 9:
            return QVariant(_tagValue.toUInt() / 1000000000);
        case 0:
        default:
            return _tagValue;
        }
    }
    else if(_tagType == TYPE_INT64 || _tagType == TYPE_UINT64){
        switch (_tagRankCode){
        case -5:
            return QVariant(_tagValue.toLongLong() * 100000);
        case -4:
            return QVariant(_tagValue.toLongLong() * 10000);
        case -3:
            return QVariant(_tagValue.toLongLong() * 1000);
        case -2:
            return QVariant(_tagValue.toLongLong() * 100);
        case -1:
            return QVariant(_tagValue.toLongLong() * 10);
        case 1:
            return QVariant(_tagValue.toLongLong() / 10);
        case 2:
            return QVariant(_tagValue.toLongLong() / 100);
        case 3:
            return QVariant(_tagValue.toLongLong() / 1000);
        case 4:
            return QVariant(_tagValue.toLongLong() / 10000);
        case 5:
            return QVariant(_tagValue.toLongLong() / 100000);
        case 6:
            return QVariant(_tagValue.toLongLong() / 1000000);
        case 7:
            return QVariant(_tagValue.toLongLong() / 10000000);
        case 8:
            return QVariant(_tagValue.toLongLong() / 100000000);
        case 9:
            return QVariant(_tagValue.toLongLong() / 1000000000);
        case 0:
        default:
            return _tagValue;
        }
    }
    else if(_tagType == TYPE_DOUBLE){
        switch (_tagRankCode){
        case -5:
            return QVariant(_tagValue.toDouble()*100000);
        case -4:
            return QVariant(_tagValue.toDouble()*10000);
        case -3:
            return QVariant(_tagValue.toDouble()*1000);
        case -2:
            return QVariant(_tagValue.toDouble()*100);
        case -1:
            return QVariant(_tagValue.toDouble()*10);
        case 1:
            return QVariant(_tagValue.toDouble()/10);
        case 2:
            return QVariant(_tagValue.toDouble()/100);
        case 3:
            return QVariant(_tagValue.toDouble()/1000);
        case 4:
            return QVariant(_tagValue.toDouble()/10000);
        case 5:
            return QVariant(_tagValue.toDouble()/100000);
        case 6:
            return QVariant(_tagValue.toDouble()/1000000);
        case 7:
            return QVariant(_tagValue.toDouble()/10000000);
        case 8:
            return QVariant(_tagValue.toDouble()/100000000);
        case 9:
            return QVariant(_tagValue.toDouble()/1000000000);
        case 0:
        default:
            return _tagValue;
        }
    }

    else if(_tagType == TYPE_FLOAT){
        switch (_tagRankCode){
        case -5:
            return QVariant(_tagValue.toFloat()*100000);
        case -4:
            return QVariant(_tagValue.toFloat()*10000);
        case -3:
            return QVariant(_tagValue.toFloat()*1000);
        case -2:
            return QVariant(_tagValue.toFloat()*100);
        case -1:
            return QVariant(_tagValue.toFloat()*10);
        case 1:
            return QVariant(_tagValue.toFloat()/10);
        case 2:
            return QVariant(_tagValue.toFloat()/100);
        case 3:
            return QVariant(_tagValue.toFloat()/1000);
        case 4:
            return QVariant(_tagValue.toFloat()/10000);
        case 5:
            return QVariant(_tagValue.toFloat()/100000);
        case 6:
            return QVariant(_tagValue.toFloat()/1000000);
        case 7:
            return QVariant(_tagValue.toFloat()/10000000);
        case 8:
            return QVariant(_tagValue.toFloat()/100000000);
        case 9:
            return QVariant(_tagValue.toFloat()/1000000000);
        case 0:
        default:
            return _tagValue;
        }
    }
}
void TagInfo::setTagValue(QVariant value){
    if(_tagValue != value){
        _tagValue = value;
        emit tagValueChanged();
        _lastUpdateTime = QDateTime::currentDateTime();
        emit lastUpdateTimeChanged();
    }
}

void TagInfo::updateTagValue(const QString& value, const QDateTime& time){
    QString v = value.trimmed().toUpper();
    QString cv_string;
    bool ok;
    QChar v_byte, cv_byte;
    double v_double, cv_double;
    float v_float, cv_float;
    unsigned int cv_uint, v_uint;
    unsigned long long cv_ulonglong, v_ulonglong;
    QDate v_data, cv_data;
    QDateTime v_dataTime, cv_dataTime;
    QTime v_time, cv_time;
    switch(_tagType){
    case TYPE_BOOL:
        cv_uint = _tagValue.toUInt(&ok);
        if(ok && cv_uint == 0 && v == "TRUE" ){
            _tagValue = QVariant(1);
            emit tagValueChanged();
            break;
        }
        else if(ok && cv_uint == 1 && v == "FALSE"){
            _tagValue = QVariant(0);
            emit tagValueChanged();
            break;
        }
        return;
    case TYPE_BYTE:
    case TYPE_INT8:
    case TYPE_UINT8:
        v_byte = *v.data();
        cv_byte = _tagValue.toChar();
        if(cv_byte != v_byte){
            _tagValue = QVariant(v_byte);
            emit tagValueChanged();
            break;
        }
        return;
    case TYPE_DEFAULT:
    case TYPE_WORD:
    case TYPE_INT16:
    case TYPE_INT32:
    case TYPE_UINT16:
    case TYPE_UINT32:
        v_uint = v.toUInt(&ok);
        cv_uint = _tagValue.toUInt();
        if(ok && v_uint != cv_uint){
            _tagValue = QVariant(v_uint);
            emit tagValueChanged();
            break;
        }
        return;
    case TYPE_INT64:
    case TYPE_UINT64:
        v_ulonglong = v.toULongLong(&ok);
        cv_ulonglong = _tagValue.toULongLong();
        if(ok && v_ulonglong != cv_ulonglong){
            _tagValue = QVariant(v_ulonglong);
            emit tagValueChanged();
            break;
        }
        return;
    case TYPE_DOUBLE:
        v_double = v.toDouble(&ok);
        cv_double = _tagValue.toDouble();
        if(ok && v_double != cv_double){
            _tagValue = QVariant(v_double);
            emit tagValueChanged();
            break;
        }
        return;
    case TYPE_FLOAT:
        v_float = v.toFloat(&ok);
        cv_float = _tagValue.toFloat();
        if(ok && v_float != cv_float){
            _tagValue = QVariant(v_float);
            emit tagValueChanged();
            break;
        }
        return;
    case TYPE_DATE:
        v_data = QDate::fromString(value);
        cv_data = _tagValue.toDate();
        if(v_data.isValid() && cv_data != v_data){
            _tagValue = QVariant(v_data);
            emit tagValueChanged();
            break;
        }
        return;
    case TYPE_DATETIME:
        v_dataTime = QDateTime::fromString(value);
        cv_dataTime =_tagValue.toDateTime();
        if(v_dataTime.isValid() && cv_dataTime != v_dataTime){
            _tagValue = QVariant(v_dataTime);
            emit tagValueChanged();
            break;
        }
        return;
    case TYPE_STRING:
        cv_string = _tagValue.toString();
        if(value != cv_string){
            _tagValue = QVariant(value);
            emit tagValueChanged();
            break;
        }
        return;
    case TYPE_TIME:
        v_time = QTime::fromString(value);
        cv_time =_tagValue.toTime();
        if(v_time.isValid() && cv_time != v_time){
            _tagValue = QVariant(v_time);
            emit tagValueChanged();
            break;
        }
        return;
    default:
        return;

    }
    if(time.isValid()){
        _lastUpdateTime = time;
        emit lastUpdateTimeChanged();
    }
}

QString TagInfo::tagName()const{
    return _tagName;
}

void TagInfo::setTagName(const QString& name){
    if(_tagName != name){
        _tagName = name;
        emit tagNameChanged();
    }
}

QString TagInfo::tagUnit()const{
    return _tagUnit;
}

void TagInfo::setTagUnit(const QString& newUnit){
    if(_tagUnit != newUnit){
        _tagUnit = newUnit;
        emit tagUnitChanged();
    }
}

int TagInfo::tagRankCode()const{
    return _tagRankCode;
}
void TagInfo::setTagRankCode(int newCode){
    if(_tagRankCode != newCode){
        _tagRankCode = newCode;
        emit tagRankCodeChanged();
        emit tagValueChanged();
    }
}

int TagInfo::projectID()const{
    return _projectID;
}

void TagInfo::setProjectID(int id){
    if(_projectID!=id){
        _projectID = id;
        emit projectIDChanged();
    }
}

QString TagInfo::projectName()const{
    return _projectName;
}
void TagInfo::setProjectName(const QString& newProjectName){
    if(_projectName != newProjectName){
        _projectName = newProjectName;
        emit projectNameChanged();
    }
}

int TagInfo::stationID()const{
    return _stationID;
}

void TagInfo::setStationID(int id){
    if(_stationID != id){
        _stationID = id;
        emit stationIDChanged();
    }
}

QString TagInfo::stationName()const{
    return _stationName;
}

void TagInfo::setStationName(const QString& newStationName){
    if(_stationName != newStationName){
        _stationName = newStationName;
        emit stationNameChanged();
    }
}

int TagInfo::moduleID()const{
    return _moduleID;
}
void TagInfo::setModuleID(int id){
    if(_moduleID != id){
        _moduleID = id;
        emit moduleIDChanged();
    }
}

int TagInfo::pointID()const{
    return _pointID;
}
void TagInfo::setPointID(int id){
    if(_pointID != id){
        _pointID = id;
        emit pointIDChanged();
    }
}

QString TagInfo::systemName()const{
    return _systemName;
}
void TagInfo::setSystemName(const QString& newSystemName){
    if(_systemName != newSystemName){
        _systemName = newSystemName;
        emit systemNameChanged();
    }
}

QDateTime TagInfo::lastUpdateTime()const{
    return _lastUpdateTime;
}

void TagInfo::setTagRWStrategy(int RWS){
    if(_tagRWStrategy != RWS){
        _tagRWStrategy = RWS;
        emit tagRWStrategyChanged(this);
    }
}

int TagInfo::tagRWStrategy()const{
    return _tagRWStrategy;
}


QDataStream& operator<<(QDataStream& out, const TagInfo& t){
    out<<t._tagID<<t._tagRankCode<<t._tagType<<t._tagRWStrategy<<t._projectID<<t._stationID<<t._moduleID<<t._pointID;
    out<<t._tagName<<t._tagUnit<<t._projectName<<t._stationName<<t._systemName;
    out<<t._tagValue;
    out<<t._lastUpdateTime;
    return out;
}
QDataStream& operator>>(QDataStream& in, TagInfo& t){
    in>>t._tagID>>t._tagRankCode>>t._tagType>>t._tagRWStrategy>>t._projectID>>t._stationID>>t._moduleID>>t._pointID;
    in>>t._tagName>>t._tagUnit>>t._projectName>>t._stationName>>t._systemName;
    in>>t._tagValue;
    in>>t._lastUpdateTime;
    return in;
}
bool operator<(const TagInfo& t1, const TagInfo& t2){
    return t1._tagID < t2._tagID;
}
bool operator>(const TagInfo& t1, const TagInfo& t2){
    return t1._tagID > t2._tagID;
}
bool operator==(const TagInfo& t1, const TagInfo& t2){
    return t1._tagID == t2._tagID;
}
bool operator>=(const TagInfo& t1, const TagInfo& t2){
    return t1._tagID >= t2._tagID;
}
bool operator<=(const TagInfo& t1, const TagInfo& t2){
    return t1._tagID <= t2._tagID;
}
