#ifndef RS_MAIN_H
#define RS_MAIN_H
/*
	Главный виджет
*/

#include <QWidget>
#include <QToolBar>
#include <QGridLayout>
#include <QTextCodec>
#include <QtSql/QSqlDatabase>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QKeyEvent>

#include "rs_manager.h"
#include "rs_main_table.h"
#include "rs_auth_dialog.h"
#include "rs_add_dialog.h"
#include "rs_query_dialog.h"
//#include "../src/servers.h"
#include "rs_about.h"
//#include "../src/cryptutils.h"
#include "rs_history.h"
#include "rs_settings.h"

#include "QDebug"

class rs_main : public QWidget
{
	Q_OBJECT
	private:
		dbQueryServer *_req_server;
		
		QTextCodec *global_codec;
		QGridLayout *main_layout;		// Слой главного окна
		rs_main_table *table;			// Таблица

        QToolBar *toolBar;
        QAction *authAction;
        QAction *saveAction;
        QAction *queryAction;
        QAction *addAction;
        QAction *historyAction;
        QAction *printAction;
        QAction *settingsAction;
        QAction *quitAction;
        QAction *aboutAction;
	
        void createToolBar();

        logic_manager *manager;
	
	protected:
		userDesc_t currentUser;
	
	public:
		rs_main(QWidget *parent = 0);
	
		/* Проверка соответствия логина и пароля аутентификационым данным в базе данных */
		bool checkAuthorizationData(QString login, QString passwd);
	
		/* Установка прав пользователя */
		void setupRights(userDesc_t currentUser);

                void keyPressEvent(QKeyEvent *event);
	
	public slots:
		
		void authentification(void);	// Авторизация пользователя
		void save(void);			// Сохранение
		void query(void);			// Запрос к базе данных
		void add(void);			// Добавление нового реле
		void history(void);			// Просмотр истории
		void print(void);			// Печать
		void settings(void);		// Настройки программы
		void exit(void);			// Выход
		void about(void);			// О программе
	
	signals:
		void quit();
};
#endif /*RS_MAIN_H*/
