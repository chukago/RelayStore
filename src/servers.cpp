#include "servers.h"

namespace queries{
	
	QString genSelectQuery(const QStringList select, const QString from, const QStringList where = QStringList(), const QStringList whereValues = QStringList())
	{
		QString res = "SELECT";
		if (!select.isEmpty()){
			for (int i = 0; i < select.size(); i++){
				res+=" " + select[i];
				if (i != select.size()-1){
					res+=",";
				};
			};
		}
		else{
			return(QString());
		};
		
		if (!from.isEmpty()){
			res += " FROM " + from;
		}
		else{
			return(QString());
		};
		
		if ((!where.isEmpty()) && (!whereValues.isEmpty()) && where.size() == whereValues.size()){
			res += " WHERE";
			for (int i = 0; i < where.size(); i++){
				if (i){
					res+=" AND";
				};
				res+=" " + where[i] + "='"+ whereValues[i] + "'";
			};
		}
		else{
			return(res);
		};
		return(res);
        }
	
	QString genInsertQuery(QString into, QStringList fields, QStringList values)
	{
		QString res = "INSERT INTO";
		
		res += " " + into + "(";
		if (fields.size() == values.size()){
			for (int i = 0; i < fields.size(); i++){
				res+=fields[i];
				if (i != fields.size()-1){
					res+=",";
				};
			};
			
			res += ")";
			
			res += " VALUES(";
			for (int i = 0; i < values.size(); i++){
				res+="'" + values[i] + "'";
				if (i != values.size()-1){
					res+=",";
				};
			};
		}
		else{
			return(QString());
		};
		res += ")";
		return(res);
        }
	
	QString genUpdateQuery(QString update, QStringList set, QStringList setValues, QStringList where, QStringList whereValues)
	{
		QString res = "UPDATE";
		res += " " + update + " SET";
		
		if (set.size() == setValues.size()){
			for (int i = 0; i < set.size(); i++){
				res += " " + set[i] + "='" + setValues[i] + "'";
				if (i != set.size()-1){
					res+=",";
				};
			};
		}
		else{
			return(QString());
		};
		
		if ((!where.isEmpty()) && (!whereValues.isEmpty()) && where.size() == whereValues.size()){
			res += " WHERE";
			for (int i = 0; i < where.size(); i++){
				if (i){
					res+=" AND";
				};
				res+=" " + where[i] + "='"+ whereValues[i] + "'";
			};
		}
		else{
			return(res);
		};
		return(res);
        }
}

dbQueryServer::dbQueryServer()
        :_currentUser(0)
{	
    rs_settings_manager *settMan = rs_settings_manager::getInstance();

    _db  = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(settMan->dbName());
    //_db.setHostName(settMan->dbServer());
    //_db.setPassword(settMan->dbPassword());
    //_db.setUserName(settMan->dbUser());
	if(_db.open()){
		_connect = true;
		_query = new QSqlQuery(_db);
		loadTypes();
		loadStations();
		loadStatuses();
	}
	else{
		_connect = false;
	};
}

dbQueryServer::~dbQueryServer(void)
{
	delete(_query);
	_db.close();
}

bool dbQueryServer::isConnected(void)
{
	return (_connect);
}

userDesc_t dbQueryServer::addUser(userDesc_t userInfo)
{
    userDesc_t tempUserDesc = {0, nullptr, nullptr, nullptr, nullptr, nullptr, 0};
	QString _into;
	QStringList _fields;
	QStringList _fieldsValues;
	
	_into = "user";
	_fields << "login" << "passwd" << "name" << "sirname" << "position" << "rights";
	_fieldsValues << userInfo.login << encrypt(userInfo.passwd) << userInfo.name << userInfo.sirname << userInfo.position << QVariant(userInfo.rights).toString();
	
	if (userInfo.login.isEmpty()){
		return(tempUserDesc);
	};
	
	if (_query->exec(queries::genInsertQuery(_into, _fields, _fieldsValues))){
		return(getUserInfo(userInfo.login, encrypt(userInfo.passwd)));
	};
        return(getUserInfo(userInfo.login, encrypt(userInfo.passwd)));
}

userDesc_t dbQueryServer::getUserInfo(QString login, QString passwd)
{
    userDesc_t tempUserDesc = {0, nullptr, nullptr, nullptr, nullptr, nullptr, 0};
	QVector<QString> args;
	
	args.push_back(login);
	args.push_back(passwd);
	
	QStringList _select;
	QString _from;
	QStringList _where;
	QStringList _whereValues;
	
    _select << "id" << "name" << "sirname" << "position" << "rights";
	_from = "user";
	_where << "login" << "passwd";
	_whereValues << login << passwd;

	if (_query->exec(queries::genSelectQuery(_select, _from, _where, _whereValues))){
		
		if (_query->next()){

			tempUserDesc.id		=	_query->value(0).toUInt(); 
			tempUserDesc.name	=	_query->value(1).toString();
			tempUserDesc.sirname	=	_query->value(2).toString();
			tempUserDesc.position	=	_query->value(3).toString();
            tempUserDesc.rights	=	_query->value(4).toUInt();

		}
		else{

            qDebug() << "No query data!" << __FILE__ << __LINE__;
			return(tempUserDesc);
		};
	}
	else{

        qDebug() << "Cannot execute query!" << __FILE__ << __LINE__;
        qDebug() << queries::genSelectQuery(_select, _from, _where, _whereValues) << __FILE__ << __LINE__;
		return(tempUserDesc);
	};
	
	return(tempUserDesc);
}

QString dbQueryServer::getUserById(unsigned int id)
{
        QString info;
        QStringList _select;
        QString _from;
        QStringList _where;
        QStringList _whereValues;

        _select << "name" << "sirname" << "position";
        _from = "user";
        _where << "id";
        _whereValues << QVariant(id).toString();

        if (_query->exec(queries::genSelectQuery(_select, _from, _where, _whereValues))){

                /*Запрос не смог возвратить данные, что в принципе невозможно!!!*/
                if (_query->next()){
                        info = _query->value(0).toString() + " " + _query->value(1).toString() + " - " + _query->value(2).toString();
                }
                else{
                        return(QString());
                };
        }
        else{
                return(QString());
        };
        return(info);
}

bool dbQueryServer::isUser(QString login)
{
	QStringList _select;
	QString _from;
	QStringList _where;
	QStringList _whereValues;
	
	_select << "id";
	_from = "user";
	_where << "login";
	_whereValues << login;
	
	if (_query->exec(queries::genSelectQuery(_select, _from, _where, _whereValues))){
		
        if (_query->next()){

			return(true);
		};
	};
	return(false);
}

void dbQueryServer::setCurrentUser(unsigned int id)
{
    _currentUser = id;
}

unsigned int dbQueryServer::currentUser(void)
{
    return(_currentUser);
}

QString dbQueryServer::encrypt(QString line)
{
	QString res;
	QByteArray temp = QCryptographicHash::hash(QVariant(line).toByteArray(), QCryptographicHash::Md5);
	
	for (int i = 0; i < temp.size(); i++){
		res.append(QVariant(QVariant(temp[i]).toUInt()).toString());
	};
	
	return (res);
};

void dbQueryServer::addType(typeDesc_t type)
{
	QString _into;
	QStringList _fields;
	QStringList _values;
	
	QString _type = type.type;
	QString _check_period = QVariant(type.checkPeriod).toString();
	
	_into = "relay";
	_fields << "type" << "check_period";
	_values << _type << _check_period;
	
	_query->exec(queries::genInsertQuery(_into, _fields, _values));
}

bool dbQueryServer::loadTypes(void)
{
	_types.clear();
	
    typeDesc_t tempTypeDesc = {0, nullptr, 0};
	
	QStringList _select;
	QString _from;
	QStringList _where;
	QStringList _whereValues;
	
	_select << "id" << "type" << "check_period";
	_from = "relay";
	
	// Запрос с сортировкой по типу
	if (_query->exec(queries::genSelectQuery(_select, _from) + " ORDER BY type")){
		
		while(_query->next()){
			tempTypeDesc.id			=	_query->value(0).toUInt();
			tempTypeDesc.type			=	_query->value(1).toString();
			tempTypeDesc.checkPeriod	=	_query->value(2).toUInt();
			_types.append(tempTypeDesc);
		};
	}
	else{

        return (false);
	};

	return (true);
}


QVector<typeDesc_t> dbQueryServer::getTypes(void)
{
	return (_types);
}

typeDesc_t dbQueryServer::getTypeById(unsigned int id)
{
    typeDesc_t tempTypeDesc = {0, nullptr, 0};
	
	if (_types.size() > 0){
		for (int i = 0; i < _types.size(); i++){
			if (_types[i].id == id){
				tempTypeDesc = _types[i];
				break;
			};
		};
	};
	
	return(tempTypeDesc);
}

typeDesc_t dbQueryServer::getTypeByName(QString type)
{
    typeDesc_t tempTypeDesc = {0, nullptr, 0};
	
	if (_types.size() > 0){
		for (int i = 0; i < _types.size(); i++){
			if (_types[i].type == type){
				tempTypeDesc = _types[i];
				break;
			};
		};
	};
	
	return(tempTypeDesc);
}

bool dbQueryServer::loadStations(void)
{
	_stations.clear();
	
    stationDesc_t tempStationDesc = {0, nullptr};
	
	QStringList _select;
	QString _from;
	QStringList _where;
	QStringList _whereValues;
	
	_select << "*";
	_from = "stations";
	
	if (_query->exec(queries::genSelectQuery(_select, _from))){
		
		while(_query->next()){
			tempStationDesc.id				=	_query->value(0).toUInt();
			tempStationDesc.station			=	_query->value(1).toString();
			_stations.append(tempStationDesc);
		};
	}
	else{
		return (false);
	};
	return (true);
};

QVector<stationDesc_t> dbQueryServer::getStations(void)
{
	return(_stations);
}

stationDesc_t dbQueryServer::getStationById(unsigned int id)
{
    stationDesc_t tempStationDesc = {0, nullptr};
	
	if (_stations.size() > 0){
		for (int i = 0; i < _stations.size(); i++){
			if (_stations[i].id == id){
				tempStationDesc = _stations[i];
				break;
			};
		};
	};
	
	return(tempStationDesc);
}

stationDesc_t dbQueryServer::getStationByName(QString station)
{
    stationDesc_t tempStationDesc = {0, nullptr};
	
	if (_stations.size() > 0){
		for (int i = 0; i < _stations.size(); i++){
			if (_stations[i].station == station){
				tempStationDesc = _stations[i];
				break;
			};
		};
	};
	
	return(tempStationDesc);
}

/*Загрузка описаний статусов*/
bool dbQueryServer::loadStatuses(void)
{
	_statuses.clear();
	
	QStringList _select;
	QString _from;
	QStringList _where;
	QStringList _whereValues;
	
	_select << "*";
	_from = "status";
	
    statusDesc_t tempStatusDesc = {0, nullptr};
	
	if (_query->exec(queries::genSelectQuery(_select, _from))){
		
		while(_query->next()){
			tempStatusDesc.id				=	_query->value(0).toUInt();
			tempStatusDesc.status			=	_query->value(1).toString();
			_statuses.append(tempStatusDesc);
		};
	}
	else{
		return (false);
	};
	return (true);
}

QVector<statusDesc_t> dbQueryServer::getStatuses(void)
{
	return (_statuses);
}

statusDesc_t dbQueryServer::getStatusById(unsigned int id)
{
    statusDesc_t tempStatusDesc = {0, nullptr};
	
	if (_statuses.size() > 0){
		for (int i = 0; i < _statuses.size(); i++){
			if (_statuses[i].id == id){
				tempStatusDesc = _statuses[i];
				break;
			};
		};
	};
	
	return(tempStatusDesc);
}

statusDesc_t dbQueryServer::getStatusByName(QString status)
{
    statusDesc_t tempStatusDesc = {0, nullptr};
	
	if (_statuses.size() > 0){
		for (int i = 0; i < _statuses.size(); i++){
			if (_statuses[i].status == status){
				tempStatusDesc = _statuses[i];
				break;
			};
		};
	};
	
	return(tempStatusDesc);
}

QVector<relayDesc_t> dbQueryServer::findRelay(findParam_t findParam, bool addToBuffer, bool sort)
{
	QVector<relayDesc_t> res;
    relayDesc_t tempRelayDesc = {0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr};
	QStringList _select;
	QString _from;
	QStringList _where;
	QStringList _whereValues;

	_select << "*";
	_from = "store";

	_relays.clear();

	/*Идентификатор*/
	if (findParam.id != 0){
		_where << "id";
		_whereValues << QVariant(findParam.id).toString();
	}
	else
	{
	
	/*Тип*/
	if (findParam.type != 0){
		_where << "type";
		_whereValues << QVariant(findParam.type).toString();
	};
	
	/*Серийный номер*/
	if (!findParam.serial_number.isEmpty()){
		_where << "serial_number";
		_whereValues << findParam.serial_number;
	};
	
	/*Дата производства*/
	if (findParam.manufactured != 0){
		_where << "manufactured";
		_whereValues << QVariant(findParam.manufactured).toString();
	};
	
	/*Станция*/
	if (findParam.station != 0){
		_where << "station";
		_whereValues << QVariant(findParam.station).toString();
	};
	
	/*Статив*/
	if (findParam.relay_rack != 0){
		_where << "relay_rack";
		_whereValues << QVariant(findParam.relay_rack).toString();
	};
	
	/*Номер прибора*/
	if (findParam.device_number != 0){
		_where << "device_number";
		_whereValues << QVariant(findParam.device_number).toString();
	};
	
	/*Статус*/
	if (findParam.status != 0){
		_where << "status";
		_whereValues << QVariant(findParam.status).toString();
	};
	
	/*Комментарий*/
	if (!findParam.comment.isEmpty()){
		_where << "comment";
		_whereValues << findParam.comment;
	};

	};

	bool query_exec = false;

	if (sort)
	{
	    query_exec = _query->exec(queries::genSelectQuery(_select, _from, _where, _whereValues) + "ORDER BY station, relay_rack, device_number");
	}
	else
	{
	    query_exec = _query->exec(queries::genSelectQuery(_select, _from, _where, _whereValues));
	};

	if (query_exec){
		
		while(_query->next()){
			
                        tempRelayDesc.id			=	_query->value(0).toUInt();
			tempRelayDesc.type			=	_query->value(1).toUInt();
			tempRelayDesc.serial_number		=	_query->value(2).toString();
			tempRelayDesc.manufactured		=	_query->value(3).toUInt();
			tempRelayDesc.station			=	_query->value(4).toUInt();
			tempRelayDesc.relay_rack		=	_query->value(5).toUInt();
                        tempRelayDesc.device_number             =	_query->value(6).toUInt();
			tempRelayDesc.last_check		=	_query->value(7).toUInt();
			tempRelayDesc.status			=	_query->value(8).toUInt();
                        tempRelayDesc.comment                   =	_query->value(9).toString();
			
                        if (((findParam.before_date !=0) || (findParam.from_date !=0))){

                            QRegExp re("\\((\\d+)\\).*$");
                            uint _check_period;
                            int pos=re.indexIn(tempRelayDesc.comment, 0);
                            if (pos != -1){
                                _check_period = re.cap(1).toUInt();
                            }
                            else{
                                _check_period = getTypeById(tempRelayDesc.type).checkPeriod;
                            };

                            QDateTime _dt = QDateTime::fromTime_t(tempRelayDesc.last_check);
                            _dt = _dt.addYears(static_cast<int>(_check_period));
                            if (_check_period != 0){
				if ((findParam.before_date != 0) && (!(findParam.before_date >= _dt.toTime_t()))){
				    continue;
				};
				if ((findParam.from_date != 0) && (!(findParam.from_date <= _dt.toTime_t()))){
				    continue;
				};
				res.append(tempRelayDesc);
				if (addToBuffer){
				    _relays.append(tempRelayDesc);
				};
			    };
			}
			else{
			    res.append(tempRelayDesc);
			    if (addToBuffer){
				_relays.append(tempRelayDesc);
			    };
			};
		};
	};

	return(res);	
}

QVector<relayDesc_t> dbQueryServer::getRelays(void)
{
	return(_relays);
}

relayDesc_t dbQueryServer::getRelayById(unsigned int id)
{
    relayDesc_t tempRelayDesc = {0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr};
	
	for (int i = 0; i < _relays.size(); i++){
		if (_relays[i].id == id){
			tempRelayDesc = _relays[i];
			return(tempRelayDesc);
		};
	};
	return(tempRelayDesc);
}

void dbQueryServer::addRelayToBuffer(relayDesc_t relay)
{
	for (int i = 0; i < _writeBuffer.size(); i++){
		if (_writeBuffer[i].id == relay.id){
                        _writeBuffer[i] = relay;
			return;
		};
	};
	_writeBuffer.append(relay);
}

relayDesc_t dbQueryServer::getRelayFromBuffer(unsigned int id)
{
    relayDesc_t tempRelayDesc = {0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr};
	for (int i = 0; i < _writeBuffer.size(); i++){
		if (_writeBuffer[i].id == id){
			tempRelayDesc = _writeBuffer[i];
			return (tempRelayDesc);
		};
	};
	return (tempRelayDesc);
}

void dbQueryServer::deleteRelayFromBuffer(unsigned int id)
{
	if (_writeBuffer.isEmpty()){
		return;
	};
	
	for (int i = 0; i < _writeBuffer.size(); i++){
		if (_writeBuffer[i].id == id){
			_writeBuffer.remove(i);
			
		};
	};
}

uint dbQueryServer::writeToDb(void)
{
	QString _table;
	QStringList _fields;
	QStringList _where;
	unsigned int res = 0;
	
	if (_writeBuffer.isEmpty()){
		return(0);
	};

	_table = "store";
	_fields << "type" << "serial_number" << "manufactured" << "station" << "relay_rack" << "device_number" << "last_check" << "status" << "comment";
	_where << "id";

        writeHistory();
	
	for (int i = 0; i < _writeBuffer.size(); i++){
		QStringList _fieldsValues;
		QStringList _whereValues;
		
		QString _id = QVariant(_writeBuffer[i].id).toString();
		QString _type = QVariant(_writeBuffer[i].type).toString();
		QString _serial_number = _writeBuffer[i].serial_number;
		QString _manufactured = QVariant(_writeBuffer[i].manufactured).toString();
		QString _station = QVariant(_writeBuffer[i].station).toString();
		QString _relay_rack = QVariant(_writeBuffer[i].relay_rack).toString();
		QString _device_number = QVariant(_writeBuffer[i].device_number).toString();
		QString _last_check = QVariant(_writeBuffer[i].last_check).toString();
		QString _status = QVariant(_writeBuffer[i].status).toString();
		QString _comment = _writeBuffer[i].comment;
		
		_fieldsValues << _type << _serial_number << _manufactured << _station << _relay_rack << _device_number << _last_check << _status << _comment;
		
		if (_writeBuffer[i].id){
			_whereValues << _id;
	
			if (_query->exec(queries::genUpdateQuery(_table, _fields, _fieldsValues, _where, _whereValues))){
				res +=1;
			};
		}
		else{
			if(_query->exec(queries::genInsertQuery(_table, _fields, _fieldsValues))){
				res+=1;
			};
		};
	};
	
	#warning "Потенциально опасная операция"
	for (int i = 0; i < _writeBuffer.size(); i++){
		for (int g = 0; g < _relays.size(); g++){
			if (_writeBuffer[i].id == _relays[g].id){
				_relays[g] = _writeBuffer[i];
				break;
			};
		};
	};

	#warning "Доработать очищение буффера"
        _writeBuffer.clear();

	return(res);
}

// Сохраняет предыдущее состояние реле - "историю" реле
uint dbQueryServer::writeHistory(void)
{
	unsigned int res = 0;
	QString _into;
	QStringList _fields;
	QStringList _fieldsValues;
        relayDesc_t _relay;

        _into = "history";
        _fields << "rel_id" << "date" << "station" << "relay_rack" << "device_number" << "last_check" << "status" << "comment" << "user";
	
	for (int i = 0; i < _writeBuffer.size(); i++){
            if (_writeBuffer[i].id != 0){
                _relay = getRelayById(_writeBuffer[i].id);
                /*
                QString _rel_id = QVariant(_writeBuffer[i].id).toString();
		QString _date = QVariant(QDateTime::currentDateTime().toTime_t()).toString();
		QString _station = QVariant(_writeBuffer[i].station).toString();
		QString _relay_rack = QVariant(_writeBuffer[i].relay_rack).toString();
		QString _device_number = QVariant(_writeBuffer[i].device_number).toString();
		QString _last_check = QVariant(_writeBuffer[i].last_check).toString();
		QString _status = QVariant(_writeBuffer[i].status).toString();
		QString _comment = _writeBuffer[i].comment;
                */
                QString _rel_id = QVariant(_relay.id).toString();
                QString _date = QVariant(QDateTime::currentDateTime().toTime_t()).toString();
                QString _station = QVariant(_relay.station).toString();
                QString _relay_rack = QVariant(_relay.relay_rack).toString();
                QString _device_number = QVariant(_relay.device_number).toString();
                QString _last_check = QVariant(_relay.last_check).toString();
                QString _status = QVariant(_relay.status).toString();
                QString _comment = _relay.comment;

                _fieldsValues << _rel_id << _date << _station << _relay_rack << _device_number << _last_check << _status << _comment << QVariant(currentUser()).toString();
		if(_query->exec(queries::genInsertQuery(_into, _fields, _fieldsValues))){
			res+=1;
		};
            };
	};
	return(res);
 }
 
 QVector<historyDesc_t> dbQueryServer::findHistory(unsigned int rel_id)
 {
	 _history.clear();
	 QVector<historyDesc_t> res;
         historyDesc_t tempHistoryDesc = {0, 0, 0, 0, 0, 0, 0, 0, nullptr, 0};
	
	 QStringList _select;
	 QString _from;
	 QStringList _where;
	 QStringList _whereValues;
         _select << "id" << "date" << "station" << "relay_rack" << "device_number" << "last_check" << "status" << "comment" << "user";
	 _from = "history";
	 _where << "rel_id";
	 _whereValues << QVariant(rel_id).toString();
	 
	 if (_query->exec(queries::genSelectQuery(_select, _from, _where, _whereValues))){
		while (_query->next()){	
			tempHistoryDesc.id				=	_query->value(0).toUInt();
			tempHistoryDesc.rel_id			=	rel_id;
			tempHistoryDesc.date			=	_query->value(1).toUInt();
			tempHistoryDesc.station			=	_query->value(2).toUInt();
			tempHistoryDesc.relay_rack		=	_query->value(3).toUInt();
			tempHistoryDesc.device_number	=	_query->value(4).toUInt();
			tempHistoryDesc.last_check		=	_query->value(5).toUInt();
			tempHistoryDesc.status			=	_query->value(6).toUInt();
			tempHistoryDesc.comment		=	_query->value(7).toString();
                        tempHistoryDesc.owner           =       _query->value(8).toUInt();
			
			_history.append(tempHistoryDesc);
			res.append(tempHistoryDesc);
			
		};
	};
	
	return(res);
 }
