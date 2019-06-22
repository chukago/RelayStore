#include "rs_main_table.h"

// Наименование столбцов таблицы

#define ID_COLUMN				0
#define TYPE_COLUMN			1
#define CHECK_PERIOD_COLUMN	2
#define SERIAL_NUMBER_COLUMN	3
#define MANUFACTURED_COLUMN	4
#define STATION_COLUMN		5
#define RELAY_RACK_COLUMN		6
#define DEVICE_NUMBER_COLUMN	7
#define LAST_CHECK_COLUMN		8
#define NEXT_CHECK_COLUMN		9
#define STATUS_COLUMN			10
#define COMMENT_COLUMN		11

void rs_main_table::setConnections(void){
	QObject::connect(this, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(_item_double_clicked(int, int)));
}

void rs_main_table::_table_setup(void)
{
	QVector<relayDesc_t> relays = _req_server->getRelays();
	
	setColumnCount(12);
	setRowCount(relays.size());

	QStringList _horizontal_header;
	_horizontal_header	<< tr("ИД")
					<< tr("Тип") 
					<< tr("Период\nпроверки")
					<< tr("Серийный\nномер")
					<< tr("Дата\nпроизводства")
					<< tr("Станция")
					<< tr("Номер\nстатива")
					<< tr("Номер\nприбора")
					<< tr("Последняя\nпроверка")
					<< tr("Следующая\nпроверка")
					<< tr("Статус")
					<< tr("Комментарий");
	setHorizontalHeaderLabels(_horizontal_header);
	
	_table_row_type _row;
	_items.resize(relays.size());
	
	for (int row = 0; row < relays.size(); row++){
		
		_row.id = new QTableWidgetItem();
		_row.type = new QTableWidgetItem();
		_row.check_period = new QTableWidgetItem();
		_row.serial_number = new QTableWidgetItem();
		_row.manufactured = new QTableWidgetItem();
		_row.station = new QTableWidgetItem();
		_row.relay_rack = new QTableWidgetItem();
		_row.device_number = new QTableWidgetItem();
		_row.last_check = new QTableWidgetItem();
		_row.next_check = new QTableWidgetItem();
		_row.status = new QTableWidgetItem();
		_row.comment = new QTableWidgetItem();
		
		/* Флаги для ячеек по умолчанию */
		Qt::ItemFlags row_default_flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		
		_row.id->setFlags(row_default_flags);
		_row.type->setFlags(row_default_flags);
		_row.check_period->setFlags(row_default_flags);
		_row.serial_number->setFlags(row_default_flags);
		_row.manufactured->setFlags(row_default_flags);
		_row.station->setFlags(row_default_flags);
		_row.relay_rack->setFlags(row_default_flags);
		_row.device_number->setFlags(row_default_flags);
		_row.last_check->setFlags(row_default_flags);
		_row.next_check->setFlags(row_default_flags);
		_row.status->setFlags(row_default_flags);
		_row.comment->setFlags(row_default_flags);
		
		_items[row] = _row;
    }
	
	for (int row = 0; row < relays.size(); row++){
		setItem(row, ID_COLUMN,				_items[row].id);
		setItem(row, TYPE_COLUMN,			_items[row].type);
		setItem(row, CHECK_PERIOD_COLUMN,	_items[row].check_period);
		setItem(row, SERIAL_NUMBER_COLUMN,	_items[row].serial_number);
		setItem(row, MANUFACTURED_COLUMN,	_items[row].manufactured);
		setItem(row, STATION_COLUMN,		_items[row].station);
		setItem(row, RELAY_RACK_COLUMN,		_items[row].relay_rack);
		setItem(row, DEVICE_NUMBER_COLUMN,	_items[row].device_number);
		setItem(row, LAST_CHECK_COLUMN,		_items[row].last_check);
		setItem(row, NEXT_CHECK_COLUMN,	_items[row].next_check);
		setItem(row, STATUS_COLUMN,		_items[row].status);
		setItem(row, COMMENT_COLUMN,		_items[row].comment);
    }

    this->hideColumn(0);

}

void rs_main_table::_write_table(void)
{
	/*Для отметки ячеек таблицы красным цветом*/
	QColor table_warning_color = QColor("red");
	QBrush table_warning = QBrush(table_warning_color);
	
	QVector<relayDesc_t> relays = _req_server->getRelays();
	
	for (int row = 0; row < relays.size(); row++){
		writeRow(row, relays[row]);
    }
	
	resizeColumnsToContents();
	//resizeRowsToContents();
}

rs_main_table::rs_main_table(QWidget *parent, dbQueryServer *req_server)
	:QTableWidget(parent)
{
    manager = logic_manager::instance();
        _locale = new QLocale(QLocale::Russian);
	_readOnly = false;
	_req_server = req_server;
	setConnections();
}

void rs_main_table::clear(void)
{
	for (int i = 0; i < _items.size(); i++){
		delete(_items[i].id);				// Идентификатор по базе данных
		delete(_items[i].type);				// Тип
		delete(_items[i].check_period);		// Период проверки
        delete(_items[i].serial_number);    // Серийный номер
		delete(_items[i].manufactured);		// Дата изготовления
		delete(_items[i].station);			// Станция
        delete(_items[i].relay_rack);		// Номер статива
        delete(_items[i].device_number);	// Номер прибора
        delete(_items[i].last_check);		// Дата последней проверки
        delete(_items[i].next_check);		// Дата следующей проверки
		delete(_items[i].status);			// Статус
		delete(_items[i].comment);			// Комментарий
    }
	_items.clear();
	clearContents();
}

void rs_main_table::new_request(void)
{
	clear();
	_table_setup();
	_write_table();
}

void rs_main_table::_item_double_clicked(int row_clicked, int)
{	
	//relayDesc_t relay;
	if (!_readOnly){
		rs_change_dialog *change_dialog = new rs_change_dialog(this, _req_server, item(row_clicked, ID_COLUMN)->text().toUInt());
		change_dialog->setModal(true);
		if (change_dialog->exec() == QDialog::Accepted){
            writeRow(row_clicked, change_dialog->getRelay());
            manager->setState(MAIN_TABLE_HAVE_CHANGE);
            QAction *saveAction = static_cast<QAction*>(manager->getObject("save_action"));
            saveAction->setEnabled(true);

            //rs_main_tool_panel *tool_panel= static_cast<rs_main_tool_panel*>(manager->getWidget("rs_main_tool_panel"));
            //tool_panel->getButton(TOOL_BUTTON_SAVE)->setEnabled(true);
        }

        delete(change_dialog);
    }
}

void rs_main_table::writeRow(int row, relayDesc_t relay)
{
	QColor table_warning_color = QColor("red");
	QBrush table_warning = QBrush(table_warning_color);
	QColor table_changed_color = QColor("yellow");
	QBrush table_changed = QBrush(table_changed_color);
	relayDesc_t _currentRelay = _req_server->getRelayById(relay.id);
	
	/*Идентификатор*/
	_items[row].id->setText(QVariant(relay.id).toString());
	
	/*Тип*/
	_items[row].type->setText(_req_server->getTypeById(relay.type).type);
	
    /*Период проверки*/

    uint _check_period = _alt_check_period(_currentRelay);

    if (_check_period != 0) {

        _items[row].check_period->setText(QVariant(_check_period).toString() + tr(" (альт.)"));
    }
    else {

        _check_period = _req_server->getTypeById(relay.type).checkPeriod;

        if (_check_period != 0) {

            _items[row].check_period->setText(QVariant(_req_server->getTypeById(relay.type).checkPeriod).toString());
        }
        else {

            _items[row].check_period->setText(tr("Не проверяется"));
        }
    }
		
	/*Серийный номер*/
	_items[row].serial_number->setText(relay.serial_number);
	
    //Дата производства
	if (relay.manufactured == 0){
		_items[row].manufactured->setText(tr("Неизвестна!"));
	}
	else{
		QDateTime manufactured_dt;
		manufactured_dt.setTime_t(relay.manufactured);
                //_items[row].manufactured->setText(manufactured_dt.date().toString("dd.MM.yyyy"));
                _items[row].manufactured->setText(_locale->toString(manufactured_dt.date(), "dd.MM.yyyy"));
    }
	
	/*Станция*/
	_items[row].station->setText(_req_server->getStationById(relay.station).station);
	
	/*Номер статива*/
	if (relay.relay_rack > 0){
		_items[row].relay_rack->setText(QVariant(relay.relay_rack).toString());
	}
	else{
		_items[row].relay_rack->setText("");
    }
	
	/*Номер прибора*/
	if (relay.device_number >0){
		_items[row].device_number->setText(QVariant(relay.device_number).toString());
	}
	else{
		_items[row].device_number->setText("");
    }
		
	QDateTime last_check_dt;
	last_check_dt.setTime_t(relay.last_check);
	
	/*Дата последней проверки*/
        if (relay.last_check == 0){
		_items[row].last_check->setText(tr("Не задана!"));
                if ((relay.status == RS_STATUS_INSTALL)||(relay.status == RS_STATUS_REPAIR)){
                    _items[row].last_check->setBackground(table_warning);
                }
	}
	else{
                //_items[row].last_check->setText(last_check_dt.toString("dd.MM.yyyy"));
                _items[row].last_check->setText(_locale->toString(last_check_dt.date(), "dd.MM.yyyy"));
                _items[row].last_check->setBackground(item(0,0)->background());
    }
	
    /*Дата следующей проверки*/
        if (_check_period == 0) {
        }
        else if (relay.last_check != 0) {
            //_items[row].next_check->setText(last_check_dt.addYears(_check_period).toString("dd.MM.yyyy"));
            _items[row].next_check->setText(_locale->toString(last_check_dt.addYears(static_cast<int>(_check_period)).date(), "dd.MM.yyyy"));

            if (last_check_dt.addYears(static_cast<int>(_check_period)) < QDateTime::currentDateTime())
                //&& ((relay.status == RS_STATUS_INSTALL)||(relay.status == RS_STATUS_REPAIR))){
            {
                _items[row].next_check->setBackground(table_warning);
            }
            else{
                _items[row].next_check->setBackground(item(0,0)->background());
            }
        }
	
	/*Статус*/
	_items[row].status->setText(_req_server->getStatusById(relay.status).status);
	
	switch (relay.status){
		case RS_STATUS_STORE:
                case RS_STATUS_REPAIR:
			_items[row].relay_rack->setText("");
			_items[row].device_number->setText("");
			break;
		case RS_STATUS_KIP:
			_items[row].station->setText("");
			_items[row].relay_rack->setText("");
			_items[row].device_number->setText("");
			break;
    }
	
	/*Комментарий*/
	_items[row].comment->setText(relay.comment);
	
	if (_currentRelay.manufactured != relay.manufactured)
	{
	    _items[row].manufactured->setBackground(table_changed);
	}

	if (_currentRelay.station != relay.station){
		_items[row].station->setBackground(table_changed);
    }
	
	if (_currentRelay.relay_rack != relay.relay_rack){
		_items[row].relay_rack->setBackground(table_changed);
    }
	
	if (_currentRelay.device_number != relay.device_number){
		_items[row].device_number->setBackground(table_changed);
    }
	
	if (_currentRelay.last_check != relay.last_check){
		_items[row].last_check->setBackground(table_changed);
    }
	
        if (_currentRelay.status != relay.status){
                _items[row].status->setBackground(table_changed);
        }

        if (_currentRelay.comment != relay.comment){
                _items[row].comment->setBackground(table_changed);
        }

        setCurrentItem(_items[row].id);
}

void rs_main_table::setReadOnly(void)
{
	_readOnly = true;
}

uint rs_main_table::_alt_check_period(relayDesc_t &relay)
{
    QRegExp re(tr("\\((\\d+)\\).*$"));
    int pos=re.indexIn(relay.comment, 0);
    if (pos != -1){
        return(re.cap(1).toUInt());
    }
    return(0);
}
