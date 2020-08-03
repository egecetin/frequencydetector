#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	QColor dark = QColor(41, 43, 44);
	QColor silver = QColor(192, 192, 192);

	// Init
	mainWidget = new QWidget;

	timePlot = new QCustomPlot;
	freqPlot = new QCustomPlot;
	detectPlot = new QCustomPlot;
	fftPlot = new QCustomPlot;
	
	line = new QFrame;
	menu = new QFrame;
	subMenu = new QFrame;

	mainHLayout = new QHBoxLayout;
	mainPlotLayout = new QVBoxLayout;
	mainMenuLayout = new QVBoxLayout;

	// Prepare left side plots
	timePlot->setParent(mainWidget);
	timePlot->setBackground(QBrush(dark, Qt::SolidPattern));
	timePlot->xAxis->setBasePen(QPen(silver));
	timePlot->xAxis->setTickPen(QPen(silver));
	timePlot->xAxis->setSubTickPen(QPen(silver));
	timePlot->xAxis->setTickLabelColor(Qt::white);
	timePlot->yAxis->setBasePen(QPen(silver));
	timePlot->yAxis->setTickPen(QPen(silver));
	timePlot->yAxis->setSubTickPen(QPen(silver));
	timePlot->yAxis->setTickLabelColor(Qt::white);

	freqPlot->setParent(mainWidget);
	freqPlot->setBackground(QBrush(dark, Qt::SolidPattern));
	freqPlot->xAxis->setBasePen(QPen(silver));
	freqPlot->xAxis->setTickPen(QPen(silver));
	freqPlot->xAxis->setSubTickPen(QPen(silver));
	freqPlot->xAxis->setTickLabelColor(Qt::white);
	freqPlot->yAxis->setBasePen(QPen(silver));
	freqPlot->yAxis->setTickPen(QPen(silver));
	freqPlot->yAxis->setSubTickPen(QPen(silver));
	freqPlot->yAxis->setTickLabelColor(Qt::white);

	detectPlot->setParent(mainWidget);
	detectPlot->setBackground(QBrush(dark, Qt::SolidPattern));
	detectPlot->xAxis->setBasePen(QPen(silver));
	detectPlot->xAxis->setTickPen(QPen(silver));
	detectPlot->xAxis->setSubTickPen(QPen(silver));
	detectPlot->xAxis->setTickLabelColor(Qt::white);
	detectPlot->yAxis->setBasePen(QPen(silver));
	detectPlot->yAxis->setTickPen(QPen(silver));
	detectPlot->yAxis->setSubTickPen(QPen(silver));
	detectPlot->yAxis->setTickLabelColor(Qt::white);

	mainPlotLayout->addWidget(timePlot);
	mainPlotLayout->addWidget(freqPlot);
	mainPlotLayout->addWidget(detectPlot);
	mainPlotLayout->setSpacing(10);

	// Prepare right side
	fftPlot->setParent(menu);
	fftPlot->setBackground(QBrush(dark, Qt::SolidPattern));
	fftPlot->xAxis->setBasePen(QPen(silver));
	fftPlot->xAxis->setTickPen(QPen(silver));
	fftPlot->xAxis->setSubTickPen(QPen(silver));
	fftPlot->xAxis->setTickLabelColor(Qt::white);
	fftPlot->yAxis->setBasePen(QPen(silver));
	fftPlot->yAxis->setTickPen(QPen(silver));
	fftPlot->yAxis->setSubTickPen(QPen(silver));
	fftPlot->yAxis->setTickLabelColor(Qt::white);

	line->setLineWidth(2);
	line->setFrameShadow(QFrame::Sunken);
	line->setFrameShape(QFrame::HLine);

	subMenu->setParent(menu);
	subMenu->setFrameShape(QFrame::NoFrame);
	
	mainMenuLayout->addWidget(fftPlot, 4);
	mainMenuLayout->addWidget(line);
	mainMenuLayout->addWidget(subMenu, 6);

	menu->setParent(mainWidget);
	menu->setLineWidth(2);
	menu->setFrameShadow(QFrame::Sunken);
	menu->setFrameShape(QFrame::Box);
	menu->setLayout(mainMenuLayout);
	
	// Set mainwindow
	mainHLayout->addLayout(mainPlotLayout, 7);
	mainHLayout->addWidget(menu, 3);
	
	mainWidget->setParent(parent);
	mainWidget->setLayout(mainHLayout);

	this->setStyleSheet("background-color: rgb(41,43,44);");	// Dark
	this->setCentralWidget(mainWidget);
	this->resize(QGuiApplication::primaryScreen()->geometry().width() / 1.3, QGuiApplication::primaryScreen()->geometry().height() / 1.3);
	this->retranslateUi();
}

void MainWindow::retranslateUi()
{
	this->setWindowTitle(tr("Frequency detector"));
}

MainWindow::~MainWindow()
{
	delete timePlot;
	delete freqPlot;
	delete detectPlot;
	delete fftPlot;
	delete mainPlotLayout;
	
	delete subMenu;

	delete mainMenuLayout;
	delete menu;	
		
	delete mainHLayout;
	delete mainWidget;
}

