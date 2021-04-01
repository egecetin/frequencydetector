#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>

#include "audioreader.h"
#include "process.h"
#include "QCustomPlot/qcustomplot.h"

#include <QMainWindow>

#include <qprogressdialog.h>
#include <qbuttongroup.h>
#include <qlayoutitem.h>
#include <qaudiooutput.h>
#include <qboxlayout.h>
#include <qspinbox.h>
#include <qwidget.h>
#include <qglobal.h>
#include <qstyle.h>
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

static QColor plotColors[] = {	navy, darkorange, goldenrot, lime, wheat, aquamarine, 
								darkviolet, peachpuff, deepskyblue, mistyrose, rosybrown	};
static int plotColorsLen = sizeof(plotColors) / sizeof(QColor);

// Mainwindow
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	/********* Variables *********/
#ifdef _WIN32
	char filePath[FILENAME_MAX] = { '\0' };
#elif __linux__
	char filePath[PATH_MAX] = { '\0' };
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
	int outputLength = 0;
	int streamIdx = 0;
	int channelIdx = 0;
	int winIdx = 15;
	int windowLength = 4096;
	int overlap = 2048;
	int nwin = 8;
	int bits = 16;
	double thresh_alpha = 3.5;
	double thresh_inf = 0.5;
	QCPRange timeXAxes = QCPRange(0, 5);
	QCPRange timeYAxes = QCPRange(0, 5);
	QCPRange freqXAxes = QCPRange(0, 5);
	QCPRange freqYAxes = QCPRange(0, 5);

	/********** Objects **********/
	QWidget *mainWidget;
	QProgressDialog *dialog = nullptr;

	QBuffer *audioBuff = nullptr;
	QAudioOutput *audioDev = nullptr;

	QCustomPlot *timePlot;
	QCustomPlot *freqPlot;
	QCustomPlot *detectPlot;
	QCustomPlot *fftPlot;
	QCPColorMap *freqMap;

	QCPItemStraightLine *infLine;

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
	QPushButton *playButton;
	QPushButton *stopButton;
	QPushButton *forwardButton;
	QPushButton *backwardButton;

	/********** Layouts **********/
	QSpacerItem *spacer1;
	QSpacerItem *spacer2;
	QSpacerItem *spacer3;
	QSpacerItem *spacer4;
	QSpacerItem *spacer5;

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
	QHBoxLayout *playerButtonsBoxLayout;

	/********* Functions *********/	
	void selectFile(bool flag = true);
	void updateValues();

	void playMedia();
	void stopMedia();
	void backwardMedia();
	void forwardMedia();

	void playerSlider();
	void playerStateChanged(QAudio::State state);

	void radioChange(int id);
	void wLenChanged(const QString &val);
	void overlapChanged(int val);
	void cutLowFreqChange(double val);
	void cutHighFreqChange(double val);

	void limitRange(QCustomPlot *plot, QCPRange *X, QCPRange *Y);

	Q_INVOKABLE void enableButtons();
	Q_INVOKABLE void disableButtons();
	Q_INVOKABLE void updatePlots(bool flag = false);
	Q_INVOKABLE void updateFFTPlot(int64_t pos);

};
#endif // MAINWINDOW_H
