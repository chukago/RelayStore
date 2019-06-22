#include "rs_add_dialog.h"

void rs_add_dialog::setConnections(void)
{
        QObject::connect(_cancel, SIGNAL(released(void)), this, SLOT(reject(void)));				// Нажатие Ok
        QObject::connect(_ok, SIGNAL(released(void)), this, SLOT(check_errors(void)));				// Нажатие Cancel
        QObject::connect(_type_ed, SIGNAL(editTextChanged(QString)), this, SLOT(check_type(QString)));		// Изменение типа
        QObject::connect(_status_ed, SIGNAL(currentIndexChanged(QString)), this, SLOT(check_status(QString)));	// Изменение статуса
        QObject::connect(_last_check_ed, SIGNAL(dateChanged(QDate)), this, SLOT(check_last_check(QDate)));	// Изменение даты последней проверки
        QObject::connect(_manufactured_ed, SIGNAL(dateChanged(QDate)), this, SLOT(check_manufactured(QDate)));  // Изменение даты последней проверки
}

rs_add_dialog::rs_add_dialog(QWidget *parent, dbQueryServer *req_server)
	:QDialog(parent)
{	
	window()->setWindowTitle(tr("Добавить новое реле"));
	
	_req_server = req_server;
	
	_type_lb = new QLabel(tr("Тип"), this);
	_period_lb = new QLabel(tr("Период  проверки"), this);
	_ser_num_lb = new QLabel(tr("Серийный номер"), this);
	_manufactured_lb = new QLabel(tr("Дата изготовления"), this);
	_station_lb = new QLabel(tr("Установлен на станции"), this);
	_relay_rack_lb = new QLabel(tr("Номер статива"), this);
	_device_number_lb = new QLabel(tr("Номер прибора"), this);
	_last_check_lb = new QLabel(tr("Дата последней проверки"), this);
	_next_check_lb = new QLabel(tr("Дата следующей проверки"), this);
	_status_lb = new QLabel(tr("Статус"), this);
	_comment_lb = new QLabel(tr("Комментарий"), this);
	
	_type_ed = new QComboBox(this);
	_period_ed = new QLineEdit(this);
	_ser_num_ed = new QLineEdit(this);
	_manufactured_ed = new QDateEdit(this);
	_station_ed = new QComboBox(this);
	_relay_rack_ed = new QLineEdit(this);
	_device_number_ed = new QLineEdit(this);
	_last_check_ed = new QDateEdit(this);
	_next_check_ed = new QLineEdit(this);
	_status_ed = new QComboBox(this);
	_comment_ed = new QLineEdit(this);

        _type_ed->setFocusPolicy(Qt::StrongFocus);
        _ser_num_ed->setFocusPolicy(Qt::StrongFocus);
        _manufactured_ed->setFocusPolicy(Qt::StrongFocus);
        _station_ed->setFocusPolicy(Qt::StrongFocus);
        _relay_rack_ed->setFocusPolicy(Qt::StrongFocus);
        _device_number_ed->setFocusPolicy(Qt::StrongFocus);
        _last_check_ed->setFocusPolicy(Qt::StrongFocus);
        _status_ed->setFocusPolicy(Qt::StrongFocus);
        _comment_ed->setFocusPolicy(Qt::StrongFocus);
	
	_ok = new QPushButton(this);
	_ok->setText(tr("Ok"));
	_cancel = new QPushButton(this);
	_cancel->setText(tr("Отмена"));
	
        _type_ed->setEditable(true);
	
	_manufactured_ed->setDisplayFormat("dd.MM.yyyy");
	
	_last_check_ed->setDisplayFormat("dd.MM.yyyy");
	
	_next_check_ed->setEnabled(false);
	
	_gl = new QGridLayout(this);
	_gl->setColumnMinimumWidth(1, 250);
	
	_gl->addWidget(_type_lb, 0, 0);
	_gl->addWidget(_type_ed, 0, 1);
	_gl->addWidget(_period_lb, 1, 0);
	_gl->addWidget(_period_ed, 1, 1);
	_gl->addWidget(_ser_num_lb, 2, 0);
	_gl->addWidget(_ser_num_ed, 2, 1);
	_gl->addWidget(_manufactured_lb, 3, 0);
	_gl->addWidget(_manufactured_ed, 3, 1);
	_gl->addWidget(_station_lb, 4, 0);
	_gl->addWidget(_station_ed, 4, 1);
	_gl->addWidget(_relay_rack_lb, 5, 0);
	_gl->addWidget(_relay_rack_ed, 5, 1);
	_gl->addWidget(_device_number_lb, 6, 0);
	_gl->addWidget(_device_number_ed, 6, 1);
	_gl->addWidget(_last_check_lb, 7, 0);
	_gl->addWidget(_last_check_ed, 7, 1);
	_gl->addWidget(_next_check_lb, 8, 0);
	_gl->addWidget(_next_check_ed, 8, 1);
	_gl->addWidget(_status_lb, 9, 0);
	_gl->addWidget(_status_ed, 9, 1);
	_gl->addWidget(_comment_lb, 10, 0);
	_gl->addWidget(_comment_ed, 10, 1);
	
	_gl->addWidget(_ok, 11, 0);
	_gl->addWidget(_cancel, 11, 1);
	
	setLayout(_gl);

	QVector<typeDesc_t> _types = _req_server->getTypes();
	QVector<stationDesc_t> _stations = _req_server->getStations();
	QVector<statusDesc_t> _statuses = _req_server->getStatuses();
	
	
        if (_types.size()){
            for (int i = 0; i < _types.size(); i++){
                _type_ed->addItem(_types[i].type);
            }

            _period_ed->setText(QVariant(_types[0].checkPeriod).toString());
            _period_ed->setEnabled(false);
        }

        for (int i = 0; i < _stations.size(); i++){
		_station_ed->addItem(_stations[i].station);
    }
	
	for (int i = 0; i < _statuses.size(); i++){
		_status_ed->addItem(_statuses[i].status);
    }
	
        setConnections();
}

void rs_add_dialog::check_errors(void)
{
	
	// Проверка ввода типа реле
	if (_type_ed->currentText() == ""){
		QMessageBox *error_msg_box = new QMessageBox(this);
		error_msg_box->setAttribute(Qt::WA_DeleteOnClose);
		error_msg_box->setWindowTitle(tr("Проверка введённых данных!"));
		error_msg_box->setText(tr("Введите тип реле!"));
		error_msg_box->setModal(true);
		error_msg_box->show();
		_type_ed->setFocus();
		return;
    }
	
	// Проверка ввода периода проверки
	if (_period_ed->text() == ""){
		QMessageBox *error_msg_box = new QMessageBox(this);
		error_msg_box->setAttribute(Qt::WA_DeleteOnClose);
		error_msg_box->setWindowTitle(tr("Проверка введённых данных!"));
		error_msg_box->setText(tr("Введите период проверки!"));
		error_msg_box->setModal(true);
		error_msg_box->show();
		_period_ed->setFocus();
		return;
    }
	
	// Проверка ввода серийного номера
	if (_ser_num_ed->text() == ""){
		QMessageBox *error_msg_box = new QMessageBox(this);
		error_msg_box->setAttribute(Qt::WA_DeleteOnClose);
		error_msg_box->setWindowTitle(tr("Проверка введённых данных!"));
		error_msg_box->setText(tr("Введите серийный номер!"));
		error_msg_box->setModal(true);
		error_msg_box->show();
		_ser_num_ed->setFocus();
		return;
    }
	
	// Проверка ввода номера статива
	if (((_relay_rack_ed->text() == "" ) && (_status_ed->currentIndex() == 0))){
		QMessageBox *error_msg_box = new QMessageBox(this);
		error_msg_box->setAttribute(Qt::WA_DeleteOnClose);
		error_msg_box->setWindowTitle(tr("Проверка введённых данных!"));
		error_msg_box->setText(tr("Введите номер статива!"));
		error_msg_box->setModal(true);
		error_msg_box->show();
		_relay_rack_ed->setFocus();
		return;
    }
	
	// Проверка ввода номера прибора
	if ((_device_number_ed->text() == "") && (_status_ed->currentIndex() == 0)){
		QMessageBox *error_msg_box = new QMessageBox(this);
		error_msg_box->setAttribute(Qt::WA_DeleteOnClose);
		error_msg_box->setWindowTitle(tr("Проверка введённых данных!"));
		error_msg_box->setText(tr("Введите номер прибора!"));
		error_msg_box->setModal(true);
		error_msg_box->show();
		_device_number_ed->setFocus();
		return;
    }
	
	if ( _req_server->getTypeByName(_type_ed->currentText()).id){
                // Проверка существования реле с заданным типом, датой производства и серийным номером
        findParam_t findParam = {0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, NULL};
		findParam.type = _req_server->getTypeByName(_type_ed->currentText()).id;
		findParam.serial_number = _ser_num_ed->text();
                findParam.manufactured = _manufactured_ed->dateTime().toTime_t();
		if (_req_server->findRelay(findParam).size() == 1)
		{
			QMessageBox *error_msg_box = new QMessageBox(this);
			error_msg_box->setAttribute(Qt::WA_DeleteOnClose);
			error_msg_box->setWindowTitle(tr("Проверка введённых данных!"));
			error_msg_box->setText(tr("Реле существует в базе данных!"));
			error_msg_box->setModal(true);
			error_msg_box->show();
			return;
        }
	}
	else{
            QMessageBox *error_msg_box = new QMessageBox(this);
            error_msg_box->setAttribute(Qt::WA_DeleteOnClose);
            error_msg_box->setWindowTitle(tr("Проверка введённых данных!"));
            error_msg_box->setText(tr("Несуществующий тип реле!"));
            error_msg_box->setModal(true);
            error_msg_box->show();
            return;
    }
	
	// Проверка "дата производства" <= "текущая дата"
	if (_manufactured_ed->date() > _manufactured_ed->date().currentDate())
	{
		QMessageBox *error_msg_box = new QMessageBox(this);
		error_msg_box->setAttribute(Qt::WA_DeleteOnClose);
		error_msg_box->setWindowTitle(tr("Проверка введённых данных!"));
		error_msg_box->setText(tr("Дата производства не может быть больше текущей даты!"));
		error_msg_box->setModal(true);
		error_msg_box->show();
		return;
    }
	
	// Проверка "дата последней проверки" <= "текущая дата"
	if (_last_check_ed->date() > _last_check_ed->date().currentDate())
	{
		QMessageBox *error_msg_box = new QMessageBox(this);
		error_msg_box->setAttribute(Qt::WA_DeleteOnClose);
		error_msg_box->setWindowTitle(tr("Проверка введённых данных!"));
		error_msg_box->setText(tr("Дата последней проверки не может быть больше текущей даты!"));
		error_msg_box->setModal(true);
		error_msg_box->show();
		return;
    }
	
	// Проверка "дата_производства" <= "дата последней проверки"
	if (_manufactured_ed->date() > _last_check_ed->date())
	{
		QMessageBox *error_msg_box = new QMessageBox(this);
		error_msg_box->setAttribute(Qt::WA_DeleteOnClose);
		error_msg_box->setWindowTitle(tr("Проверка введённых данных!"));
		error_msg_box->setText(tr("Дата последней проверки не может быть меньше даты производства!"));
		error_msg_box->setModal(true);
		error_msg_box->show();
		return;
    }
	
	QDateTime intermediate;
	QDateTime manufactured_dt, last_check_dt;
    relayDesc_t relay = {0, 0, NULL, 0, 0, 0, 0, 0, 0, NULL};
	
	relay.type = _req_server->getTypeByName(_type_ed->currentText()).id;
	relay.serial_number = _ser_num_ed->text();
	
	manufactured_dt.setDate(_manufactured_ed->date());
	relay.manufactured = manufactured_dt.toTime_t();
	
	if (_station_ed->isEnabled()){
		relay.station = _req_server->getStationByName(_station_ed->currentText()).id;
	}
	else{
		relay.station = 0;
    }
	
	if ((_relay_rack_ed->text() != "") && (_relay_rack_ed->isEnabled()))
	{
		relay.relay_rack = _relay_rack_ed->text().toUInt();
	}
	else
	{
		relay.relay_rack = 0;
    }
	
	if ((_device_number_ed->text() != "") && (_device_number_ed->isEnabled()))
	{
		relay.device_number = _device_number_ed->text().toUInt();
	}
	else
	{
		relay.device_number = 0;
    }
	
	last_check_dt.setDate(_last_check_ed->date());
	if (last_check_dt < manufactured_dt){
		relay.last_check = 0;
	}
	else{
		relay.last_check = last_check_dt.toTime_t();
    }
	
	relay.status = _req_server->getStatusByName(_status_ed->currentText()).id;
	
	relay.comment = _comment_ed->text();
	
	_req_server->addRelayToBuffer(relay);
	_req_server->writeToDb();
	
	accept();
}

void rs_add_dialog::check_type(QString type)
{
	typeDesc_t _type = _req_server->getTypeByName(type);
	
	if (_type.id !=0){
	    _period_ed->setText(QVariant(_type.checkPeriod).toString());
	    _period_ed->setEnabled(false);
	    return;
    }
}

void rs_add_dialog::check_status(QString status)
{
    switch (_req_server->getStatusByName(status).id){
    case RS_STATUS_INSTALL: // Установленно
	_relay_rack_ed->setEnabled(true);
	_device_number_ed->setEnabled(true);
	_station_ed->setEnabled(true);
	break;
    case RS_STATUS_STORE: // На складе
    case RS_STATUS_REPAIR:
	_relay_rack_ed->setEnabled(false);
	_device_number_ed->setEnabled(false);
	_station_ed->setEnabled(true);
	break;
    case RS_STATUS_KIP: // В КИП
	_relay_rack_ed->setEnabled(false);
	_device_number_ed->setEnabled(false);
	_station_ed->setEnabled(false);
	break;
    }
    return;
}

void rs_add_dialog::check_manufactured(QDate manufactured)
{
	if (manufactured > _last_check_ed->date()){
		_last_check_ed->setDate(manufactured);
    }
}

void rs_add_dialog::check_last_check(QDate last_check)
{
    if (_period_ed->text().toUInt()){
        _next_check_ed->setText(last_check.addYears(QVariant(_period_ed->text()).toUInt()).toString("dd.MM.yyyy"));
    }
    else{
        _next_check_ed->setText("");
    }
}

void rs_add_dialog::keyPressEvent(QKeyEvent *key_event)
{
    switch(key_event->key()){
        case Qt::Key_Return:
        break;
    }
}
