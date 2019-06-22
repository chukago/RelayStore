#ifndef RS_SETTINGS_MANAGER_H
#define RS_SETTINGS_MANAGER_H

#include <QSettings>

class rs_settings_manager
{
public:

    static rs_settings_manager *getInstance();
    const QString &dbName();

private:

    static rs_settings_manager *_instance;
    rs_settings_manager();

    QString db_name;
};

#endif // RS_SETTINGS_MANAGER_H
