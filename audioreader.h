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

	struct StreamData_s
	{
		int nChannel;
		int dataLen;
		int64_t **channelData;
	};	
	typedef struct StreamData_s StreamData;

	struct AudioData_s
	{
		int nStream;		
		StreamData *data;
	};
	typedef struct AudioData_s AudioData;

	// Functions
	int initAudioReaderStruct(char* filename, AudioReader* ctx);
	void deinitAudioReaderStruct(AudioReader *ctx);
	int* findAudioStreams(AVFormatContext* formatCtx, int* n);
	int readAudioFile(AudioReader *ctx, AudioData *data);
	int allocateAudioData(AudioData *audio, AudioReader ctx);
	void deallocAudioData(AudioData *audio);	

	// Debug functions
	void printStreamInformation(const AVFormatContext *ctx, const AVCodec* codec, const AVCodecContext* codecCtx, int audioStreamIndex);
	char* currentDateTime();

#ifdef __cplusplus
}
#endif