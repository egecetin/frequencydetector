#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	// Init
	mainWidget = new QWidget;

	timePlot = new QCustomPlot;
	freqPlot = new QCustomPlot;
	detectPlot = new QCustomPlot;
	fftPlot = new QCustomPlot;
	
	line = new QFrame;
	menu = new QFrame;

	mainHLayout = new QHBoxLayout;
	mainPlotLayout = new QVBoxLayout;
	mainMenuLayout = new QVBoxLayout;

	// Prepare left side plots
	timePlot->setParent(mainWidget);
	freqPlot->setParent(mainWidget);
	detectPlot->setParent(mainWidget);

	mainPlotLayout->addWidget(timePlot);
	mainPlotLayout->addWidget(freqPlot);
	mainPlotLayout->addWidget(detectPlot);
	mainPlotLayout->setSpacing(10);

	// Prepare right side
	fftPlot->setParent(mainWidget);

	menu->setParent(mainWidget);
	menu->setFrameShadow(QFrame::Sunken);
	menu->setFrameShape(QFrame::StyledPanel);
	
	mainMenuLayout->addWidget(fftPlot);
	mainMenuLayout->addWidget(menu);
	
	// Set mainwindow
	line->setParent(mainWidget);
	line->setFrameShadow(QFrame::Sunken);
	line->setFrameShape(QFrame::VLine);

	mainHLayout->addLayout(mainPlotLayout, 7);
	mainHLayout->addWidget(line);
	mainHLayout->addLayout(mainMenuLayout, 3);
	
	mainWidget->setParent(parent);
	mainWidget->setLayout(mainHLayout);
	
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
	
	delete line;
	delete menu;
	
	delete mainPlotLayout;
	delete mainMenuLayout;
	delete mainHLayout;
}

