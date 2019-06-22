#ifndef SERVERS_H
#define SERVERS_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVector>
#include <QVariant>
#include <QString>
#include <QRegExp>
#include <QCryptographicHash>
#include <QDebug>
#include <QStringList>
#include <QDateTime>
#include "types.h"

#include "rs_settings_manager.h"

class dbQueryServer
{
	private:
		QSqlDatabase _db;
		QSqlQuery *_query;
	
		QVector<typeDesc_t> _types;
		QVector<stationDesc_t> _stations;
		QVector<statusDesc_t> _statuses;
		QVector<relayDesc_t> _relays;
		QVector<relayDesc_t> _writeBuffer;
		QVector<historyDesc_t> _history;
			
		bool _connect;
                unsigned int _currentUser;
	
	public:
        dbQueryServer();
		~dbQueryServer(void);
		/*Установлено ли соединение с базой данных*/
		bool isConnected(void);
	
		/*Добавить пользователя*/
		userDesc_t addUser(userDesc_t userInfo);
		/*Запрос данных о пользователе из базы данных*/
		userDesc_t getUserInfo(QString login, QString passwd);
                /**/
                QString getUserById(unsigned int id);
		/*Существует ли пользователь?*/
		bool isUser(QString login);
                /*Установить текущего пользователя*/
                void setCurrentUser(unsigned int id);
                /*Текущий пользователь*/
                unsigned int currentUser(void);
	
		/*Зашифровать строку*/
		QString encrypt(QString line);
	
		/*Добавление типа в БД*/
		void addType(typeDesc_t type);
		/*Загрузка описаний типов*/
		bool loadTypes(void);
		/*Получение массива с описаниями типов*/
		QVector<typeDesc_t> getTypes(void);
		/*Получения описания типа по идентификатору*/
		typeDesc_t getTypeById(unsigned int id);
		/*Получения описания типа по наименованию типа*/
		typeDesc_t getTypeByName(QString type);
		
		/*Загрузка описаний станций*/
		bool loadStations(void);
		/*Получение массива с описаниями станций*/
		QVector<stationDesc_t> getStations(void);
		/*Получения описания станции по идентификатору*/
		stationDesc_t getStationById(unsigned int id);
		/*Получения описания станции по наименованию станции*/
		stationDesc_t getStationByName(QString station);
		
		/*Загрузка описаний статусов*/
		bool loadStatuses(void);
		/*Получение массива с описаниями статусов*/
		QVector<statusDesc_t> getStatuses(void);
		/*Получения описания статуса по идентификатору*/
		statusDesc_t getStatusById(unsigned int id);
                /*Получения описания статуса по наименованию статуса*/
		statusDesc_t getStatusByName(QString status);
		
		/*Поиск по базе*/
		QVector<relayDesc_t> findRelay(findParam_t findParam, bool addToBuffer = false, bool sort = false);
		/*Ссылка на массив с описаниями реле*/
		QVector<relayDesc_t> getRelays(void);
		/*Получение описания реле по идентификатору*/
		relayDesc_t getRelayById(unsigned int id);
		
		/*Добавление реле в буффер для записи*/
		void addRelayToBuffer(relayDesc_t relay);
		/*Получение описания реле из буффера*/
		relayDesc_t getRelayFromBuffer(unsigned int id);
		/*Удаление реле из буффера*/
		void deleteRelayFromBuffer(unsigned int id);
		/*Запись из буфера в БД*/
        uint writeToDb(void);
		/*Запись истории*/
        uint writeHistory(void);
		
		/*Поиск истории по идентификатору реле*/
		QVector<historyDesc_t> findHistory(unsigned int id);
		
};

#endif /*SERVERS_H*/
