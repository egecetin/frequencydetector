#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	AudioReader reader = AudioReader_Default;
	char filename[] = "C:\\Users\\egece\\Desktop\\01_This_Is_War_(Apex_Remix).flac";
		
	initAudioReaderStruct(filename, &reader);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
