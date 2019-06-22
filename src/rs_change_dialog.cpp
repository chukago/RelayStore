#include "rs_change_dialog.h"

void rs_change_dialog::setConnections(void)
{
	QObject::connect(_cancel, SIGNAL(released(void)), this, SLOT(reject(void)));
	QObject::connect(_ok, SIGNAL(released(void)), this, SLOT(check_errors(void)));
	QObject::connect(_status_ed, SIGNAL(currentIndexChanged(QString)), this, SLOT(check_status(QString)));		// Изменение статуса
	QObject::connect(_last_check_ed, SIGNAL(dateChanged(QDate)), this, SLOT(check_last_check(QDate)));			// Изменение даты последней проверки
}

rs_change_dialog::rs_change_dialog(QWidget *parent, dbQueryServer *req_server, unsigned int id)
	:QDialog(parent)
{
	window()->setWindowTitle(tr("Изменить параметры"));
	
	_req_server = req_server;
	_id = id;
	
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
	
	_type_ed = new QLineEdit(this);
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
	
	_ok = new QPushButton(this);
	_ok->setText(tr("Ok"));
	_cancel = new QPushButton(this);
	_cancel->setText(tr("Отмена"));
	
    _manufactured_ed->setDisplayFormat("dd.MM.yyyy");
    _manufactured_ed->setEnabled(false);

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
	
	relayDesc_t _relay = _req_server->getRelayFromBuffer(_id);
	if (!_relay.id){
		_relay = _req_server->getRelayById(_id);
    }
	//QVector<typeDesc_t> _type = _req_server->getTypes();
	QVector<stationDesc_t> _station = _req_server->getStations();
	QVector<statusDesc_t> _status = _req_server->getStatuses();
	
	_type_ed->setText(_req_server->getTypeById(_relay.type).type);
	_type_ed->setEnabled(false);

	_period_ed->setText(QVariant(_req_server->getTypeById(_relay.type).checkPeriod).toString());
	_period_ed->setEnabled(false);
	
	_ser_num_ed->setText(_relay.serial_number);
	_ser_num_ed->setEnabled(false);
	
    QDateTime manufactured_dt;
    manufactured_dt.setTime_t(_relay.manufactured);
    _manufactured_ed->setDate(manufactured_dt.date());
	
	for (int i = 0; i < _station.size(); i++){
		_station_ed->addItem(_station[i].station);
		if (_station[i].id == _relay.station){
			_station_ed->setCurrentIndex(i);
        }
    }
	
	_relay_rack_ed->setText(QVariant(_relay.relay_rack).toString());
	
	_device_number_ed->setText(QVariant(_relay.device_number).toString());
	
	QDateTime last_check_dt;
	last_check_dt.setTime_t(_relay.last_check);
	_last_check_ed->setDateTime(last_check_dt);
	
	_next_check_ed->setText(last_check_dt.addYears(_req_server->getTypeById(_relay.type).checkPeriod).toString("dd.MM.yyyy"));
	
	for (int i = 0; i < _status.size(); i++){
		_status_ed->addItem(_status[i].status);
		if (_status[i].id == _relay.status){
			_status_ed->setCurrentIndex(i);
        }
    }
	
	switch (_relay.status){
	case RS_STATUS_INSTALL: // Установленно
		_relay_rack_ed->setEnabled(true);
		_device_number_ed->setEnabled(true);
		_station_ed->setEnabled(true);
		break;
	case RS_STATUS_STORE: // На складе
        case RS_STATUS_REPAIR: //Аварийный запас
		_relay_rack_ed->setEnabled(false);
		_device_number_ed->setEnabled(false);
		_station_ed->setEnabled(true);
		break;
	case RS_STATUS_KIP: // В КИП
		_relay_rack_ed->setEnabled(false);
		_device_number_ed->setEnabled(false);
		_station_ed->setEnabled(false);
		_last_check_ed->setEnabled(false);
		break;
    }
	
	_comment_ed->setText(_relay.comment);
	
	setConnections();
}

relayDesc_t rs_change_dialog::getRelay(void)
{
	return(_relay);
}

void rs_change_dialog::check_errors(void)
{	
	relayDesc_t relay = _req_server->getRelayById(_id);
	
	// Проверка ввода номера статива
	if ((_relay_rack_ed->text() == "" ) && (_status_ed->currentIndex() == 0)){
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

    /*
    // Проверка соответствия даты производства и проверки
	if (_manufactured_ed->dateTime().toTime_t() > _last_check_ed->dateTime().toTime_t()){
		QMessageBox *error_msg_box = new QMessageBox(this);
		error_msg_box->setAttribute(Qt::WA_DeleteOnClose);
		error_msg_box->setWindowTitle(tr("Проверка введённых данных!"));
		error_msg_box->setText(tr("Дата производства старше даты проверки!"));
		error_msg_box->setModal(true);
		error_msg_box->show();
		_device_number_ed->setFocus();
		return;
    }
    */

	// Дата производства
    // relay.manufactured = _manufactured_ed->dateTime().toTime_t();

	// Станция
	if (_station_ed->isEnabled()){
		relay.station = _req_server->getStationByName(_station_ed->currentText()).id;
	}
	else{
		relay.station = 0;
    }
	
	// Статив
	if (_relay_rack_ed->isEnabled()){
		relay.relay_rack = QVariant(_relay_rack_ed->text()).toUInt();
	}
	else{
		relay.relay_rack = 0;
    }
	
	// Номер прибора
	if (_device_number_ed->isEnabled()){
		relay.device_number = QVariant(_device_number_ed->text()).toUInt();
		}
	else{
		relay.device_number = 0;
    }
	
	// Последняя проверка
	relay.last_check = _last_check_ed->dateTime().toTime_t();
	
	// Статус
	relay.status = _req_server->getStatusByName(_status_ed->currentText()).id;
	
	// Комментарий
	relay.comment = _comment_ed->text();
	
	_req_server->addRelayToBuffer(relay);
	_relay = relay;
	accept();
}

void rs_change_dialog::check_status(QString status)
{
	switch (_req_server->getStatusByName(status).id){
	case RS_STATUS_INSTALL: // Установленно
		_relay_rack_ed->setEnabled(true);
		_device_number_ed->setEnabled(true);
		_station_ed->setEnabled(true);
		_last_check_ed->setEnabled(true);
		break;
	case RS_STATUS_STORE: // На складе
		_relay_rack_ed->setEnabled(false);
		_device_number_ed->setEnabled(false);
		_station_ed->setEnabled(true);
		_last_check_ed->setEnabled(true);
		break;
	case RS_STATUS_KIP: // В КИП
		_relay_rack_ed->setEnabled(false);
		_device_number_ed->setEnabled(false);
		_station_ed->setEnabled(false);
		_last_check_ed->setEnabled(false);
		break;
    }
}

void rs_change_dialog::check_last_check(QDate last_check)
{
	_next_check_ed->setText(last_check.addYears(QVariant(_period_ed->text()).toUInt()).toString("dd.MM.yyyy"));
}
