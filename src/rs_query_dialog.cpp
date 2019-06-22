#include "rs_query_dialog.h"

void rs_query_dialog::setConnections(void)
{
    QObject::connect(_next_check_from_ck, SIGNAL(stateChanged(int)), this, SLOT(next_check_from_enable(int)));
	QObject::connect(_next_check_before_ck, SIGNAL(stateChanged(int)), this, SLOT(next_check_before_enable(int)));
	QObject::connect(_ok, SIGNAL(released(void)), this, SLOT(request(void)));
	QObject::connect(_cancel, SIGNAL(released(void)), this, SLOT(reject(void)));
}

rs_query_dialog::rs_query_dialog(QWidget *parent, dbQueryServer *req_server)
	:QDialog(parent)
{
	window()->setWindowTitle(tr("Поиск реле"));
	
	_req_server = req_server;
	
    //_id_lb = new QLabel(tr("ИД"), this);
	_type_lb = new QLabel(tr("Тип"), this);
	_ser_num_lb = new QLabel(tr("Серийный номер"), this);
	_station_lb = new QLabel(tr("Станция"), this);
	_relay_rack_lb = new QLabel(tr("Статив"), this);
	_device_number_lb = new QLabel(tr("Номер прибора"), this);
        _next_check_from_lb = new QLabel(tr("Следующая проверка от"), this);
	_next_check_before_lb = new QLabel(tr("Следующая проверка до"), this);
	_status_lb = new QLabel(tr("Статус"), this);
	_sort_lb = new QLabel(tr("Сортировка"), this);
	
    //_id_ed = new QLineEdit(this);
	_type_ed = new QComboBox(this);
        _type_ed->setEditable(true);
	_ser_num_ed = new QLineEdit(this);
	_station_ed = new QComboBox(this);
	_relay_rack_ed = new QLineEdit(this);
	_device_number_ed = new QLineEdit(this);
        _next_check_from_ed = new QDateEdit(this);
	_next_check_before_ed = new QDateEdit(this);
	_status_ed = new QComboBox(this);

        _next_check_from_ck = new QCheckBox(this);
	_next_check_before_ck = new QCheckBox(this);
	
        _next_check_from_ed->setDisplayFormat("dd.MM.yyyy");
	_next_check_before_ed->setDisplayFormat("dd.MM.yyyy");
	
	_sort_ck = new QCheckBox(this);

	_ok = new QPushButton(this);
	_ok->setText(tr("Ok"));
	
	_cancel = new QPushButton(this);
	_cancel->setText(tr("Cancel"));
	
	_gl = new QGridLayout(this);
	
    //_gl->addWidget(_id_lb, 0, 0);
    _gl->addWidget(_type_lb, 0, 0);
    _gl->addWidget(_ser_num_lb, 1, 0);
    _gl->addWidget(_station_lb, 2, 0);
    _gl->addWidget(_relay_rack_lb, 3, 0);
    _gl->addWidget(_device_number_lb, 4, 0);
    _gl->addWidget(_next_check_from_lb, 5, 0);
    _gl->addWidget(_next_check_before_lb, 6, 0);
    _gl->addWidget(_status_lb, 7, 0);
	
    //_gl->addWidget(_id_ed, 0, 1);
    _gl->addWidget(_type_ed, 0, 1);
    _gl->addWidget(_ser_num_ed, 1, 1);
    _gl->addWidget(_station_ed, 2, 1);
    _gl->addWidget(_relay_rack_ed, 3, 1);
    _gl->addWidget(_device_number_ed, 4, 1);
    _gl->addWidget(_next_check_from_ed, 5, 1);
    _gl->addWidget(_next_check_before_ed, 6, 1);
    _gl->addWidget(_status_ed, 7, 1);
	
    _gl->addWidget(_next_check_from_ck, 5, 2);
    _gl->addWidget(_next_check_before_ck, 6, 2);

    _gl->addWidget(_sort_ck, 8, 1);
    _gl->addWidget(_sort_lb, 8, 0);
	
    _gl->addWidget(_ok, 9, 0);
    _gl->addWidget(_cancel, 9, 1);
	
        _next_check_from_ed->setEnabled(false);
        _next_check_before_ed->setEnabled(false);
        //_relay_rack_ed->setEnabled(false);
        //_device_number_ed->setEnabled(false);
	
	_type_ed->addItem("*");
	_station_ed->addItem("*");
	_status_ed->addItem("*");
	
	QVector<typeDesc_t> types = _req_server->getTypes();
	for (int i = 0; i < types.size(); i++){
		_type_ed->addItem(types[i].type);
    }
	
	QVector<stationDesc_t> stations = _req_server->getStations();
	for (int i = 0; i < stations.size(); i++){
		_station_ed->addItem(stations[i].station);
    }
	
	QVector<statusDesc_t> statuses = _req_server->getStatuses();
	for (int i = 0; i < statuses.size(); i++){
		_status_ed->addItem(statuses[i].status);
    }
	
	setConnections();
	
}


void rs_query_dialog::next_check_from_enable(int state)
{
        if (state == Qt::Checked){
                _next_check_from_ed->setEnabled(true);
                return;
        }
        _next_check_from_ed->setEnabled(false);
}

void rs_query_dialog::next_check_before_enable(int state)
{
	if (state == Qt::Checked){
		_next_check_before_ed->setEnabled(true);
		return;
    }
	_next_check_before_ed->setEnabled(false);
}

void rs_query_dialog::request(void)
{
    findParam_t findParam = {0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, 0, nullptr};
	
	/*ИД*/
    //findParam.id			=	QVariant(_id_ed->text()).toInt();

	/*Тип*/
	findParam.type			=	_req_server->getTypeByName(_type_ed->currentText()).id;
	
	/*Серийный номер*/
	if (!_ser_num_ed->text().isEmpty()){
		findParam.serial_number		=	_ser_num_ed->text();
    }
	
	/*Номер станции*/
	findParam.station			=	_req_server->getStationByName(_station_ed->currentText()).id;
	
	/*Номер статива*/
	findParam.relay_rack		=	_relay_rack_ed->text().toUInt();
	
	/*Номер прибора*/
	findParam.device_number	=	_device_number_ed->text().toUInt();
	

        /*Дата следующей проверки от*/
        if (_next_check_from_ed->isEnabled() && _next_check_from_ed->dateTime().isValid()){
                findParam.from_date		=	_next_check_from_ed->dateTime().toTime_t();
        }

        /*Дата следующей проверки до*/
	if (_next_check_before_ed->isEnabled() && _next_check_before_ed->dateTime().isValid()){
		findParam.before_date		=	_next_check_before_ed->dateTime().toTime_t();
    }
	
	/*Статус*/
	findParam.status			=	_req_server->getStatusByName(_status_ed->currentText()).id;
	
	if (_sort_ck->isChecked())
	{
	    _req_server->findRelay(findParam, true, true);
	}
	else
	{
	    _req_server->findRelay(findParam, true);
	}
	
	accept();
}
