#ifndef RS_MAIN_TABLE_H
#define RS_MAIN_TABLE_H

#include <QTableWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVector>
#include <QTableWidgetItem>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QDateEdit>
#include <QFile>
#include <QRegExp>
#include "rs_change_dialog.h"
#include "rs_html.h"
#include "../src/servers.h"
#include <QLocale>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QAction>

#include "rs_manager.h"

struct _table_row_type{
	QTableWidgetItem *id;				// Идентификатор по базе данных
	QTableWidgetItem *type;				// Тип
	QTableWidgetItem *check_period;		// Период проверки
	QTableWidgetItem *serial_number;		// Серийный номер
	QTableWidgetItem *manufactured;		// Дата изготовления
	QTableWidgetItem *station;			// Станция
	QTableWidgetItem *relay_rack;		// Номер статива
	QTableWidgetItem *device_number;		// Номер прибора
	QTableWidgetItem *last_check;		// Дата последней проверки
	QTableWidgetItem *next_check;		// Дата следующей проверки
	QTableWidgetItem *status;			// Статус
	QTableWidgetItem *comment;			// Примечание
};

class rs_main_table : public QTableWidget
{
	Q_OBJECT
	
	private:

    logic_manager *manager;
		
                QMenu *_popup_menu;

		dbQueryServer *_req_server;

                QLocale *_locale;
		
		bool _readOnly;
		/* Массив строк таблицы */
		QVector<_table_row_type> _items;
	
		void writeRow(int row, relayDesc_t relay);
	
		void setConnections(void);
		void _table_setup(void);
        void _write_table(void);
        int _alt_check_period(relayDesc_t &relay);
        QBrush defaultBackground;
	
    private slots:
        void _item_double_clicked(int, int);
		
	public:
		rs_main_table(QWidget *parent, dbQueryServer *req_server);
		
		/*Очищение таблицы и всех данных класса*/
		void clear(void);
	
		/*Поступление нового запроса*/
		void new_request(void);
		
		/*Только для чтения*/
		void setReadOnly(void);
	
};

#endif /*RS_MAIN_TABLE_H*/
