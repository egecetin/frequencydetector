#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>

	// Definitions
	#define CHANNEL_LIMIT 24

	struct AudioReader
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

	// Functions
	int initAudioReaderStruct(char* filename, struct AudioReader* ctx);
	void deallocAudioReaderStruct(struct AudioReader* ctx);
	void deinit_audio_reader(AVCodecContext* codecCtx, AVFormatContext *formatCtx);
	int* findAudioStreams(AVFormatContext* formatCtx, int* n);

#ifdef __cplusplus
}
#endif