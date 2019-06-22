#ifndef RS_ABOUT_H
#define RS_ABOUT_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDate>
#include <QString>
#include <QGridLayout>
//#include "../src/cryptutils.h"
//#include "../src/servers.h"

class rs_about : public QDialog
{
	Q_OBJECT
	
	private:
        //QLabel *_info;
        //QPushButton *_ok;
        //QGridLayout *_gl;
	public:
		rs_about(QWidget *parent);
};

#endif /*RS_ABOUT_H*/
