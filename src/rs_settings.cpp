#include "rs_settings.h"

rs_settings::rs_settings(QWidget *parent, dbQueryServer *req_server)
	:QDialog(parent)
{
	_req_server = req_server;
	
	window()->setWindowTitle(tr("Настройки"));
	
	_login_lb = new QLabel(tr("Логин*"), this);
	_passwd_lb = new QLabel(tr("Пароль*"), this);
	_confirm_lb = new QLabel(tr("Подтверждение*"), this);
	_name_lb = new QLabel(tr("Имя*"), this);
	_sirname_lb = new QLabel(tr("Фамилия*"), this);
	_position_lb = new QLabel(tr("Должность*"), this);
	
	_login_ed = new QLineEdit(this);
	_passwd_ed = new QLineEdit(this);
	_passwd_ed->setEchoMode(QLineEdit::Password);
	_confirm_ed = new QLineEdit(this);
	_confirm_ed->setEchoMode(QLineEdit::Password);
	_name_ed = new QLineEdit(this);
	_sirname_ed = new QLineEdit(this);
	_position_ed = new QLineEdit(this);
	
	
	_user_gb = new QGroupBox(tr("Права"), this);
	_read_rb = new QCheckBox(tr("Чтение"), this);
	_write_rb = new QCheckBox(tr("Запись"), this);
	_add_rb = new QCheckBox(tr("Добавление"), this);
	_sett_rb = new QCheckBox(tr("Настройки"), this);
	_print_rb = new QCheckBox(tr("Печать"), this);
	_ok = new QPushButton(tr("Добавить"), this);
	_cancel = new QPushButton(tr("Отмена"), this);
	
	_gb_vl = new QVBoxLayout();
	
	_gb_vl->addWidget(_read_rb);
	_gb_vl->addWidget(_write_rb);
	_gb_vl->addWidget(_add_rb);
	_gb_vl->addWidget(_sett_rb);
	_gb_vl->addWidget(_print_rb);
	
	_user_gb->setLayout(_gb_vl);
	
	gl = new QGridLayout();
	
	gl->addWidget(_login_lb, 0, 0);
	gl->addWidget(_login_ed, 0, 1);
	gl->addWidget(_passwd_lb, 1, 0);
	gl->addWidget(_passwd_ed, 1, 1);
	gl->addWidget(_confirm_lb, 2, 0);
	gl->addWidget(_confirm_ed, 2, 1);
	gl->addWidget(_name_lb, 3, 0);
	gl->addWidget(_name_ed, 3, 1);
	gl->addWidget(_sirname_lb, 4, 0);
	gl->addWidget(_sirname_ed, 4, 1);
	gl->addWidget(_position_lb, 5, 0);
	gl->addWidget(_position_ed, 5, 1);
	gl->addWidget(_user_gb, 6, 0, 1, 2);
	gl->addWidget(_ok, 7, 0);
	gl->addWidget(_cancel, 7, 1);
	
	setLayout(gl);
	QObject::connect(_ok, SIGNAL(released(void)), this, SLOT(accept(void)));
	QObject::connect(_cancel, SIGNAL(released(void)), this, SLOT(reject(void)));
}

void rs_settings::accept(void)
{
	if (_login_ed->text().isEmpty() || _passwd_ed->text().isEmpty() || _confirm_ed->text().isEmpty() || _name_ed->text().isEmpty() ||
		_sirname_ed->text().isEmpty() || _position_ed->text().isEmpty()){
		QMessageBox *fail_message = new QMessageBox(this);
		fail_message->window()->setWindowTitle(tr("Внимание!"));
		fail_message->setText(tr("Пожалуйста, заполните все поля!"));
		fail_message->setAttribute(Qt::WA_DeleteOnClose);
		fail_message->exec();
		return;
    }
	if (_passwd_ed->text().size() < 6){
		QMessageBox *fail_message = new QMessageBox(this);
		fail_message->window()->setWindowTitle(tr("Внимание!"));
		fail_message->setText(tr("Введите пароль не менее 6 символов!"));
		fail_message->setAttribute(Qt::WA_DeleteOnClose);
		fail_message->exec();
		return;
    }
	if (_passwd_ed->text() != _confirm_ed->text()){
		QMessageBox *fail_message = new QMessageBox(this);
		fail_message->window()->setWindowTitle(tr("Внимание!"));
		fail_message->setText(tr("Пароль и подтверждение не совпадают!"));
		fail_message->setAttribute(Qt::WA_DeleteOnClose);
		fail_message->exec();
		return;
    }
	if (_req_server->isUser(_login_ed->text())){
		QMessageBox *fail_message = new QMessageBox(this);
		fail_message->window()->setWindowTitle(tr("Внимание!"));
		fail_message->setText(tr("Пользователь существует!"));
		fail_message->setAttribute(Qt::WA_DeleteOnClose);
		fail_message->exec();
		return;
    }
	unsigned int rights = 0;
	if (_read_rb->isChecked()){
		rights |= RS_READ;
    }
	if (_write_rb->isChecked()){
		rights |= RS_WRITE;
    }
	if (_add_rb->isChecked()){
		rights |= RS_ADD;
    }
	if (_sett_rb->isChecked()){
		rights |= RS_SETT;
    }
	if (_print_rb->isChecked()){
		rights |= RS_PRINT;
    }
    userDesc_t newUser = {0, _login_ed->text(), _passwd_ed->text(), _name_ed->text(), _sirname_ed->text(), _position_ed->text(), rights};
	if (_req_server->addUser(newUser).id){
		QMessageBox *success_message = new QMessageBox(this);
		success_message->window()->setWindowTitle(tr("Внимание!"));
		success_message->setText(tr("Пользователь успешно зарегистрирован!"));
		success_message->setAttribute(Qt::WA_DeleteOnClose);
		success_message->exec();
    }
	QDialog::accept();
}
