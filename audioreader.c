#include "audioreader.h"

/** ##############################################################################################################
	Initializes the context for audio read

	Input;
		filename: Full path to file
	Output;
		ctx		: Struct for audio reading
		retval	: Error value. Zero if no error, negative if AudioReader struct invalid, positive if cant decode some streams
*/
int initAudioReaderStruct(char *filename, struct AudioReader *ctx)
{
	int err = 0, buff = 0;
	
#ifdef _WIN32
	strcpy_s(ctx->fullpath, FILENAME_MAX, filename);
#elif __linux__
	strcpy(ctx->fullpath, filename);
#endif // WIN32

	if (err = avformat_open_input(&(ctx->formatContext), filename, NULL, 0), err)	// Open file
		goto cleanup;
	if (err = avformat_find_stream_info(ctx->formatContext, NULL), err)				// Get info
		goto cleanup;

	ctx->audioStreamIndexes = findAudioStreams(ctx->formatContext, &(ctx->nStream));// Find audio streams
	if (!(ctx->audioStreamIndexes)) {
		err = AVERROR_STREAM_NOT_FOUND;
		goto cleanup;
	}

	// Init struct
	ctx->codecContexts = (AVCodecContext**)av_malloc(ctx->nStream * sizeof(AVCodecContext*));
	ctx->codecTypes = (AVCodec**)av_malloc(ctx->nStream * sizeof(AVCodec*));

	for (int i = 0; i < ctx->nStream; ++i) {
		// Find decoder
		ctx->codecTypes[i] = avcodec_find_decoder(ctx->formatContext->streams[ctx->audioStreamIndexes[i]]->codecpar->codec_id);
		if (!ctx->codecTypes[i]) {
			ctx->audioStreamIndexes[i] = AVERROR_DECODER_NOT_FOUND;
			continue;
		}

		// Allocate context
		ctx->codecContexts[i] = avcodec_alloc_context3(ctx->codecTypes[i]);
		if (!ctx->codecTypes[i]) {
			ctx->audioStreamIndexes[i] = AVERROR_UNKNOWN;
			continue;
		}

		// Init context parameters
		buff = avcodec_parameters_to_context(ctx->codecContexts[i], ctx->formatContext->streams[ctx->audioStreamIndexes[i]]->codecpar);
		if (buff) {
			ctx->audioStreamIndexes[i] = buff;
			continue;
		}

		ctx->codecContexts[i]->request_sample_fmt = av_get_alt_sample_fmt(ctx->codecContexts[i]->sample_fmt, 0);

		// Init decoder
		buff = avcodec_open2(ctx->codecContexts[i], ctx->codecTypes[i], NULL);	
		if (buff) {
			ctx->audioStreamIndexes[i] = buff;
		}
	}

	// Clean struct
	buff = 0;
	for (int i = 0; i < ctx->nStream; ++i) {
		if (ctx->audioStreamIndexes[i] < 0) {
			avcodec_free_context(&(ctx->codecContexts[i]));
			av_free(ctx->codecTypes[i]); ctx->codecTypes[i] = NULL;
			for (int j = i; j < ctx->nStream - 1; ++j) {
				ctx->codecContexts[j] = ctx->codecContexts[j + 1];
				ctx->codecTypes[j] = ctx->codecTypes[j + 1];
				ctx->audioStreamIndexes[j] = ctx->audioStreamIndexes[j + 1];
			}

			--ctx->nStream;
			++buff;
		}
	}

	ctx->codecContexts = (AVCodecContext**)av_realloc(ctx->codecContexts, ctx->nStream * sizeof(AVCodecContext*));
	ctx->codecTypes = (AVCodec**)av_realloc(ctx->codecTypes, ctx->nStream * sizeof(AVCodec*));
	ctx->audioStreamIndexes = (int*)av_realloc(ctx->audioStreamIndexes, ctx->nStream * sizeof(int));

cleanup:

	if (err)
		deallocAudioReaderStruct(ctx);
	else
		err = buff;

	return err;
}

/** ##############################################################################################################
	Deinitializes the context for audio read

	Input;
		ctx: Struct for audio reading
	Output;
*/
void deallocAudioReaderStruct(struct AudioReader *ctx)
{
	#ifdef _WIN32
	for (int i = 0; i < FILENAME_MAX; ++i) {
	#elif __linux__
	for (int i = 0; i < PATH_MAX; ++i) {
	#endif // WIN32
		ctx->fullpath[i] = '\0';
	}

	for (int i = 0; i < ctx->nStream; ++i) {
		avcodec_free_context(&(ctx->codecContexts[i]));		
		av_free(ctx->codecTypes[i]); ctx->codecTypes[i] = NULL;
	}

	av_free(ctx->audioStreamIndexes); ctx->audioStreamIndexes = NULL;
	av_free(ctx->codecContexts); ctx->codecContexts = NULL;
	av_free(ctx->codecTypes); ctx->codecTypes = NULL;

	avformat_close_input(&(ctx->formatContext));
}

/** ##############################################################################################################
	Find the audio stream indexes. Maximum 24 channel is allowed

	Input;
		formatCtx	: Opened AVFormatContext
	Output;
		n			: Length of retval
		retval		: Indexes of audio streams
*/
int* findAudioStreams(AVFormatContext* formatCtx, int *n)
{
	int j = 0;
	int* audioStreamIndex = (int*)av_malloc(sizeof(int) * CHANNEL_LIMIT);
	
	for (size_t i = 0; i < formatCtx->nb_streams; ++i) {
		if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			audioStreamIndex[j] = i;
			++j;
		}
		if (j >= CHANNEL_LIMIT)
			break;
	}	
	audioStreamIndex = (int*)av_realloc(audioStreamIndex, sizeof(int)*j);	
	if (!j)
		audioStreamIndex = NULL;
	*n = j;

	return audioStreamIndex;
}
