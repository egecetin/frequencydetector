#include "audioreader.h"

/** ##############################################################################################################
	Initializes the context for audio read

	Input;
		filename: Full path to file
	Output;
		ctx		: Struct for audio reading
		retval	: Error value. Zero if no error, negative if AudioReader struct invalid, positive if cant decode some streams
*/
int initAudioReaderStruct(char *filename, AudioReader *ctx)
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

#ifdef _DEBUG
	for (int i = 0; i < ctx->nStream; ++i)
		printStreamInformation(ctx->formatContext, ctx->codecTypes[i], ctx->codecContexts[i], ctx->audioStreamIndexes[i]);
#endif // _DEBUG

cleanup:
	if (err)
		deinitAudioReaderStruct(ctx);
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
void deinitAudioReaderStruct(AudioReader * ctx)
{
	for (int i = 0; i < ctx->nStream; ++i) {
		avcodec_free_context(&(ctx->codecContexts[i]));
		av_free(ctx->codecTypes[i]); ctx->codecTypes[i] = NULL;
	}

	av_free(ctx->audioStreamIndexes); ctx->audioStreamIndexes = NULL;
	av_free(ctx->codecContexts); ctx->codecContexts = NULL;
	av_free(ctx->codecTypes); ctx->codecTypes = NULL;

	avformat_close_input(&(ctx->formatContext));

	#ifdef _WIN32
	for (int i = 0; i < FILENAME_MAX; ++i)
	#elif __linux__
	for (int i = 0; i < PATH_MAX; ++i)
	#endif
	{
		ctx->fullpath[i] = '\0';
	}
}

/** ##############################################################################################################
	Find the audio stream indexes. Maximum 24 different stream is allowed

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

/** ##############################################################################################################
	Read whole audio file

	Input;
		ctx		: Context of opened file
	Output;
		data	: Output data
		retval	: Error value
*/
int readAudioFile(AudioReader *ctx, AudioData *data)
{
	int err = 0;
	int *currPos = NULL;
	AVFrame* frame = NULL;
	AVPacket packet;

	// Init variables
	av_init_packet(&packet);
	frame = av_frame_alloc();
	// <-------------------- Allocate currPos
	if (!(frame && currPos)) {
		err = AVERROR_UNKNOWN;
		goto cleanup;
	}
	deallocAudioData(data); // Clear previous contents
	if (err = allocateAudioData(data, *ctx), err)
		goto cleanup;
	
	while (!(err = av_read_frame(ctx->formatContext, &packet))) {

		err = -1;
		for (int i = 0; i < ctx->nStream; ++i) {

			// Find corresponding index
			if (ctx->audioStreamIndexes[i] == packet.stream_index) {

				err = avcodec_send_packet(ctx->codecContexts[i], &packet); // Send to decode				
				// Receive the packet
				while (!(err = avcodec_receive_frame(ctx->codecContexts[i], frame))) {

					// Planar format
					if (av_sample_fmt_is_planar(ctx->codecContexts[i]->sample_fmt)) {
						for (int j = 0; j < ctx->codecContexts[i]->channels; ++j) {
							frame->extended_data[j];
							frame->nb_samples;
							
						}

					}
					// Packed format
					else {

					}


					// Free frame.
					av_frame_unref(frame);
				}
				if (err == AVERROR(EAGAIN))	// If eagain successfully received all packets
					err = 0;
				break;
			}
		}
		av_packet_unref(&packet);
	}

cleanup:
	av_frame_free(&frame);

	if (err) {
		deallocAudioData(data);
	}

	return err;
}

/** ##############################################################################################################
	Allocates AudioData structure and its subcontents

	Input;
	Output;
*/
int allocateAudioData(AudioData *audio, AudioReader ctx)
{
	audio->nStream = ctx.nStream;
	audio->data = (StreamData*)av_malloc(ctx.nStream * sizeof(StreamData*));
	if (!audio->data)
		return AVERROR_UNKNOWN;
	
	for (int i = 0; i < ctx.nStream; ++i) {
		audio->data[i].dataLen = (ctx.formatContext->duration / AV_TIME_BASE)*ctx.codecContexts[i]->sample_rate;
		audio->data[i].nChannel = ctx.codecContexts[i]->channels;
		audio->data[i].channelData = (int64_t**)av_malloc(audio->data[i].nChannel * sizeof(int64_t*));
		if (!audio->data[i].channelData)
			return AVERROR_UNKNOWN;

		for (int j = 0; j < audio->data[i].nChannel; ++j) {
			audio->data[i].channelData[j] = (int64_t*)av_malloc(audio->data[i].dataLen * sizeof(int64_t));
			if (!audio->data[i].channelData[j])
				return AVERROR_UNKNOWN;
		}
	}

	return 0;
}

/** ##############################################################################################################
	Deallocates AudioData structure and its subcontents

	Input;
	Output;
*/
void deallocAudioData(AudioData *audio)
{
	for (int i = 0; i < audio->nStream; ++i) {
		for (int j = 0; j < audio->data[i].nChannel; ++j) {
			av_free(audio->data[i].channelData[j]);
			audio->data[i].channelData[j] = NULL;
		}
		audio->data[i].nChannel = 0;
		audio->data[i].dataLen = 0;
	}
	av_free(audio->data->channelData);
	audio->data->channelData = NULL;
	av_free(audio->data);
	audio->data = NULL;
	audio->nStream = 0;
}

/** ##############################################################################################################
	Print information to file

	Input;
	Output;
*/
void printStreamInformation(const AVFormatContext *ctx, const AVCodec *codec, const AVCodecContext *codecCtx, int audioStreamIndex)
{
#ifdef _WIN32
	FILE *fptr = NULL;
	fopen_s(&fptr, "log.txt", "a+");
#elif __linux__
	FILE *fptr = fopen("log.txt", "w+");
#endif	
	
	char *time = currentDateTime();
	fprintf(fptr, "%s\n", time);
	free(time);
	
	fprintf(fptr, "Duration: %7f\n sec", (double)ctx->duration / AV_TIME_BASE);
	fprintf(fptr, "Codec: %s\n", codec->long_name);
	if (codec->sample_fmts != NULL) {
		fprintf(fptr, "Supported sample formats: ");
		for (int i = 0; codec->sample_fmts[i] != -1; ++i) {
			fprintf(fptr, "%s", av_get_sample_fmt_name(codec->sample_fmts[i]));
			if (codec->sample_fmts[i + 1] != -1) {
				fprintf(fptr, ", ");
			}
		}
		fprintf(fptr, "\n");
	}
	fprintf(fptr, "---------\n");
	fprintf(fptr, "Stream ID:     %7d\n", audioStreamIndex);
	fprintf(fptr, "Sample Format: %7s\n", av_get_sample_fmt_name(codecCtx->sample_fmt));
	fprintf(fptr, "Sample Rate:   %7d\n", codecCtx->sample_rate);
	fprintf(fptr, "Sample Size:   %7d\n", av_get_bytes_per_sample(codecCtx->sample_fmt));
	fprintf(fptr, "Bit:           %7d\n", codecCtx->bits_per_raw_sample);
	fprintf(fptr, "Channels:      %7d\n", codecCtx->channels);
	fprintf(fptr, "Planar:        %7d\n", av_sample_fmt_is_planar(codecCtx->sample_fmt));
	fprintf(fptr, "--------------------------------------------------------------\n");

	fclose(fptr);
}

char* currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char*      buf = (char*)malloc(80);
	localtime_s(&tstruct, &now);
	strftime(buf, 80, "%Y-%m-%d_%H%M%S", &tstruct);

	return buf;
}