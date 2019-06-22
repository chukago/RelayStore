#ifndef RS_HISTORY_H
#define RS_HISTORY_H

#include <QDialog>
#include <QVector>
#include <QString>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "../src/servers.h"

class rs_history : public QDialog
{
	Q_OBJECT
	
	private:
		QLabel *_info;
		QTableWidget *_table;
		QPushButton *_ok;
		QVBoxLayout *_layout;
		
	public:
		rs_history(QWidget *parent, dbQueryServer *req_server, unsigned int id);
		~rs_history(void);
};

#endif /*RS_HISTORY_H*/
