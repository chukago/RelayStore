#include "rs_auth_dialog.h"

rs_auth_dialog::rs_auth_dialog(QWidget *parent)
	:QDialog(parent)
{
	window()->setWindowTitle(tr("Авторизация"));
	QLabel *login_lb = new QLabel(tr("Логин"), this);
	QLabel *passwd_lb = new QLabel(tr("Пароль"), this);
	
	_login = new QLineEdit(this);
	_passwd = new QLineEdit(this);
	_passwd->setEchoMode(QLineEdit::Password);
	
	QPushButton *ok_button = new QPushButton(this);
	ok_button->setText(tr("Да"));
	
	QPushButton *cancel_button = new QPushButton(this);
	cancel_button->setText(tr("Отменить"));
	
	QGridLayout *grid_layout = new QGridLayout(this);
	
	grid_layout->addWidget(login_lb, 0, 0);
	
	grid_layout->addWidget(_login, 0 ,1);
	
	grid_layout->addWidget(passwd_lb, 1, 0);
	
	grid_layout->addWidget(_passwd, 1, 1);
	
	grid_layout->addWidget(ok_button, 2, 0);
	
	grid_layout->addWidget(cancel_button, 2, 1);
	
	setLayout(grid_layout);
	
	QObject::connect(ok_button, SIGNAL(clicked()), this, SLOT(accept()));
	QObject::connect(cancel_button, SIGNAL(clicked()), this, SLOT(reject()));
	
}

QString rs_auth_dialog::getLogin(void)
{
	return (_login->text()); 
}

QString rs_auth_dialog::getPassword(void)
{
	return (_passwd->text());
}

void rs_auth_dialog::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Return:
    case Qt::Key_Enter:

        if(focusWidget() == _login)
        {
            _passwd->setFocus();
            break;
        }

        if(focusWidget() == _passwd)
        {
            accept();
            break;
        }

        break;
    }
}
