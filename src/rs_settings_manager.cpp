#include "rs_settings_manager.h"

rs_settings_manager *rs_settings_manager::_instance = 0;

rs_settings_manager *rs_settings_manager::getInstance()
{
    if(_instance == 0) {

        _instance = new rs_settings_manager();
    }

    return _instance;
}

const QString &rs_settings_manager::dbName()
{
    return db_name;
}

rs_settings_manager::rs_settings_manager()
{   
    QSettings settings("settings.ini", QSettings::IniFormat);

    db_name = settings.value("database/name").toString();
}


