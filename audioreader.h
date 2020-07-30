#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>

	#define CHANNEL_LIMIT 24

	int init_audio_reader(char *filename, AVCodec **codec, AVCodecContext **codecCtx, AVFormatContext **formatCtx, int **audioStreamIndex, int *nStream);
	void deinit_audio_reader(AVCodecContext* codecCtx, AVFormatContext *formatCtx);
	int* findAudioStreams(AVFormatContext* formatCtx, int* n);


#ifdef __cplusplus
}
#endif