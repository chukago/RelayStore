#ifndef RS_QUERY_DIALOG_H
#define RS_QUERY_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QComboBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "../src/servers.h"

class rs_query_dialog : public QDialog
{
	Q_OBJECT
	
	private:
		
		dbQueryServer *_req_server;
	
		QGridLayout *_gl;
		
        //QLabel *_id_lb;
		QLabel *_type_lb;
		QLabel *_ser_num_lb;
		QLabel *_station_lb;
		QLabel *_relay_rack_lb;
		QLabel *_device_number_lb;
        QLabel *_next_check_from_lb;
		QLabel *_next_check_before_lb;
		QLabel *_status_lb;
		QLabel *_sort_lb;

        //QLineEdit *_id_ed;
		QComboBox *_type_ed;
		QLineEdit *_ser_num_ed;
		QComboBox *_station_ed;
		QLineEdit *_relay_rack_ed;
		QLineEdit *_device_number_ed;

                QDateEdit *_next_check_from_ed;
		QDateEdit *_next_check_before_ed;
		QComboBox *_status_ed;

		QCheckBox *_sort_ck;

                QCheckBox *_next_check_from_ck;
		QCheckBox *_next_check_before_ck;
		
		QPushButton *_ok;
		QPushButton *_cancel;
		
		void setConnections(void);
		
	public:
		QString result_query;
		unsigned int nextCheck;
		rs_query_dialog(QWidget *parent, dbQueryServer *req_server);
	
    public slots:
        void next_check_from_enable(int);
		void next_check_before_enable(int);
		void request(void);
	
};

#endif /*RS_QUERY_DIALOG_H*/
