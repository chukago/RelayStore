#include "rs_history.h"

rs_history::rs_history(QWidget *parent, dbQueryServer *req_server, unsigned int id)
	:QDialog(parent)
{
	setMinimumSize(parent->width() / 2, parent->height() / 2);
	window()->setWindowTitle(tr("История реле"));
	QVector<historyDesc_t> history = req_server->findHistory(id);
	//setWindowState(Qt::WindowMaximized);
	_info = new QLabel(this);
	_table = new QTableWidget(this);
	_layout = new QVBoxLayout();
	_layout->addWidget(_info);
	_layout->addWidget(_table);
	setLayout(_layout);
	/*Серийный номер*/
	relayDesc_t currentRelay  = req_server->getRelayById(id);
	QString type = req_server->getTypeById(currentRelay.type).type;
	QString serial_number = currentRelay.serial_number;
	_info->setText(tr("История реле ") + type + tr(", серийный номер ") + serial_number);
	_info->setAlignment(Qt::AlignHCenter);
	
	_table->setRowCount(history.size());
        _table->setColumnCount(8);
	
	QStringList _horizontal_header;
	_horizontal_header	<< tr("Дата")
					<< tr("Станция")
					<< tr("Номер\nстатива")
					<< tr("Номер\nприбора")
					<< tr("Последняя\nпроверка")
					<< tr("Статус")
                                        << tr("Комментарий")
                                        << tr("Владелец");
	_table->setHorizontalHeaderLabels(_horizontal_header);
	
	for (int row = 0; row < history.size(); row++){
		/*Дата*/
		QDateTime _dt = QDateTime::fromTime_t(history[row].date);
		_table->setItem(row, 0, new QTableWidgetItem(_dt.toString("dd.MM.yy hh:mm:ss")));
		/*Станция*/
		_table->setItem(row, 1, new QTableWidgetItem(req_server->getStationById(history[row].station).station));
		/*Статив*/
		_table->setItem(row, 2, new QTableWidgetItem(QVariant(history[row].relay_rack).toString()));
		/*Номер прибора*/
		_table->setItem(row, 3, new QTableWidgetItem(QVariant(history[row].device_number).toString()));
		/*Последняя проверка*/
		_dt = QDateTime::fromTime_t(history[row].last_check);
		_table->setItem(row, 4, new QTableWidgetItem(_dt.toString("dd.MM.yy")));
		/*Статус*/
		_table->setItem(row, 5, new QTableWidgetItem(req_server->getStatusById(history[row].status).status));
		/*Комментарий*/
		_table->setItem(row, 6, new QTableWidgetItem(history[row].comment));
                /*Владелец*/
                _table->setItem(row, 7, new QTableWidgetItem(req_server->getUserById(history[row].owner)));
    }
	_table->resizeColumnsToContents();
}

rs_history::~rs_history(void)
{
	for (int row = 0; row < _table->rowCount(); row++){
		for (int col = 0; col < _table->columnCount(); col++){
			delete(_table->item(row, col));
        }
    }
	delete(_table);
}
