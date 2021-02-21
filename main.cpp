#include "mainwindow.h"

#include <QApplication>

#include <qdebug.h>
#include <qglobal.h>

int main(int argc, char *argv[])
{
	//AudioReader reader = AudioReader_Default;
	//AudioData audio = AudioData_Default;
	//char filename[] = "D:\\VS_Repos\\Frequency Detector\\01_This_Is_War_(Apex_Remix).flac";
	//char filename[] = "C:\\Users\\egece\\Desktop\\Alarm.wav";
	
	//initAudioReaderStruct(filename, &reader);
	//readAudioFile(&reader, &audio);
	//deinitAudioReaderStruct(&reader);

	//FILE *fptr = fopen("out.txt","w+");
	//double *ptr = audio.data->channelData[0];
	//for (int i = 0; i < audio.data->dataLen; ++i) {
	//	fprintf(fptr, "%f\n", ptr[i]);
	//}
	//fclose(fptr);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
