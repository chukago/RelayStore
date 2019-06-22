#include "rs_main.h"

#include <QFontDatabase>
#include <QAction>

//QAction *authAction;
//QAction *saveAction;
//QAction *queryAction;
//QAction *addAction;
//QAction *historyAction;
//QAction *printAction;
//QAction *settingsAction;
//QAction *quitAction;
//QAction *aboutAction;

void rs_main::createToolBar()
{
    toolBar = new QToolBar(this);
    toolBar->setIconSize(QSize(72,72));

    authAction = toolBar->addAction(QIcon(":icon/gfx/connect.png"), "Вход", this, SLOT(authentification()));
    saveAction = toolBar->addAction(QIcon(":icon/gfx/save.png"), "Сохранить", this, SLOT(save()));
    queryAction = toolBar->addAction(QIcon(":icon/gfx/query.png"), "Поиск", this, SLOT(query()));
    addAction = toolBar->addAction(QIcon(":icon/gfx/add.png"), "Добавить", this, SLOT(add()));
    historyAction = toolBar->addAction(QIcon(":icon/gfx/history.png"), "История", this, SLOT(history()));
    printAction = toolBar->addAction(QIcon(":icon/gfx/print.png"), "Печать", this, SLOT(print()));
    settingsAction = toolBar->addAction(QIcon(":icon/gfx/settings.png"), "Настройки", this, SLOT(settings()));
    //quitAction = toolBar->addAction(QIcon(":icon/gfx/quit.png"), "Выход", this, SLOT(exit()));
    //aboutAction = toolBar->addAction(QIcon(":icon/gfx/about.png"), "О программе", this, SLOT(about()));

    saveAction->setEnabled(false);
    queryAction->setEnabled(false);
    addAction->setEnabled(false);
    historyAction->setEnabled(false);
    printAction->setEnabled(false);
    settingsAction->setEnabled(false);

    manager->registerObject("save_action", saveAction);
}

rs_main::rs_main(QWidget *parent)
        :QWidget(parent)
{
    QFontDatabase::addApplicationFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf");

    manager = logic_manager::instance();

	setWindowState(Qt::WindowMaximized);
    window()->setWindowTitle(tr("Склад реле v.0.4"));
    setWindowIcon(QIcon(":icon/gfx/app.png"));
	
    _req_server = new dbQueryServer();

    if (!_req_server->isConnected()){
		QMessageBox  *fail_message = new QMessageBox(this);
		fail_message->window()->setWindowTitle(tr("Внимание!"));
		fail_message->setText(tr("Невозможно подключиться к базе данных!"));
		fail_message->setAttribute(Qt::WA_DeleteOnClose);
		fail_message->setModal(true);
		fail_message->exec();
		setEnabled(false);
    }

    createToolBar();

	table = new rs_main_table(this, _req_server);
	
	main_layout = new QGridLayout(this);
	
    main_layout->addWidget(toolBar, 0, 0);
	main_layout->addWidget(table, 1, 0);

    setLayout(main_layout);
}

bool rs_main::checkAuthorizationData(QString login, QString passwd)
{
	if ((login == "") || (passwd == "")){
		return (false);
    }
	
	currentUser = _req_server->getUserInfo(login, _req_server->encrypt(passwd));
	
	if (currentUser.id)
	{
        authAction->setEnabled(false);
        _req_server->setCurrentUser(currentUser.id);
		return(true);
    }
	
	return(false);
	
}

void rs_main::setupRights(userDesc_t currentUser)
{
    //God Mode
    /*
    queryAction->setEnabled(true);
    historyAction->setEnabled(true);
    printAction->setEnabled(true);
    addAction->setEnabled(true);
    settingsAction->setEnabled(true);
    printAction->setEnabled(true);
    return;
    */

	if (currentUser.rights & RS_READ){

        queryAction->setEnabled(true);
        historyAction->setEnabled(true);
        printAction->setEnabled(true);
    }
	
	if (currentUser.rights & RS_WRITE){
	}
	else{
		table->setReadOnly();
    }
	
	if (currentUser.rights & RS_ADD){

        addAction->setEnabled(true);
    }
	
	if (currentUser.rights & RS_SETT){

        settingsAction->setEnabled(true);
    }
	
	if (currentUser.rights & RS_PRINT){

        printAction->setEnabled(true);
    }
}

void rs_main::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_F1:
        if (!manager->state(LOGIN)) authentification();
        break;
    case Qt::Key_F2:
        if (currentUser.rights & RS_WRITE) save();
        break;
    case Qt::Key_F3:
        if (currentUser.rights & RS_READ) query();
        break;
    case Qt::Key_F4:
        if (currentUser.rights & RS_ADD) add();
        break;
    case Qt::Key_F5:
        if (currentUser.rights & RS_READ) history();
        break;
    case Qt::Key_F6:
        if (currentUser.rights & RS_PRINT) print();
        break;
    case Qt::Key_F7:
        if (currentUser.rights & RS_SETT) settings();
        break;
    case Qt::Key_F8:
        exit();
        break;
    case Qt::Key_F9:
        about();
        break;
    }
}

/* Вызывается при нажатии кнопки "Авторизация" */
void rs_main::authentification(void)
{
	rs_auth_dialog *auth_dialog = new rs_auth_dialog(this);
	auth_dialog->setModal(true);

	if (auth_dialog->exec() == QDialog::Accepted){
		if (!checkAuthorizationData(auth_dialog->getLogin(), auth_dialog->getPassword())){
			QMessageBox  *fail_message = new QMessageBox(this);
			fail_message->window()->setWindowTitle(tr("Внимание!"));
			fail_message->setText(tr("Логин или пароль неверен!"));
			fail_message->setAttribute(Qt::WA_DeleteOnClose);
			fail_message->setModal(true);
			fail_message->exec();
		}
		else{
                    manager->setState(LOGIN);
                    setupRights(currentUser);
        }
    }

	delete(auth_dialog);
}

void rs_main::save(void)
{
    if(manager->state(MAIN_TABLE_HAVE_CHANGE))
    {
        QMessageBox  *save_message = new QMessageBox(this);
        save_message->addButton(QMessageBox::Yes);
        save_message->addButton(QMessageBox::No);
        save_message->window()->setWindowTitle(tr("Сохранение!"));
        save_message->setText(tr("Вы действительно хотите сохранить изменения?"));
        save_message->setAttribute(Qt::WA_DeleteOnClose);

        if (save_message->exec() == QMessageBox::Yes){

            _req_server->writeToDb();
#warning "Временно! Очень дорогостоящая операция!"
            table->new_request();
            manager->unsetState(MAIN_TABLE_HAVE_CHANGE);
            saveAction->setEnabled(false);
        }
    }
}

void rs_main::query(void)
{
	rs_query_dialog *query_dialog = new rs_query_dialog(this, _req_server);

    query_dialog->setModal(true);

    if (query_dialog->exec() == QDialog::Accepted){

        table->new_request();
    }

    delete query_dialog;
}

void rs_main::add(void)
{
	rs_add_dialog *add_dialog = new rs_add_dialog(this, _req_server);
	add_dialog->window()->setWindowTitle(tr("Добавить реле"));
	add_dialog->setAttribute(Qt::WA_DeleteOnClose);
	add_dialog->setModal(true);
	add_dialog->show();
}

void rs_main::history(void)
{
    if (table->rowCount() &&  table->item(table->currentRow(), 0) != nullptr){
		rs_history *histWnd = new rs_history(this, _req_server, table->item(table->currentRow(), 0)->text().toUInt());
		histWnd->exec();
		delete(histWnd);
    }
}

void rs_main::print(void)
{
	QString printFileName = QFileDialog::getSaveFileName(this, tr("Выбор файла"), "/home", tr("Файлы *html.xls (*.html.xls)"));
	if (printFileName.indexOf(QRegExp("xls$")) == -1){
		printFileName.append(".html.xls");
    }
	rs_html *htmlPrint = new rs_html(printFileName, "Arial");
	if (table->rowCount()){
		QVector<QVector<QString> > tempTable;
		QVector<int> colSizes;
		QVector<QString> tempRow;
		for (int col = 0; col < table->columnCount(); col++)
		{
			colSizes.append(100);
        }
		
		for (int col = 0; col < table->columnCount(); col++){
			tempRow.append(table->horizontalHeaderItem(col)->text());
        }
		tempTable.append(tempRow);
		tempRow.clear();
		
		for (int row = 0; row < table->rowCount(); row++){
			for (int col = 0; col < table->columnCount(); col++){
				tempRow.append(table->item(row, col)->text());
            }
			tempTable.append(tempRow);
			tempRow.clear();
        }
		htmlPrint->begin();
		htmlPrint->write_table(&tempTable, colSizes);
		htmlPrint->end();
    }
    /*
    QPrinter *printer = new QPrinter();
    QPrintDialog pDialog(printer, this);
    QVector<QVector<QString> > tempTable;
    QVector<QString> tempRow;

    for (int row = 0; row < table->rowCount(); row++){
	for (int col = 0; col < table->columnCount(); col++){
	    tempRow.append(table->item(row, col)->text());
    }
	tempTable.append(tempRow);
	tempRow.clear();
    }

    pDialog.setModal(true);
    if (pDialog.exec()){
	rs_pdf *pdfPrinting = new rs_pdf(printer);
	pdfPrinting->print(tempTable);
    }
    */
}

void rs_main::settings(void)
{
	rs_settings *_settings = new rs_settings(this, _req_server);
	_settings->exec();
}

void rs_main::exit(void)
{
	quit();
}

void rs_main::about(void)
{
    rs_about *about = new rs_about(this);

    about->exec();
    delete(about);
}
