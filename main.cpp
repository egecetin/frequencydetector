#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	qRegisterMetaType<QCPRange>("QCPRange");

    QApplication a(argc, argv);

	// Setup GUI
	a.setWindowIcon(QIcon("app.ico"));

	// Set stylesheet
	QFile f("qdarkstyle/style.qss");
	if (!f.exists()) {
		printf("Unable to set stylesheet, file not found\n");
	}
	else {
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		qApp->setStyleSheet(ts.readAll());
	}
	
	MainWindow w;
    w.show();

    return a.exec();
}
