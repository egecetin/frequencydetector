#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "audioreader.h"
#include "QCustomPlot/qcustomplot.h"

#include <QMainWindow>

#include <qboxlayout.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qframe.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
	void retranslateUi();
    ~MainWindow();

private:
	/********** Objects **********/
	QWidget *mainWidget;
	QWidget *timeplotWidget;
	QWidget *freqplotWidget;
	QWidget *detectplotWidget;
	QWidget *fftplotWidget;

	QCustomPlot *timePlot;
	QCustomPlot *freqPlot;
	QCustomPlot *detectPlot;
	QCustomPlot *fftPlot;

	QFrame *line;
	QFrame *menu;
	QFrame *subMenu;

	/********** Layouts **********/
	QHBoxLayout *mainHLayout;
	QVBoxLayout *mainPlotLayout;
	QVBoxLayout *mainMenuLayout;
};
#endif // MAINWINDOW_H
