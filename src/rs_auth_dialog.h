#ifndef RS_AUTH_DIALOG
#define RS_AUTH_DIALOG

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QDebug>

class rs_auth_dialog : public QDialog
{
	Q_OBJECT
	
	private:
		QLineEdit *_login;
		QLineEdit *_passwd;
	
	public:
		rs_auth_dialog(QWidget *parent);
		
		/* Возвращает введённый логин */
		QString getLogin(void);
		
		/* Возвращает введённый пароль */
		QString getPassword(void);

                void keyPressEvent(QKeyEvent *event);
	
};

#endif /*RS_AUTH_DIALOG*/
