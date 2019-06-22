#include "rs_manager.h"

/* logic_manager */

logic_manager *logic_manager::_instance = nullptr;

logic_manager::logic_manager()
{
}

logic_manager *logic_manager::instance()
{
    if(_instance == nullptr)
    {
        _instance = new logic_manager;
    }

    return(_instance);
}

LogicManagerError_t logic_manager::registerObject(QString name, QObject *object)
{
    for (int i = 0; i < _objects.size(); i++)
    {
        if (_objects.at(i).name == name)
        {
            return LM_UNKNOWN;
        }
    }

    NamedObject_t no = {name, object};

    _objects.append(no);

    return LM_SUCCESS;
}

QObject* logic_manager::getObject(QString name) const
{
    for (int i = 0; i < _objects.size(); i++)
    {
        if (_objects.at(i).name == name)
        {
            return  _objects.at(i).object;
        }
    }

    return nullptr;
}

void logic_manager::setState(LogicManagerState_t state)
{
    for(QList<LogicManagerState_t>::const_iterator it = _state.begin(); it != _state.end(); it++)
    {
        if((*it) == state)
        {
            return;
        }
    }

    _state.push_back(state);
}

void logic_manager::unsetState(LogicManagerState_t state)
{
    for(QList<LogicManagerState_t>::iterator it = _state.begin(); it != _state.end(); it++)
    {
        if((*it) == state)
        {
            _state.erase(it);
            return;
        }
    }
}

bool logic_manager::state(LogicManagerState_t state) const
{
    for(QList<LogicManagerState_t>::const_iterator it = _state.begin(); it != _state.end(); it++)
    {
        if((*it) == state)
        {
            return(true);
        }
    }

    return(false);
}
