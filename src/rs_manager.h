#ifndef RS_WIDGET_MANAGER_H
#define RS_WIDGET_MANAGER_H

#include <QList>
#include <QWidget>
#include <QString>
#include <QPair>
#include <QDebug>

enum LogicManagerError_t{
    LM_UNKNOWN,
    LM_SUCCESS,
    LM_ALREADY_IN_USE
};

enum LogicManagerState_t{
    LOGIN,
    MAIN_TABLE_HAVE_CHANGE
};

class logic_manager
{
private:

    static logic_manager *_instance;

    struct NamedObject_t
    {
        QString name;
        QObject *object;
    };

    QList <NamedObject_t> _objects;
    QList<LogicManagerState_t> _state;

protected:
    logic_manager();

public:
    static logic_manager *instance();

    LogicManagerError_t registerObject(QString name, QObject *object);
    QObject* getObject(QString name) const;

    void setState(LogicManagerState_t state);
    void unsetState(LogicManagerState_t state);
    bool state(LogicManagerState_t state) const;
};

#endif // RS_WIDGET_MANAGER_H
