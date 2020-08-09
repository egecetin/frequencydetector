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
	subMenu = new QFrame;

	wlenLabel = new QLabel;
	nwinLabel = new QLabel;
	infLabel = new QLabel;
	alphaLabel = new QLabel;

	wlenBox = new QComboBox;
	nwinBox = new QSpinBox;
	infBox = new QDoubleSpinBox;
	alphaBox = new QDoubleSpinBox;

	radioGroup = new QButtonGroup;
	radio_low  = new QRadioButton;
	radio_high = new QRadioButton;
	radio_pass = new QRadioButton;
	radio_stop = new QRadioButton;

	mainHLayout = new QHBoxLayout;
	mainPlotLayout = new QVBoxLayout;
	mainMenuLayout = new QVBoxLayout;
	subMenuLayout = new QVBoxLayout;

	wlenBoxLayout = new QHBoxLayout;
	nwinBoxLayout = new QHBoxLayout;
	infBoxLayout = new QHBoxLayout;
	alphaBoxLayout = new QHBoxLayout;

	// Prepare left side plots
	timePlot->setParent(mainWidget);
	timePlot->setBackground(QBrush(dark, Qt::SolidPattern));
	timePlot->xAxis->setBasePen(QPen(silver));
	timePlot->xAxis->setTickPen(QPen(silver));
	timePlot->xAxis->setSubTickPen(QPen(silver));
	timePlot->xAxis->setTickLabelColor(Qt::white);
	timePlot->xAxis->grid()->setVisible(false);
	timePlot->yAxis->setBasePen(QPen(silver));
	timePlot->yAxis->setTickPen(QPen(silver));
	timePlot->yAxis->setSubTickPen(QPen(silver));
	timePlot->yAxis->setTickLabelColor(Qt::white);
	timePlot->yAxis->grid()->setVisible(false);

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
	detectPlot->xAxis->setLabelColor(Qt::white);
	detectPlot->xAxis->setBasePen(QPen(silver));
	detectPlot->xAxis->setTickPen(QPen(silver));
	detectPlot->xAxis->setSubTickPen(QPen(silver));
	detectPlot->xAxis->setTickLabelColor(Qt::white);
	detectPlot->xAxis->grid()->setVisible(false);
	detectPlot->yAxis->setBasePen(QPen(silver));
	detectPlot->yAxis->setTickPen(QPen(silver));
	detectPlot->yAxis->setSubTickPen(QPen(silver));
	detectPlot->yAxis->setTickLabelColor(Qt::white);
	detectPlot->yAxis->grid()->setVisible(false);

	mainPlotLayout->addWidget(timePlot);
	mainPlotLayout->addWidget(freqPlot);
	mainPlotLayout->addWidget(detectPlot);
	mainPlotLayout->setSpacing(10);

	// Prepare right side
	fftPlot->setParent(menu);
	fftPlot->setBackground(QBrush(dark, Qt::SolidPattern));
	fftPlot->xAxis->setLabelColor(Qt::white);
	fftPlot->xAxis->setBasePen(QPen(silver));
	fftPlot->xAxis->setTickPen(QPen(silver));
	fftPlot->xAxis->setSubTickPen(QPen(silver));
	fftPlot->xAxis->setTickLabelColor(Qt::white);
	fftPlot->xAxis->grid()->setSubGridVisible(true);
	fftPlot->xAxis->grid()->setPen(QPen(silver, 1, Qt::DotLine));
	fftPlot->xAxis->grid()->setSubGridPen(QPen(dimgray, 1, Qt::DotLine));
	fftPlot->yAxis->setBasePen(QPen(silver));
	fftPlot->yAxis->setTickPen(QPen(silver));
	fftPlot->yAxis->setSubTickPen(QPen(silver));
	fftPlot->yAxis->setTickLabelColor(Qt::white);
	fftPlot->yAxis->grid()->setSubGridVisible(true);
	fftPlot->yAxis->grid()->setPen(QPen(silver, 1, Qt::DotLine));
	fftPlot->yAxis->grid()->setSubGridPen(QPen(dimgray, 1, Qt::DotLine));	

	line->setLineWidth(2);
	line->setFrameShadow(QFrame::Sunken);
	line->setFrameShape(QFrame::HLine);
	
	// wlenBox <-------------------
	wlenLabel->setParent(menu);
	wlenLabel->setStyleSheet("color: rgb(192, 192, 192)");
	wlenBox->setParent(menu);
	wlenBox->setStyleSheet("color: rgb(192, 192, 192)");
	for (int i = 0; i < log2(MAX_WLEN) - log2(MIN_WLEN) + 1; ++i) {
		wlenBox->addItem(QString::number(MAX_WLEN / pow(2, i / 5)));
	}
	wlenBoxLayout->addWidget(wlenLabel, 6);
	wlenBoxLayout->addWidget(wlenBox, 4);
	
	nwinLabel->setParent(menu);
	nwinLabel->setStyleSheet("color: rgb(192, 192, 192)");
	nwinBox->setParent(menu);
	nwinBox->setSingleStep(1);
	nwinBox->setMinimum(1);
	nwinBox->setMaximum(100);
	nwinBox->setAlignment(Qt::AlignRight);
	nwinBox->setStyleSheet("color: rgb(192, 192, 192)");
	nwinBoxLayout->addWidget(nwinLabel, 6);
	nwinBoxLayout->addWidget(nwinBox, 4);

	infLabel->setParent(menu);
	infLabel->setStyleSheet("color: rgb(192, 192, 192)");
	infBox->setParent(menu);
	infBox->setDecimals(4);
	infBox->setSingleStep(0.0001);
	infBox->setMinimum(0.0001);
	infBox->setMaximum(100);
	infBox->setAlignment(Qt::AlignRight);
	infBox->setStyleSheet("color: rgb(192, 192, 192)");
	infBoxLayout->addWidget(infLabel, 6);
	infBoxLayout->addWidget(infBox, 4);

	alphaLabel->setParent(menu);
	alphaLabel->setStyleSheet("color: rgb(192, 192, 192)");
	alphaBox->setParent(menu);	
	alphaBox->setDecimals(4);
	alphaBox->setSingleStep(0.0001);
	alphaBox->setMinimum(0.0001);
	alphaBox->setMaximum(100);
	alphaBox->setAlignment(Qt::AlignRight);
	alphaBox->setStyleSheet("color: rgb(192, 192, 192)");
	alphaBoxLayout->addWidget(alphaLabel, 6);
	alphaBoxLayout->addWidget(alphaBox, 4);

	subMenuLayout->addLayout(wlenBoxLayout);
	subMenuLayout->addLayout(nwinBoxLayout);
	subMenuLayout->addLayout(infBoxLayout);
	subMenuLayout->addLayout(alphaBoxLayout);
	subMenu->setParent(mainWidget);
	subMenu->setFrameShape(QFrame::NoFrame);
	subMenu->setLayout(subMenuLayout);

	mainMenuLayout->addWidget(fftPlot, 4);
	mainMenuLayout->addWidget(line);
	mainMenuLayout->addWidget(subMenu, 6);

	menu->setParent(mainWidget);
	menu->setLineWidth(2);
	menu->setFrameShadow(QFrame::Sunken);
	menu->setFrameShape(QFrame::Box);
	menu->setLayout(mainMenuLayout);
	
	// Set mainwindow
	mainHLayout->setParent(mainWidget);
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
	fftPlot->xAxis->setLabel(tr("Frequency"));
	detectPlot->xAxis->setLabel(tr("Time"));
	wlenLabel->setText(tr("Window length :"));
	nwinLabel->setText(tr("Window number :"));
	infLabel->setText(tr("Influence :"));
	alphaLabel->setText(tr("Alpha :"));

	this->setWindowTitle(tr("Frequency detector"));
}

MainWindow::~MainWindow()
{
	delete mainWidget;
}

