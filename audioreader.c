#include "audioreader.h"

int init_audio_reader(char *filename, AVCodec *codec, AVCodecContext *codecCtx, AVFormatContext *formatCtx, int *audioStreamIndex, int *nStream)
{
	int err = 0;
	
	if (err = avformat_open_input(&formatCtx, filename, NULL, 0), err)	// Open file
		goto cleanup;
	if (err = avformat_find_stream_info(formatCtx, NULL), err)			// Get info
		goto cleanup;

	if (!audioStreamIndex)
		free(audioStreamIndex);
	audioStreamIndex = findAudioStreams(formatCtx, nStream);			// Find audio part
	if (!audioStreamIndex) {
		err = AVERROR_STREAM_NOT_FOUND;
		goto cleanup;
	}

	// vvvvvv  Need multiple stream support below  vvvvvv
	codec = avcodec_find_decoder(formatCtx->streams[audioStreamIndex]->codecpar->codec_id);
	if (!codec) {
		err = AVERROR_DECODER_NOT_FOUND;
		goto cleanup;
	}

	codecCtx = avcodec_alloc_context3(codec);							// Allocate codec context
	if (!codecCtx) {
		err = AVERROR_UNKNOWN;
		goto cleanup;
	}

	if (err = avcodec_parameters_to_context(codecCtx, formatCtx->streams[audioStreamIndex]->codecpar), err)
		goto cleanup;
	codecCtx->request_sample_fmt = av_get_alt_sample_fmt(codecCtx->sample_fmt, 0);

	if (err = avcodec_open2(codecCtx, codec, NULL), err)				// Init decoder
		goto cleanup;

cleanup:
	if (err) {
		avcodec_free_context(&codecCtx);
		avformat_close_input(&formatCtx);
	}

	return err;
}

void deinit_audio_reader(AVCodecContext *codecCtx, AVFormatContext *formatCtx)
{
	avcodec_free_context(&codecCtx);
	avformat_close_input(&formatCtx);
}

int* findAudioStreams(AVFormatContext* formatCtx, int *n)
{
	int j = 0;
	int* audioStreamIndex = (int*)av_malloc(sizeof(int) * CHANNEL_LIMIT);
	
	for (size_t i = 0; i < formatCtx->nb_streams; ++i) {
		if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			audioStreamIndex[j] = i;
			++j;
		}
	}	
	audioStreamIndex = (int*)av_realloc(audioStreamIndex, sizeof(int)*j);
	*n = j;

	return audioStreamIndex;
}
