#ifndef RS_ADD_DIALOG_H
#define RS_ADD_DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QVector>
#include <QKeyEvent>
#include "../src/servers.h"

class rs_add_dialog : public QDialog
{
	Q_OBJECT
	
	private:
		
		dbQueryServer *_req_server;
	
		struct rs_relay *_relay;
		QLabel *_type_lb; 
		QLabel *_period_lb;
		QLabel *_ser_num_lb;
		QLabel *_manufactured_lb;
		QLabel *_station_lb;
		QLabel *_relay_rack_lb;
		QLabel *_device_number_lb;
		QLabel *_last_check_lb;
		QLabel *_next_check_lb;
		QLabel *_status_lb;
		QLabel *_comment_lb;
		
		QComboBox *_type_ed; 
		QLineEdit *_period_ed;
		QLineEdit *_ser_num_ed;
		QDateEdit *_manufactured_ed;
		QComboBox *_station_ed;
		QLineEdit *_relay_rack_ed;
		QLineEdit *_device_number_ed;
		QDateEdit *_last_check_ed;
		QLineEdit *_next_check_ed;
		QComboBox *_status_ed;
		QLineEdit *_comment_ed;
		
		QPushButton *_ok;
		QPushButton *_cancel;
		QGridLayout *_gl;
	
		void setConnections(void);
		
		/*
		bool check_input(void);			// Проверка ввода
		bool check_data(void);			// Проверка введённых данных
		bool check_database(void);		// Проверка в базе данных
		QString gen_query(void);			// Генерация запроса
		*/
	
	public:
		rs_add_dialog(QWidget *parent, dbQueryServer *req_server);
	
	public slots:
		void check_errors(void);		// Проверка введённых данных
		void check_type(QString);		// Проверка введённых  данных в поле типа реле
                //void check_period(QString);		// Проверка введённого периода проверки
		void check_status(QString);		// Проверка статуса
		void check_manufactured(QDate);	// Проверка даты производства
		void check_last_check(QDate);	// Проверка даты последней проверки
        protected:
                void keyPressEvent(QKeyEvent* key_event);
};

#endif /*RS_ADD_DIALOG_H*/
