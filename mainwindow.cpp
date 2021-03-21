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
	spacer4 = new QSpacerItem(40, 20, QSizePolicy::Expanding);
	spacer5 = new QSpacerItem(40, 20, QSizePolicy::Expanding);

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

	backwardButton->setParent(menu);
	backwardButton->setFixedSize((backwardButton->iconSize() += backwardButton->iconSize() / 2));
	backwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));

	playButton->setParent(menu);
	playButton->setCheckable(true);
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
	connect(selectButton, &QPushButton::pressed, this, &MainWindow::selectFile);
	connect(plotButton, &QPushButton::pressed, this, [this]{ this->updatePlots(false); }, Qt::ConnectionType::QueuedConnection);
	connect(playButton, &QPushButton::pressed, this, &MainWindow::playMedia);
	connect(stopButton, &QPushButton::pressed, this, &MainWindow::stopMedia);
	connect(backwardButton, &QPushButton::pressed, this, &MainWindow::backwardMedia);
	connect(forwardButton, &QPushButton::pressed, this, &MainWindow::forwardMedia);

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
	{
		this->disableButtons();
		updateValues();
		std::thread th(&MainWindow::updatePlots, this, true);
		th.detach();
	}
}

Q_INVOKABLE void MainWindow::updatePlots(bool flag)
{
	ERR_STATUS status = 0;

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

	double timeLimit = audio.data->dataLen / audio.data->samplingFreq;
	
	// Plot time data
	double *x = (double*)malloc(audio.data->dataLen * sizeof(double));
	double *y = audio.data->channelData[channelIdx];
	for (size_t idx = 0; idx < audio.data->dataLen; ++idx)
		x[idx] = idx;
	QVector<double> vX(x, &x[audio.data->dataLen]);
	QVector<double> vY(y, &y[audio.data->dataLen]);
	timePlot->graph(0)->setData(vX, vY, true);
	timePlot->graph(0)->rescaleAxes();
	QMetaObject::invokeMethod(this->timePlot, "replot", Qt::ConnectionType::QueuedConnection);

	// Plot spectrogram
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
	QMetaObject::invokeMethod(this->freqPlot, "replot", Qt::ConnectionType::QueuedConnection);

	// Plot detections
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

	free(x);
	QMetaObject::invokeMethod(this, "enableButtons", Qt::ConnectionType::QueuedConnection);
}

Q_INVOKABLE void MainWindow::updateFFTPlot()
{
	return void();
}

void MainWindow::updateValues()
{
}

void MainWindow::playMedia()
{
	if (!this->playButton->isChecked())
	{
		playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
		playButton->update();

		float *arrBuff = (float*)malloc(audio.data->dataLen * sizeof(float));

		size_t ind1 = cblas_idamax(audio.data->dataLen, audio.data->channelData[channelIdx], 1);
		size_t ind2 = cblas_idamin(audio.data->dataLen, audio.data->channelData[channelIdx], 1);

		double max_val;
		if(audio.data->channelData[channelIdx][ind1] > abs(audio.data->channelData[channelIdx][ind2]))
			max_val = audio.data->channelData[channelIdx][ind1];
		else
			max_val = abs(audio.data->channelData[channelIdx][ind2]);

		for (size_t jdx = 0; jdx < audio.data->dataLen; ++jdx)
			arrBuff[jdx] = audio.data->channelData[channelIdx][jdx] / max_val;
		QByteArray *bArr = new QByteArray((char*)arrBuff, audio.data->dataLen * sizeof(float));
		QBuffer *buff = new QBuffer(bArr);
		free(arrBuff);
		buff->open(QBuffer::ReadOnly);

		QAudioFormat format;
		format.setSampleRate(audio.data->samplingFreq);
		format.setChannelCount(1);
		format.setSampleSize(32);
		format.setCodec("audio/pcm");
		format.setByteOrder(QAudioFormat::LittleEndian);
		format.setSampleType(QAudioFormat::SampleType::Float);

		if (audioDev)
			delete audioDev;

		QAudioDeviceInfo deviceInfo(QAudioDeviceInfo::defaultOutputDevice());
		if(!deviceInfo.isFormatSupported(format))
			format = deviceInfo.nearestFormat(format);
		audioDev = new QAudioOutput(deviceInfo, format);
		if (audioDev->error() != QAudio::NoError)
			return;

		audioDev->setVolume(0.5);
		audioDev->start(buff);
	}
	else
	{
		playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
		playButton->update();

		audioDev->suspend();
	}
}

void MainWindow::stopMedia()
{
}

void MainWindow::backwardMedia()
{
}

void MainWindow::forwardMedia()
{
}

Q_INVOKABLE void MainWindow::enableButtons()
{
	selectButton->setEnabled(true);
	plotButton->setEnabled(true);
	
	radio_low->setEnabled(true);
	radio_high->setEnabled(true);
	radio_pass->setEnabled(true);
	radio_stop->setEnabled(true);

	this->update();
}

Q_INVOKABLE void MainWindow::disableButtons()
{
	selectButton->setEnabled(false);
	plotButton->setEnabled(false);

	radio_low->setEnabled(false);
	radio_high->setEnabled(false);
	radio_pass->setEnabled(false);
	radio_stop->setEnabled(false);

	this->update();
}
