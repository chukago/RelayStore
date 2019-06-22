#include <QApplication>
#include <QTextCodec>
#include <QDir>
#include "src/rs_main.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QDir::setCurrent(app.applicationDirPath());

	rs_main *mainWidget = new rs_main();
	
	mainWidget->show();

	QObject::connect(mainWidget, SIGNAL(quit()), &app, SLOT(quit()));
	
	return app.exec();
}
