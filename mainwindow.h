#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "audioreader.h"
#include "process.h"
#include "QCustomPlot/qcustomplot.h"

#include <QMainWindow>

#include <qbuttongroup.h>
#include <qlayoutitem.h>
#include <qboxlayout.h>
#include <qspinbox.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qframe.h>

// UI Colors
#define dark QColor(41, 43, 44)
#define silver QColor(192, 192, 192)
#define dimgray QColor(105, 105, 105)

// Plot Colors
#define navy QColor(0, 0, 128)
#define crimson QColor(220, 20, 60)
#define darkorange QColor(255, 140, 0)
#define goldenrot QColor(218, 165, 32)
#define lime QColor(0, 255, 0)
#define wheat QColor(245, 222, 179)
#define aquamarine QColor(127, 255, 212)
#define darkviolet QColor(148, 0, 211)
#define peachpuff QColor(255, 218, 185)
#define deepskyblue QColor(0, 191, 255)
#define mistyrose QColor(255, 228, 225)
#define rosybrown QColor(188, 143, 143)

static QColor plotColors[] = {	navy, crimson, darkorange, goldenrot, lime, wheat, aquamarine, 
								darkviolet, peachpuff, deepskyblue, mistyrose, rosybrown	};
static int plotColorsLen = sizeof(plotColors) / sizeof(QColor);

// Mainwindow
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

	QCustomPlot *timePlot;
	QCustomPlot *freqPlot;
	QCustomPlot *detectPlot;
	QCustomPlot *fftPlot;

	QFrame *line;
	QFrame *menu;
	QFrame *subMenu;

	QLabel *wlenLabel;
	QLabel *nwinLabel;
	QLabel *infLabel;
	QLabel *alphaLabel;
	QLabel *channelLabel;
	QLabel *path;

	QComboBox *wlenBox;
	QComboBox *channelBox;
	QSpinBox *nwinBox;
	QDoubleSpinBox *infBox;
	QDoubleSpinBox *alphaBox;	

	QButtonGroup *radioGroup;
	QRadioButton *radio_low;
	QRadioButton *radio_high;
	QRadioButton *radio_pass;
	QRadioButton *radio_stop;
	QPushButton *selectButton;

	/********** Layouts **********/
	QSpacerItem *spacer1;
	QSpacerItem *spacer2;

	QHBoxLayout *mainHLayout;
	QVBoxLayout *mainPlotLayout;
	QVBoxLayout *mainMenuLayout;
	QVBoxLayout *subMenuLayout;

	QHBoxLayout *selectButtonLayout;
	QHBoxLayout *channelBoxLayout;
	QHBoxLayout *wlenBoxLayout;
	QHBoxLayout *nwinBoxLayout;
	QHBoxLayout *infBoxLayout;
	QHBoxLayout *alphaBoxLayout;
};
#endif // MAINWINDOW_H
