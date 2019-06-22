#ifndef RS_RELAY_TABLE_MODEL_H
#define RS_RELAY_TABLE_MODEL_H

#include <QAbstractTableModel>

class RelayTableModel : public QAbstractTableModel
{
public:
    RelayTableModel();

    int columnCount(const QModelIndex &parent = QModelIndex());
};

#endif // RS_RELAY_TABLE_MODEL_H
