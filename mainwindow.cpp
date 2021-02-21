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
	radioMenu = new QFrame;

	wlenLabel = new QLabel;
	overlapLabel = new QLabel;
	nwinLabel = new QLabel;
	winTypeLabel = new QLabel;
	infLabel = new QLabel;
	alphaLabel = new QLabel;
	channelLabel = new QLabel;
	path = new QLabel;
	lowFreqLabel = new QLabel;
	highFreqLabel = new QLabel;

	wlenBox = new QComboBox;
	winTypeBox = new QComboBox;
	channelBox = new QComboBox;
	nwinBox = new QSpinBox;
	overlapBox = new QSpinBox;
	infBox = new QDoubleSpinBox;
	alphaBox = new QDoubleSpinBox;
	lowFreqBox = new QDoubleSpinBox;
	highFreqBox = new QDoubleSpinBox;

	radioGroup = new QButtonGroup;
	radio_low  = new QRadioButton;
	radio_high = new QRadioButton;
	radio_pass = new QRadioButton;
	radio_stop = new QRadioButton;
	selectButton = new QPushButton;
	plotButton = new QPushButton;

	mainHLayout = new QHBoxLayout;
	mainPlotLayout = new QVBoxLayout;
	mainMenuLayout = new QVBoxLayout;
	subMenuLayout = new QVBoxLayout;
	radioMenuLayout = new QVBoxLayout;
	cutFreqLayout = new QVBoxLayout;

	filterGroupLayout = new QHBoxLayout;
	selectButtonLayout = new QHBoxLayout;
	channelBoxLayout = new QHBoxLayout;
	wlenBoxLayout = new QHBoxLayout;
	overlapBoxLayout = new QHBoxLayout;
	nwinBoxLayout = new QHBoxLayout;
	winTypeBoxLayout = new QHBoxLayout;
	infBoxLayout = new QHBoxLayout;
	alphaBoxLayout = new QHBoxLayout;
	lowFreqBoxLayout = new QHBoxLayout;
	highFreqBoxLayout = new QHBoxLayout;

	spacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding);
	spacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding);
	spacer3 = new QSpacerItem(20, 5, QSizePolicy::Expanding);

	// Prepare left side plots
	timePlot->setParent(mainWidget);
	timePlot->setOpenGl(true);
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
	timePlot->addGraph();
	timePlot->graph(0)->setPen(QPen(navy));
	timePlot->graph(0)->setBrush(QBrush(navy));
	timePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	connect(timePlot->xAxis, SIGNAL(rangeChanged(QCPRange)), timePlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(timePlot->yAxis, SIGNAL(rangeChanged(QCPRange)), timePlot->yAxis2, SLOT(setRange(QCPRange)));

	freqPlot->setParent(mainWidget);
	freqPlot->setOpenGl(true);
	freqPlot->setBackground(QBrush(dark, Qt::SolidPattern));
	freqPlot->xAxis->setBasePen(QPen(silver));
	freqPlot->xAxis->setTickPen(QPen(silver));
	freqPlot->xAxis->setSubTickPen(QPen(silver));
	freqPlot->xAxis->setTickLabelColor(Qt::white);
	freqPlot->yAxis->setBasePen(QPen(silver));
	freqPlot->yAxis->setTickPen(QPen(silver));
	freqPlot->yAxis->setSubTickPen(QPen(silver));
	freqPlot->yAxis->setTickLabelColor(Qt::white);
	freqPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	freqPlot->axisRect()->setupFullAxesBox(true);

	detectPlot->setParent(mainWidget);
	detectPlot->setOpenGl(true);
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
	
	path->setParent(menu);
	path->setWordWrap(true);
	path->setAlignment(Qt::AlignCenter);

	selectButton->setParent(menu);
	plotButton->setParent(menu);
	selectButtonLayout->addSpacerItem(spacer1);
	selectButtonLayout->addWidget(selectButton);
	selectButtonLayout->addWidget(plotButton);
	selectButtonLayout->addSpacerItem(spacer2);

	channelLabel->setParent(menu);
	channelBox->setParent(menu);
	channelBoxLayout->addWidget(channelLabel, 6);
	channelBoxLayout->addWidget(channelBox, 4);

	winTypeLabel->setParent(menu);
	winTypeBox->setParent(menu);
	winTypeBoxLayout->addWidget(winTypeLabel, 6);
	winTypeBoxLayout->addWidget(winTypeBox, 4);

	nwinLabel->setParent(menu);
	nwinBox->setParent(menu);
	nwinBox->setSingleStep(1);
	nwinBox->setMinimum(1);
	nwinBox->setMaximum(100);
	nwinBox->setAlignment(Qt::AlignRight);
	nwinBoxLayout->addWidget(nwinLabel, 6);
	nwinBoxLayout->addWidget(nwinBox, 4);

	wlenLabel->setParent(menu);
	wlenBox->setParent(menu);
	wlenBoxLayout->addWidget(wlenLabel, 6);
	wlenBoxLayout->addWidget(wlenBox, 4);

	overlapLabel->setParent(menu);
	overlapBox->setParent(menu);
	overlapBox->setSingleStep(1);
	overlapBox->setMinimum(1);
	overlapBox->setMaximum(4095);
	overlapBox->setAlignment(Qt::AlignRight);
	overlapBoxLayout->addWidget(overlapLabel, 6);
	overlapBoxLayout->addWidget(overlapBox, 4);

	infLabel->setParent(menu);
	infBox->setParent(menu);
	infBox->setDecimals(4);
	infBox->setSingleStep(0.0001);
	infBox->setMinimum(0.0001);
	infBox->setMaximum(100);
	infBox->setAlignment(Qt::AlignRight);
	infBoxLayout->addWidget(infLabel, 6);
	infBoxLayout->addWidget(infBox, 4);

	alphaLabel->setParent(menu);
	alphaBox->setParent(menu);
	alphaBox->setDecimals(4);
	alphaBox->setSingleStep(0.0001);
	alphaBox->setMinimum(0.0001);
	alphaBox->setMaximum(100);
	alphaBox->setAlignment(Qt::AlignRight);
	alphaBoxLayout->addWidget(alphaLabel, 6);
	alphaBoxLayout->addWidget(alphaBox, 4);

	radioGroup->setParent(radioMenu);
	radio_high->setParent(radioMenu);
	radio_low->setParent(radioMenu);
	radio_pass->setParent(radioMenu);
	radio_stop->setParent(radioMenu);

	radioMenu->setParent(menu);
	radioMenu->setLineWidth(2);
	radioMenu->setFrameShadow(QFrame::Sunken);
	radioMenu->setFrameShape(QFrame::Box);
	radioMenuLayout->addWidget(radio_high);
	radioMenuLayout->addWidget(radio_low);
	radioMenuLayout->addWidget(radio_pass);
	radioMenuLayout->addWidget(radio_stop);
	radioMenu->setLayout(radioMenuLayout);

	lowFreqLabel->setParent(menu);
	//lowFreqLabel->setStyleSheet("color: rgb(192, 192, 192)");
	lowFreqBox->setParent(menu);
	lowFreqBox->setDecimals(4);
	lowFreqBox->setSingleStep(0.0001);
	lowFreqBox->setMinimum(0.0001);
	lowFreqBox->setMaximum(1);
	lowFreqBox->setAlignment(Qt::AlignRight);
	//lowFreqBox->setStyleSheet("color: rgb(192, 192, 192)");
	lowFreqBoxLayout->addWidget(lowFreqLabel);
	lowFreqBoxLayout->addWidget(lowFreqBox);

	highFreqLabel->setParent(menu);
	//highFreqLabel->setStyleSheet("color: rgb(192, 192, 192)");
	highFreqBox->setParent(menu);
	highFreqBox->setDecimals(4);
	highFreqBox->setSingleStep(0.0001);
	highFreqBox->setMinimum(0.0001);
	highFreqBox->setMaximum(1);
	highFreqBox->setAlignment(Qt::AlignRight);
	//highFreqBox->setStyleSheet("color: rgb(192, 192, 192)");
	highFreqBoxLayout->addWidget(highFreqLabel);
	highFreqBoxLayout->addWidget(highFreqBox);

	cutFreqLayout->addLayout(lowFreqBoxLayout);
	cutFreqLayout->addLayout(highFreqBoxLayout);
	filterGroupLayout->addWidget(radioMenu, 2);
	filterGroupLayout->addLayout(cutFreqLayout, 8);

	subMenuLayout->addSpacerItem(spacer3);
	subMenuLayout->addWidget(path);
	subMenuLayout->addLayout(selectButtonLayout);
	subMenuLayout->addLayout(channelBoxLayout);
	subMenuLayout->addLayout(winTypeBoxLayout);
	subMenuLayout->addLayout(nwinBoxLayout);
	subMenuLayout->addLayout(wlenBoxLayout);
	subMenuLayout->addLayout(overlapBoxLayout);
	subMenuLayout->addLayout(infBoxLayout);
	subMenuLayout->addLayout(alphaBoxLayout);
	subMenuLayout->addLayout(filterGroupLayout);
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

	// Connect functions
	connect(selectButton, &QPushButton::pressed, this, &MainWindow::selectFile);
	connect(plotButton, &QPushButton::pressed, this, [this]{ this->updatePlots(false); }, Qt::ConnectionType::QueuedConnection);

	// Init processing
	double rFreq = 200.0 / 2048;
	windowFunctions = init_windows();
	init_globalvar(4096, 8, 200, 3.5, 0.5, ippsHighPass, &rFreq, 16);

	// Last changes
	// this->setStyleSheet("background-color: rgb(41,43,44);");	// Dark
	this->setCentralWidget(mainWidget);
	this->resize(QGuiApplication::primaryScreen()->geometry().width() / 1.3, QGuiApplication::primaryScreen()->geometry().height() / 1.3);
	this->retranslateUi();
}

void MainWindow::retranslateUi()
{
	fftPlot->xAxis->setLabel(tr("Frequency"));
	detectPlot->xAxis->setLabel(tr("Time"));
	path->setText("");
	selectButton->setText(tr("Select File"));
	plotButton->setText(tr("Plot"));
	channelLabel->setText(tr("Channel :"));
	winTypeLabel->setText(tr("Window Type :"));
	wlenLabel->setText(tr("Window Length :"));
	overlapLabel->setText(tr("Overlap :"));
	nwinLabel->setText(tr("Window Number :"));
	infLabel->setText(tr("Influence :"));
	alphaLabel->setText(tr("Alpha :"));
	radio_high->setText(tr("High Pass"));
	radio_low->setText(tr("Low Pass"));
	radio_pass->setText(tr("Band Pass"));
	radio_stop->setText(tr("Band Stop"));
	lowFreqLabel->setText(tr("Low Cutoff Frequency:"));
	highFreqLabel->setText(tr("High Cutoff Frequency:"));

	for (int i = 0; i < log2(MAX_WLEN) - log2(MIN_WLEN) + 1; ++i) {
		wlenBox->addItem(QString::number(MAX_WLEN / pow(2, i)));
	}
	winTypeBox->addItem(QString("Hanning"));
	winTypeBox->addItem(QString("Hamming"));
	winTypeBox->addItem(QString("Rectangle"));
	winTypeBox->addItem(QString("Blackman"));
	winTypeBox->addItem(QString("Bartlett"));

	// Set defaults
	nwinBox->setValue(8);
	winTypeBox->setCurrentIndex(0);
	wlenBox->setCurrentIndex(3);
	overlapBox->setValue(2048);
	infBox->setValue(0.5);
	alphaBox->setValue(3.5);
	radio_high->setChecked(true);
	lowFreqBox->setValue(200.0 / 2048);
	highFreqBox->setValue(0.98);
	highFreqBox->setEnabled(false);

	this->setWindowTitle(tr("Frequency Detector"));
}

MainWindow::~MainWindow()
{
	delete mainWidget;
}

void MainWindow::selectFile()
{
	QString fileName = QFileDialog::getOpenFileName(this);
	path->setText(fileName);
	
#ifdef _WIN32
	memset(filePath, '\0', FILENAME_MAX);
	strcpy_s(filePath, FILENAME_MAX, fileName.toStdString().c_str());
#else
	memset(filePath, '\0', PATH_MAX);
	strcpy(filePath, fileName.toStdString().c_str());
#endif // WIN32
	
	// Trigger processes
	if (fileName.length())
		QMetaObject::invokeMethod(this, "updatePlots", Qt::ConnectionType::QueuedConnection, Q_ARG(bool, true));

}

Q_INVOKABLE void MainWindow::updatePlots(bool flag)
{
	ERR_STATUS status = 0;
	disableButtons();
	//updateValues();

	if (flag) { // If file changed
		// Dealloc memory
		deinitAudioReaderStruct(&reader);
		deallocAudioData(&audio);

		// Read new file
		status = initAudioReaderStruct(filePath, &reader);
		status = readAudioFile(&reader, &audio);
	}

	if (spectrogramData) { // Clear previous data
		for (int i = 0; i < outputLength; ++i) {
			MKL_free(spectrogramData[i]);
			MKL_free(alarmsData[i]);
		}
		MKL_free(alarmLengths);
		MKL_free(alarmsData);
		MKL_free(spectrogramData);
	}

	int bits = 0;
	if (reader.codecContexts[streamIdx]->bits_per_raw_sample)
		bits = reader.codecContexts[streamIdx]->bits_per_raw_sample;
	else if (reader.codecContexts[streamIdx]->bits_per_coded_sample)
		bits = reader.codecContexts[streamIdx]->bits_per_coded_sample;
	else // Should not be reached
		bits = 16;

	status = processFile(&audio, streamIdx, channelIdx, windowFunctions[winIdx], windowLength, overlap, bits, &spectrogramData, &alarmsData, &alarmLengths, &outputLength);

	// Plot time data
	double *x = (double*)malloc(audio.data->dataLen * sizeof(double));
	double *y = audio.data->channelData[channelIdx];
	for (size_t idx = 0; idx < audio.data->dataLen; ++idx)
		x[idx] = idx;
	QVector<double> vX(x, &x[audio.data->dataLen]);
	QVector<double> vY(y, &y[audio.data->dataLen]);
	timePlot->graph(0)->setData(vX, vY, true);
	timePlot->graph(0)->rescaleAxes();
	timePlot->replot();

	// Plot spectrogram
	if (freqMap)
		delete freqMap;
	freqMap = new QCPColorMap(freqPlot->xAxis, freqPlot->yAxis);
	freqMap->setGradient(QCPColorGradient::gpThermal);
	freqMap->setDataRange(QCPRange(-150, 0));
	freqMap->data()->setSize(floor((audio.data->dataLen - windowLength) / (windowLength - overlap)) + 1, windowLength / 2 + 1);
	for (size_t idx = 0; idx < floor((audio.data->dataLen - windowLength) / (windowLength - overlap)) + 1; ++idx)
	{
		double *ptr = spectrogramData[idx];
		for (size_t jdx = 0; jdx < windowLength / 2 + 1; ++jdx)
		{
			freqMap->data()->setCell(idx, jdx, ptr[jdx]);
		}
	}
	freqMap->rescaleAxes();
	freqPlot->replot();

	// Plot detections

	enableButtons();
}

void MainWindow::updateValues()
{
}

void MainWindow::enableButtons()
{
	selectButton->setEnabled(true);
	plotButton->setEnabled(true);
	
	radio_low->setEnabled(true);
	radio_high->setEnabled(true);
	radio_pass->setEnabled(true);
	radio_stop->setEnabled(true);

	this->update();
}

void MainWindow::disableButtons()
{
	selectButton->setEnabled(false);
	plotButton->setEnabled(false);

	radio_low->setEnabled(false);
	radio_high->setEnabled(false);
	radio_pass->setEnabled(false);
	radio_stop->setEnabled(false);

	this->update();
}
