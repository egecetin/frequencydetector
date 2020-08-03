#pragma once

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>

	// Definitions
	#define STREAM_LIMIT			 24
	#define EXTEND_BUFFER_IN_SEC	  1
	
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
		double **channelData;
	};	
	typedef struct StreamData_s StreamData;

	struct AudioData_s
	{
		int nStream;
		StreamData *data;
	};
	typedef struct AudioData_s AudioData;
	const AudioData AudioData_Default = { 0, NULL };

	// Allocations
	int initAudioReaderStruct(char* filename, AudioReader* ctx);
	int allocateAudioData(AudioData *audio, AudioReader ctx);

	// Functions	
	int* findAudioStreams(AVFormatContext* formatCtx, int* n);
	int readAudioFile(AudioReader *ctx, AudioData *data);
	double** getDataPointers(AudioData *data, int *len);

	// Deallocations
	void deinitAudioReaderStruct(AudioReader *ctx);
	void deallocAudioData(AudioData *audio);

	// Debug functions
	void printStreamInformation(const AVFormatContext *ctx, const AVCodec* codec, const AVCodecContext* codecCtx, int audioStreamIndex);
	char* currentDateTime();

#ifdef __cplusplus
}
#endif