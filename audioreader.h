#pragma once

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>

	// Definitions
	#define CHANNEL_LIMIT 24

	struct AudioReader_s
	{
	#ifdef _WIN32
		char fullpath[FILENAME_MAX];
	#elif __linux__
		char fullpath[PATH_MAX];
	#endif // WIN32		
		int nStream;
		int *audioStreamIndexes;
		AVFormatContext* formatContext;
		AVCodecContext** codecContexts;
		AVCodec** codecTypes;
	};

	typedef struct AudioReader_s AudioReader;	
	const AudioReader AudioReader_Default = { {'\0'}, 0, NULL, NULL, NULL, NULL };

	// Functions
	int initAudioReaderStruct(char* filename, AudioReader* ctx);
	void deallocAudioReaderStruct(AudioReader *ctx);
	int* findAudioStreams(AVFormatContext* formatCtx, int* n);
	double** readAudioFile(AudioReader *ctx);
	// Need stream version!!!!!

	void printStreamInformation(const AVFormatContext *ctx, const AVCodec* codec, const AVCodecContext* codecCtx, int audioStreamIndex);
	char* currentDateTime();

#ifdef __cplusplus
}
#endif