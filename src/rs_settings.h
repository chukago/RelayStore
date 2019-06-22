#ifndef RS_SETTINGS_H
#define RS_SETTINGS_H

#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDialog>
#include <QMessageBox>
#include <QVariant>
#include "../src/servers.h"

class rs_settings : public QDialog
{
	Q_OBJECT
	
	private:
		dbQueryServer *_req_server;
	
		QLabel *_login_lb;
		QLabel *_passwd_lb;
		QLabel *_confirm_lb;
		QLabel *_name_lb;
		QLabel *_sirname_lb;
		QLabel *_position_lb;
		
		QLineEdit *_login_ed;
		QLineEdit *_passwd_ed;
		QLineEdit *_confirm_ed;
		QLineEdit *_name_ed;
		QLineEdit *_sirname_ed;
		QLineEdit *_position_ed;
	
		QGroupBox *_user_gb;
	
		QCheckBox *_read_rb;
		QCheckBox *_write_rb;
		QCheckBox *_add_rb;
		QCheckBox *_sett_rb;
		QCheckBox *_print_rb;
	
		QPushButton *_ok;
		QPushButton *_cancel;
		
		QVBoxLayout *_gb_vl;
	
		QGridLayout *gl;
	public:
		rs_settings(QWidget *parent, dbQueryServer *req_server);
	public slots:
		void accept(void);
};

#endif /*RS_SETTINGS_H*/
