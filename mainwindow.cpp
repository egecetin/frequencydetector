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
	freqMap = new QCPColorMap(freqPlot->xAxis, freqPlot->yAxis);
	freqMap->setVisible(false);

	infLine = new QCPItemStraightLine(timePlot);
	infLine->setLayer(timePlot->layer("axes"));

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
	playButton = new QPushButton;
	stopButton = new QPushButton;
	forwardButton = new QPushButton;
	backwardButton = new QPushButton;

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
	playerButtonsBoxLayout = new QHBoxLayout;

	spacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding);
	spacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding);
	spacer3 = new QSpacerItem(20, 5, QSizePolicy::Expanding);
	spacer4 = new QSpacerItem(40, 20, QSizePolicy::Maximum);
	spacer5 = new QSpacerItem(40, 20, QSizePolicy::Maximum);
	
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
	timePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	connect(timePlot->xAxis, SIGNAL(rangeChanged(QCPRange)), timePlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(timePlot->yAxis, SIGNAL(rangeChanged(QCPRange)), timePlot->yAxis2, SLOT(setRange(QCPRange)));
	//connect(timePlot, &QCustomPlot::beforeReplot, this, [this] { this->limitRange(timePlot, &timeXAxes, &timeYAxes); } );

	infLine->point1->setCoords(0, 0);
	infLine->point2->setCoords(0, 1);
	infLine->setPen(QPen(crimson));

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
	connect(freqPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), freqPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(freqPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), freqPlot->yAxis2, SLOT(setRange(QCPRange)));
	//connect(freqPlot, &QCustomPlot::beforeReplot, this, [this] { this->limitRange(freqPlot, &freqXAxes, &freqYAxes); });
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
	detectPlot->addGraph();
	detectPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssStar);
	detectPlot->graph(0)->setPen(QPen(darkorange));
	detectPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
	detectPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	connect(detectPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), detectPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(detectPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), detectPlot->yAxis2, SLOT(setRange(QCPRange)));
	//connect(detectPlot, &QCustomPlot::beforeReplot, this, [this] { this->limitRange(detectPlot, &freqXAxes, &freqYAxes); });
	
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
	fftPlot->addGraph();
	fftPlot->graph(0)->setPen(QPen(darkorange));
	fftPlot->addGraph();
	fftPlot->graph(1)->setPen(QPen(aquamarine));
	fftPlot->graph(1)->setBrush(QBrush(QColor(127, 255, 212, 40)));
	fftPlot->addGraph();
	fftPlot->graph(2)->setPen(QPen(lime));
	fftPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
	fftPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssStar);

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
	channelBox->addItem("0");
	channelBox->setEditable(true);
	channelBox->lineEdit()->setReadOnly(true);
	channelBox->lineEdit()->setAlignment(Qt::AlignRight);
	channelBoxLayout->addWidget(channelLabel, 6);
	channelBoxLayout->addWidget(channelBox, 4);

	winTypeLabel->setParent(menu);
	winTypeBox->setParent(menu);
	winTypeBox->setEditable(true);
	winTypeBox->lineEdit()->setReadOnly(true);
	winTypeBox->lineEdit()->setAlignment(Qt::AlignRight);
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
	wlenBox->setEditable(true);
	wlenBox->lineEdit()->setReadOnly(true);
	wlenBox->lineEdit()->setAlignment(Qt::AlignRight);
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
	radio_low->setParent(radioMenu);
	radioGroup->addButton(radio_low, 0);
	radio_high->setParent(radioMenu);
	radioGroup->addButton(radio_high, 1);
	radio_pass->setParent(radioMenu);
	radioGroup->addButton(radio_pass, 2);
	radio_stop->setParent(radioMenu);
	radioGroup->addButton(radio_stop, 3);

	radioMenu->setParent(menu);
	radioMenu->setLineWidth(2);
	radioMenu->setFrameShadow(QFrame::Sunken);
	radioMenu->setFrameShape(QFrame::Box);
	radioMenuLayout->addWidget(radio_low);
	radioMenuLayout->addWidget(radio_high);
	radioMenuLayout->addWidget(radio_pass);
	radioMenuLayout->addWidget(radio_stop);
	radioMenu->setLayout(radioMenuLayout);

	lowFreqLabel->setParent(menu);
	lowFreqBox->setParent(menu);
	lowFreqBox->setDecimals(4);
	lowFreqBox->setSingleStep(0.0001);
	lowFreqBox->setMinimum(0.0001);
	lowFreqBox->setMaximum(1);
	lowFreqBox->setAlignment(Qt::AlignRight);
	lowFreqBoxLayout->addWidget(lowFreqLabel);
	lowFreqBoxLayout->addWidget(lowFreqBox);

	highFreqLabel->setParent(menu);
	highFreqBox->setParent(menu);
	highFreqBox->setDecimals(4);
	highFreqBox->setSingleStep(0.0001);
	highFreqBox->setMinimum(0.0001);
	highFreqBox->setMaximum(1);
	highFreqBox->setAlignment(Qt::AlignRight);
	highFreqBoxLayout->addWidget(highFreqLabel);
	highFreqBoxLayout->addWidget(highFreqBox);

	cutFreqLayout->addLayout(lowFreqBoxLayout);
	cutFreqLayout->addLayout(highFreqBoxLayout);
	filterGroupLayout->addWidget(radioMenu, 2);
	filterGroupLayout->addLayout(cutFreqLayout, 8);

	backwardButton->setParent(menu);
	backwardButton->setFixedSize((backwardButton->iconSize() += backwardButton->iconSize() / 2));
	backwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));

	playButton->setParent(menu);
	playButton->setFixedSize((playButton->iconSize() += playButton->iconSize() / 2));
	playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

	stopButton->setParent(menu);
	stopButton->setFixedSize((stopButton->iconSize() += stopButton->iconSize() / 2));
	stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

	forwardButton->setParent(menu);
	forwardButton->setFixedSize((forwardButton->iconSize() += forwardButton->iconSize() / 2));
	forwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));

	playerButtonsBoxLayout->addSpacerItem(spacer4);
	playerButtonsBoxLayout->addWidget(backwardButton);
	playerButtonsBoxLayout->addWidget(playButton);
	playerButtonsBoxLayout->addWidget(stopButton);
	playerButtonsBoxLayout->addWidget(forwardButton);
	playerButtonsBoxLayout->addSpacerItem(spacer5);
	playerButtonsBoxLayout->setSpacing(0);

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
	subMenuLayout->addLayout(playerButtonsBoxLayout);
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
	connect(selectButton, &QPushButton::pressed, this, [this] { this->selectFile(true); }, Qt::ConnectionType::QueuedConnection);
	connect(plotButton, &QPushButton::pressed, this, [this] { this->selectFile(false); }, Qt::ConnectionType::QueuedConnection);
	connect(playButton, &QPushButton::pressed, this, &MainWindow::playMedia);
	connect(stopButton, &QPushButton::pressed, this, &MainWindow::stopMedia);
	connect(backwardButton, &QPushButton::pressed, this, &MainWindow::backwardMedia);
	connect(forwardButton, &QPushButton::pressed, this, &MainWindow::forwardMedia);
	connect(wlenBox, &QComboBox::currentTextChanged, this, &MainWindow::wLenChanged);
	connect(overlapBox, static_cast<void(QSpinBox::*)(int)>(QSpinBox::valueChanged), this, &MainWindow::overlapChanged);
	connect(radioGroup, static_cast<void(QButtonGroup::*)(int)>(QButtonGroup::buttonClicked), this, &MainWindow::radioChange);


	// Init processing with default values
	double rFreq[] = { 300.0 / 2048,0 };
	windowFunctions = init_windows();
	init_globalvar(4096, 8, 300 / M_PI, 3.5, 0.5, ippsHighPass, rFreq, 16);

	// Last changes
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
	channelBox->setCurrentIndex(0);
	nwinBox->setValue(8);
	winTypeBox->setCurrentIndex(0);
	wlenBox->setCurrentIndex(3);
	overlapBox->setValue(2048);
	infBox->setValue(0.5);
	alphaBox->setValue(3.5);
	radio_high->setChecked(true);
	lowFreqBox->setEnabled(true);
	lowFreqBox->setValue(300.0 / 2048);
	highFreqBox->setValue(0.98);
	highFreqBox->setEnabled(false);

	this->setWindowTitle(tr("Frequency Detector"));
}

MainWindow::~MainWindow()
{
	delete mainWidget;
}

void MainWindow::selectFile(bool flag)
{
	QString fileName;
	if (flag)
	{
		fileName = QFileDialog::getOpenFileName(this);
		path->setText(fileName);

#ifdef _WIN32
		memset(filePath, '\0', FILENAME_MAX);
		strcpy_s(filePath, FILENAME_MAX, fileName.toStdString().c_str());
#else
		memset(filePath, '\0', PATH_MAX);
		strcpy(filePath, fileName.toStdString().c_str());
#endif // WIN32
	}	
	
	// Trigger processes
	if (fileName.length() || (!flag && this->filePath[0]))
	{
		this->disableButtons();
		this->updateValues();
		if (dialog)
			delete dialog;
		dialog = new QProgressDialog("Processing request...", "", 0, 100, this);
		dialog->setWindowModality(Qt::WindowModality::WindowModal);
		dialog->setCancelButton(nullptr);
		dialog->setWindowFlags(dialog->windowFlags() | Qt::FramelessWindowHint | Qt::WindowTitleHint);
		
		dialog->setValue(5);
		dialog->setVisible(true);
		dialog->update();

		std::thread th(&MainWindow::updatePlots, this, flag);
		th.detach();
	}
}

Q_INVOKABLE void MainWindow::updatePlots(bool flag)
{
	ERR_STATUS status = 0;

	if (flag) { // If file changed
		QMetaObject::invokeMethod(this->dialog, "setLabelText", Qt::QueuedConnection, Q_ARG(QString, "Reading file..."));
		QMetaObject::invokeMethod(this->dialog, "setValue", Qt::QueuedConnection, Q_ARG(int, 10));

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

	bits = 0;
	if (reader.codecContexts[streamIdx]->bits_per_raw_sample)
		bits = reader.codecContexts[streamIdx]->bits_per_raw_sample;
	else if (reader.codecContexts[streamIdx]->bits_per_coded_sample)
		bits = reader.codecContexts[streamIdx]->bits_per_coded_sample;
	else // Should not be reached
		bits = 16;

	QMetaObject::invokeMethod(this->dialog, "setLabelText", Qt::QueuedConnection, Q_ARG(QString, "Processing data..."));
	QMetaObject::invokeMethod(this->dialog, "setValue", Qt::QueuedConnection, Q_ARG(int, 35));
	status = processFile(&audio, streamIdx, channelIdx, windowFunctions[winIdx], windowLength, overlap, bits, &spectrogramData, &alarmsData, &alarmLengths, &outputLength);

	double timeLimit = audio.data->dataLen / audio.data->samplingFreq;	

	// Plot time data
	QMetaObject::invokeMethod(this->dialog, "setLabelText", Qt::QueuedConnection, Q_ARG(QString, "Plotting..."));
	QMetaObject::invokeMethod(this->dialog, "setValue", Qt::QueuedConnection, Q_ARG(int, 75));
	double *x = (double*)malloc(audio.data->dataLen * sizeof(double));
	double *y = audio.data->channelData[channelIdx];
#pragma omp simd
	for (size_t idx = 0; idx < audio.data->dataLen; ++idx)
		x[idx] = idx;
	QVector<double> vX(x, &x[audio.data->dataLen]);
	QVector<double> vY(y, &y[audio.data->dataLen]);
	timePlot->graph(0)->setData(vX, vY, true);
	timePlot->graph(0)->rescaleAxes();

	size_t m_idx = cblas_idamax(audio.data->dataLen, y, 1);
	timeXAxes = QCPRange(0, audio.data->dataLen);
	timeYAxes = QCPRange(-y[m_idx], y[m_idx]);

	QMetaObject::invokeMethod(this->timePlot, "replot", Qt::ConnectionType::QueuedConnection);

	// Plot spectrogram
	QMetaObject::invokeMethod(this->dialog, "setValue", Qt::QueuedConnection, Q_ARG(int, 85));
	freqMap->setVisible(true);
	freqMap->data()->clear();
	freqMap->setGradient(QCPColorGradient::gpThermal);
	freqMap->setDataRange(QCPRange(-150, 0));
	freqMap->data()->setRange(QCPRange(0, floor((audio.data->dataLen - windowLength) / (windowLength - overlap)) + 1), QCPRange(0, windowLength / 2 + 1));
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
	freqXAxes = QCPRange(0, floor((audio.data->dataLen - windowLength) / (windowLength - overlap)) + 1);
	freqYAxes = QCPRange(0, windowLength / 2 + 1);
	QMetaObject::invokeMethod(this->freqPlot, "replot", Qt::ConnectionType::QueuedConnection);

	// Plot detections
	QMetaObject::invokeMethod(this->dialog, "setValue", Qt::QueuedConnection, Q_ARG(int, 95));
	detectPlot->graph(0)->data()->clear();
	for (size_t idx = 0; idx < floor((audio.data->dataLen - windowLength) / (windowLength - overlap)) + 1; ++idx)
	{
		QVector<double> vX2, vY2;
		double *ptr = alarmsData[idx];
		for (size_t jdx = 0; jdx < alarmLengths[idx]; ++jdx)
		{
			vY2 << ptr[jdx];
			vX2 << idx;
		}
		detectPlot->graph(0)->addData(vX2, vY2, true);
	}
	detectPlot->xAxis->setRange(0, floor((audio.data->dataLen - windowLength) / (windowLength - overlap)) + 1);
	detectPlot->yAxis->setRange(0, windowLength / 2 + 1);
	QMetaObject::invokeMethod(this->detectPlot, "replot", Qt::ConnectionType::QueuedConnection);	

	this->updateFFTPlot(0);

	if (audioDev)
	{
		delete audioDev;
		audioDev = nullptr;
	}
	audioDev = new QAudioOutput;

	QMetaObject::invokeMethod(this->dialog, "setValue", Qt::QueuedConnection, Q_ARG(int, 100));
	QMetaObject::invokeMethod(this, "enableButtons", Qt::ConnectionType::QueuedConnection);
	free(x);
}

Q_INVOKABLE void MainWindow::updateFFTPlot(int64_t pos)
{	
	size_t indx = 0;
	int n = 0;
	double *values = (double*)MKL_calloc(fBuffLen, sizeof(double), 64);
	double *th_values = (double*)MKL_calloc(fBuffLen, sizeof(double), 64);
	int64_t startPos = int64_t(pos / (windowLength - overlap) - nwin - 1) * (windowLength - overlap);
	if (startPos < 0)
		startPos = 0;

	double *out = estimate_freq_local(&(audio.data->channelData[channelIdx][startPos]), windowFunctions[winIdx], &n, windowLength - overlap, values, th_values);

	// Take the logarithm
	ippsAddC_64f_I(1e-14, values, fBuffLen);
	cblas_dscal(fBuffLen, pow(2, -bits) / nwin, values, 1);
	vdLog10(fBuffLen, values, values);
	indx = cblas_idamax(fBuffLen, values, 1);

	ippsAddC_64f_I(1e-14, th_values, fBuffLen);
	cblas_dscal(fBuffLen, pow(2, -bits) / nwin, th_values, 1);
	vdLog10(fBuffLen, th_values, th_values);

	// Set all values
	double *x = (double*)malloc(sizeof(double) * fBuffLen);
#pragma omp simd
	for (size_t idx = 0; idx < fBuffLen; ++idx)
		x[idx] = idx;
	QVector<double> vX(x, &x[fBuffLen]);
	QVector<double> vY(values, &values[fBuffLen]);
	fftPlot->graph(0)->setData(vX, vY, true);
	fftPlot->xAxis->setRange(0, fBuffLen);
	fftPlot->yAxis->setRange(-round(-(values[indx] / 10 - 1)) * 5, 0);

	// Set threshold values
	vY = QVector<double>(th_values, &th_values[fBuffLen]);
	fftPlot->graph(1)->setData(vX, vY, true);

	// Mark estimated values
	//fftPlot->graph(2)->setData();	

	QMetaObject::invokeMethod(this->fftPlot, "replot", Qt::ConnectionType::QueuedConnection);
}

void MainWindow::updateValues()
{
	windowLength = std::stoi(this->wlenBox->currentText().toStdString());
	overlap = this->overlapBox->value();
	nwin = this->nwinBox->value();
	thresh_alpha = this->alphaBox->value();
	thresh_inf = this->infBox->value();	
	channelIdx = this->channelBox->currentIndex();
	winIdx = this->wlenBox->currentIndex() * 5 + this->winTypeBox->currentIndex();

	double rFreq[] = { this->lowFreqBox->value(), this->highFreqBox->value() };
	init_globalvar(windowLength, nwin, rFreq[0] * windowLength / 2 / M_PI, thresh_alpha, thresh_inf, static_cast<IppFilterType>(radioGroup->checkedId()), rFreq, bits);
}

void MainWindow::playMedia()
{
	switch (audioDev->state())
	{
	case QAudio::State::ActiveState:
	{
		playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
		audioDev->suspend();

		playButton->update();
		break;
	}
	case QAudio::State::SuspendedState:
		playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
		audioDev->resume();
		playButton->update();
		break;
	case QAudio::State::StoppedState:
	case QAudio::State::IdleState:
	{
		// Prepare audio output device
		QAudioFormat format;
		format.setSampleRate(audio.data->samplingFreq);
		format.setChannelCount(1);
		format.setSampleSize(32);
		format.setCodec("audio/pcm");
		format.setByteOrder(QAudioFormat::LittleEndian);
		format.setSampleType(QAudioFormat::SampleType::Float);

		if (audioDev)
		{
			delete audioDev;
			audioDev = nullptr;
		}
		if (audioBuff)
		{
			delete audioBuff;
			audioBuff = nullptr;
		}

		audioBuff = new QBuffer;

		QAudioDeviceInfo deviceInfo(QAudioDeviceInfo::defaultOutputDevice());
		if (!deviceInfo.isFormatSupported(format))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Format is not supported. Trying with nearest format!"));
			format = deviceInfo.nearestFormat(format);
		}

		audioDev = new QAudioOutput(deviceInfo, format);
		if (audioDev->error() != QAudio::NoError)
		{
			QMessageBox::critical(this, tr("Error"), tr("Cant access the audio device!"));
		}
		audioDev->setVolume(0.75);
		audioDev->setNotifyInterval(100);
		connect(audioDev, &QAudioOutput::notify, this, &MainWindow::playerSlider, Qt::ConnectionType::QueuedConnection);
		connect(audioDev, &QAudioOutput::stateChanged, this, &MainWindow::playerStateChanged, Qt::ConnectionType::QueuedConnection);
		
		// Prepare data
		double max_val = 0;
		float *arrBuff = (float*)malloc(audio.data->dataLen * sizeof(float));

		// Find absolute max
		size_t ind1 = cblas_idamax(audio.data->dataLen, audio.data->channelData[channelIdx], 1);
		max_val = abs(audio.data->channelData[channelIdx][ind1]);

		// Change precision and scale
		ippsConvert_64f32f(audio.data->channelData[channelIdx], arrBuff, audio.data->dataLen);
		cblas_sscal(audio.data->dataLen, 1.0 / max_val, arrBuff, 1);

		// Set buffer
		QByteArray bArr = QByteArray((char*)arrBuff, audio.data->dataLen * sizeof(float));
		audioBuff->setData(bArr);
		audioBuff->open(QBuffer::ReadOnly);

		free(arrBuff);

		// Start audio and update gui
		playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
		audioDev->start(audioBuff);
		playButton->update();
		break;
	}
	case QAudio::State::InterruptedState:
		QMessageBox::warning(this, tr("Warning"), tr("Another process has control of device!"));
		break;	
	default:
		QMessageBox::critical(this, tr("Error"), tr("Unknown audio device state!"));
		break;
	}
}

void MainWindow::stopMedia()
{
	playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	audioDev->stop();
	playButton->update();
}

void MainWindow::backwardMedia()
{
	int64_t pos = audioBuff->pos() - this->audio.data->samplingFreq * 5 * sizeof(float);
	if (pos < 0)
		audioBuff->seek(0);
	else
		audioBuff->seek(pos);
}

void MainWindow::forwardMedia()
{
	int64_t pos = audioBuff->pos() + this->audio.data->samplingFreq * 5 * sizeof(float);
	if (pos > audioBuff->size())
		audioBuff->seek(audioBuff->size());
	else
		audioBuff->seek(pos);
}

void MainWindow::playerSlider()
{
	infLine->point1->setCoords(audioBuff->pos() / sizeof(float), 0);
	infLine->point2->setCoords(audioBuff->pos() / sizeof(float), 1);
	updateFFTPlot(audioBuff->pos() / sizeof(float));
	QMetaObject::invokeMethod(this->timePlot, "replot", Qt::ConnectionType::QueuedConnection);
}

void MainWindow::playerStateChanged(QAudio::State state)
{
	switch (state)
	{
	case QAudio::ActiveState:
		break;
	case QAudio::SuspendedState:
		break;
	case QAudio::StoppedState:
		break;
	case QAudio::IdleState:
		playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
		audioDev->stop();
		playButton->update();
		break;
	case QAudio::InterruptedState:
		break;
	default:
		break;
	}
}

void MainWindow::radioChange(int id)
{
	switch (radioGroup->checkedId())
	{
	case 0:
		lowFreqBox->setEnabled(false);
		highFreqBox->setEnabled(true);
		break;
	case 1:
		lowFreqBox->setEnabled(true);
		highFreqBox->setEnabled(false);
		break;
	case 2:
	case 3:
		lowFreqBox->setEnabled(true);
		highFreqBox->setEnabled(true);
		break;
	default:
		break;
	}

	this->update();
}

void MainWindow::wLenChanged(const QString &val)
{
	if (overlapBox->value() >= std::stoi(val.toStdString()))
	{
		overlapBox->setValue(std::stoi(val.toStdString()) / 2);
	}
	overlapBox->setMaximum(std::stoi(val.toStdString()) - 1);
}

void MainWindow::overlapChanged(int val)
{
	if (val >= std::stoi(wlenBox->currentText().toStdString()))
	{
		this->overlapBox->setValue(std::stoi(wlenBox->currentText().toStdString()) - 1);
	}
}

void MainWindow::limitRange(QCustomPlot *plot, QCPRange *X, QCPRange *Y)
{
	if (plot->xAxis->range().lower < X->lower)
		plot->xAxis->setRangeLower(X->lower);
	if (plot->xAxis->range().upper > X->upper)
		plot->xAxis->setRangeUpper(X->upper);
	if (plot->yAxis->range().lower < Y->lower)
		plot->yAxis->setRangeLower(Y->lower);
	if (plot->yAxis->range().upper > Y->upper)
		plot->yAxis->setRangeUpper(Y->upper);
}

Q_INVOKABLE void MainWindow::enableButtons()
{
	backwardButton->setEnabled(true);
	playButton->setEnabled(true);
	stopButton->setEnabled(true);
	forwardButton ->setEnabled(true);

	nwinBox->setEnabled(true);
	winTypeBox->setEnabled(true);
	wlenBox->setEnabled(true);
	overlapBox->setEnabled(true);
	infBox->setEnabled(true);
	alphaBox->setEnabled(true);

	switch (radioGroup->checkedId())
	{
	case 0:
		lowFreqBox->setEnabled(false);
		highFreqBox->setEnabled(true);
		break;
	case 1:
		lowFreqBox->setEnabled(true);
		highFreqBox->setEnabled(false);
		break;
	case 2:
	case 3:
		lowFreqBox->setEnabled(true);
		highFreqBox->setEnabled(true);
		break;
	default:
		break;
	}

	selectButton->setEnabled(true);
	plotButton->setEnabled(true);
	
	radio_low->setEnabled(true);
	radio_high->setEnabled(true);
	radio_pass->setEnabled(true);
	radio_stop->setEnabled(true);


	// Update channel number
	this->channelBox->clear();
	for (size_t idx = 0; idx < audio.data->nChannel; ++idx)
		this->channelBox->addItem(std::to_string(idx).c_str());

	if (this->dialog)
		this->dialog->setValue(100);

	this->update();
}

Q_INVOKABLE void MainWindow::disableButtons()
{
	backwardButton->setEnabled(false);
	playButton->setEnabled(false);
	stopButton->setEnabled(false);
	forwardButton->setEnabled(false);

	nwinBox->setEnabled(false);
	winTypeBox->setEnabled(false);
	wlenBox->setEnabled(false);
	overlapBox->setEnabled(false);
	infBox->setEnabled(false);
	alphaBox->setEnabled(false);
	lowFreqBox->setEnabled(false);
	highFreqBox->setEnabled(false);

	selectButton->setEnabled(false);
	plotButton->setEnabled(false);

	radio_low->setEnabled(false);
	radio_high->setEnabled(false);
	radio_pass->setEnabled(false);
	radio_stop->setEnabled(false);

	this->update();
}
