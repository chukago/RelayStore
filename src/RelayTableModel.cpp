#include "RelayTableModel.h"

enum ColumnsDef_t {

    ID_COLUMN = 0,
    TYPE_COLUMN = 1,
    CHECK_PERIOD_COLUMN	= 2,
    SERIAL_NUMBER_COLUMN = 3,
    MANUFACTURED_COLUMN = 4,
    STATION_COLUMN = 5,
    RELAY_RACK_COLUMN = 6,
    DEVICE_NUMBER_COLUMN = 7,
    LAST_CHECK_COLUMN = 8,
    NEXT_CHECK_COLUMN = 9,
    STATUS_COLUMN = 10,
    COMMENT_COLUMN = 11
};

RelayTableModel::RelayTableModel():
    QAbstractTableModel()
{

}

int RelayTableModel::columnCount(const QModelIndex &/*parent*/)
{
    return 0;
}


