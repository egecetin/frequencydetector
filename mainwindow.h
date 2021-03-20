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
	/********* Variables *********/
#ifdef _WIN32
	char filePath[FILENAME_MAX];
#elif __linux__
	char filePath[PATH_MAX];
#endif // WIN32

	/********* Functions *********/
    MainWindow(QWidget *parent = nullptr);
	void retranslateUi();
    ~MainWindow();

private:
	/********* Variables *********/
	AudioReader reader = AudioReader_Default;
	AudioData audio = AudioData_Default;
	Ipp64f** windowFunctions = NULL;
	double** spectrogramData = NULL;
	double** alarmsData = NULL;
	int* alarmLengths = NULL;
	int outputLength;
	int streamIdx = 0;
	int channelIdx = 0;
	int winIdx = 15;
	int windowLength = 4096;
	int overlap = 2048;

	/********** Objects **********/
	QWidget *mainWidget;

	QCustomPlot *timePlot;
	QCustomPlot *freqPlot;
	QCustomPlot *detectPlot;
	QCustomPlot *fftPlot;
	QCPColorMap *freqMap = nullptr;

	QFrame *line;
	QFrame *menu;
	QFrame *subMenu;
	QFrame *radioMenu;

	QLabel *wlenLabel;
	QLabel *overlapLabel;
	QLabel *winTypeLabel;
	QLabel *nwinLabel;
	QLabel *infLabel;
	QLabel *alphaLabel;
	QLabel *channelLabel;
	QLabel *path;
	QLabel *lowFreqLabel;
	QLabel *highFreqLabel;

	QComboBox *wlenBox;
	QComboBox *winTypeBox;
	QComboBox *channelBox;
	QSpinBox *nwinBox;
	QSpinBox *overlapBox;
	QDoubleSpinBox *infBox;
	QDoubleSpinBox *alphaBox;
	QDoubleSpinBox *lowFreqBox;
	QDoubleSpinBox *highFreqBox;

	QButtonGroup *radioGroup;
	QRadioButton *radio_low;
	QRadioButton *radio_high;
	QRadioButton *radio_pass;
	QRadioButton *radio_stop;
	QPushButton *selectButton;
	QPushButton *plotButton;

	/********** Layouts **********/
	QSpacerItem *spacer1;
	QSpacerItem *spacer2;
	QSpacerItem *spacer3;

	QHBoxLayout *mainHLayout;
	QVBoxLayout *mainPlotLayout;
	QVBoxLayout *mainMenuLayout;
	QVBoxLayout *subMenuLayout;
	QVBoxLayout *radioMenuLayout;
	QVBoxLayout *cutFreqLayout;

	QHBoxLayout *filterGroupLayout;
	QHBoxLayout *selectButtonLayout;
	QHBoxLayout *channelBoxLayout;
	QHBoxLayout *wlenBoxLayout;
	QHBoxLayout *overlapBoxLayout;
	QHBoxLayout *nwinBoxLayout;
	QHBoxLayout *winTypeBoxLayout;
	QHBoxLayout *infBoxLayout;
	QHBoxLayout *alphaBoxLayout;
	QHBoxLayout *lowFreqBoxLayout;
	QHBoxLayout *highFreqBoxLayout;

	/********* Functions *********/	
	void selectFile();
	void updateValues();

	Q_INVOKABLE void enableButtons();
	Q_INVOKABLE void disableButtons();
	Q_INVOKABLE void updatePlots(bool flag = false);
	Q_INVOKABLE void updateFFTPlot();

};
#endif // MAINWINDOW_H
