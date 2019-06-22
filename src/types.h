#ifndef TYPES_H
#define TYPES_H

#define RS_READ		0x00000001
#define RS_WRITE	0x00000002
#define RS_ADD		0x00000004
#define RS_SETT		0x00000008
#define RS_PRINT	0x00000010

#define RS_STATUS_INSTALL   0x00000001
#define RS_STATUS_STORE	    0x00000002
#define RS_STATUS_KIP	    0x00000003
#define RS_STATUS_REPAIR    0x00000004

/*Тип описания пользователя*/
typedef struct userDesc_t{
        unsigned int id;			// Идентификатор
        QString login;				// Логин
        QString passwd;				// Пароль
        QString name;				// Имя
        QString sirname;			// Фамилия
        QString position;			// Должность
        unsigned int rights;			// Права доступа
} userDesc_t;

/*Тип описания реле*/
typedef struct relayDesc_t{
        unsigned int id;			// Идентификатор по базе данных
        unsigned int type;			// Тип
        QString serial_number;			// Серийный номер
        unsigned int manufactured;		// Дата изготовления
        unsigned int station;			// Станция
        unsigned int relay_rack;		// Номер статива
        unsigned int device_number;             // Номер прибора
        unsigned int last_check;		// Дата последней проверки
        unsigned int status;			// Статус
        QString comment;			// Примечание
} relayDesc_t;

/*Тип описания типа реле*/
typedef struct typeDesc_t{
        unsigned int id;			// Идентификатор
        QString type;				// Тип
        unsigned int checkPeriod;		// Период проверки
} typeDesc_t;

/*Тип описания станции*/
typedef struct stationDesc_t{
        unsigned int id;			// Идентификатор
        QString station;			// Станция
} stationDesc_t;

/*Тип описания статуса*/
typedef struct statusDesc_t{
        unsigned int id;			// Идентификатор
        QString status;				// Статус
} statusDesc_t;

/* Тип параметров для поиска реле*/
typedef struct findParam_t{
        unsigned int id;                // Идентификатор по базе данных
        unsigned int type;              // Тип
        unsigned int check_period;		// Период проверки
        QString serial_number;			// Серийный номер
        unsigned int manufactured;		// Дата изготовления
        unsigned int station;			// Станция
        unsigned int relay_rack;		// Номер статива
        unsigned int device_number;     // Номер прибора
        unsigned int from_date;         // Дата, после которой реле должно быть проверено
        unsigned int before_date;		// Дата, до которой реле должно быть проверено
        unsigned int status;			// Статус
        QString comment;                // Примечание
} findParam_t;

/*Тип описатель истории*/
typedef struct historyDesc_t{
        unsigned int id;			// Идентификатор истории
        unsigned int rel_id;			// Идентификатор реле
        unsigned int date;			// Дата добавления записи
        unsigned int station;			// Станция
        unsigned int relay_rack;		// Номер статива
        unsigned int device_number;             // Номер прибора
        unsigned int last_check;		// Дата, до которой реле должно быть проверено
        unsigned int status;			// Статус
        QString comment;			// Примечание
        unsigned int owner;                     // Внёсший изменения
} historyDesc_t;

#endif // TYPES_H
