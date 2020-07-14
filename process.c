#include "process.h"

/** ##############################################################################################################
	Init window vectors from MAX_WLEN to MIN_WLEN every power of 2 for Hanning, Hamming, Rectangle, Blackman and Bartlett. 

	Notes;
		Blackman optimum alpha=-0.5/(1+cos(2pi/(len-1))

	Input;

	Output;
		retval  : 2D window vector.
*/
Ipp64f** init_windows(void) {

	ERR_STATUS status = ippStsNoErr;
	
	// Allocate memory
	Ipp64f** arr = ippMalloc((5 * (log2(MAX_WLEN) - log2(MIN_WLEN) + 1) * sizeof(Ipp64f*)));
	Ipp64f* ones = ippsMalloc_64f(MAX_WLEN * sizeof(Ipp64f));	
	if (!(arr && ones)) {	// Check allocated memory
		status = ippStsMemAllocErr;
		goto cleanup;
	}
	for (int i = 0; i < 5 * (log2(MAX_WLEN) - log2(MIN_WLEN) + 1); ++i) {
		arr[i] = ippsMalloc_64f(MAX_WLEN / pow(2, i / 5) * sizeof(Ipp64f));
		if (!arr[i]) {		// Check allocated memory
			status = ippStsMemAllocErr;
			goto cleanup;
		}
	}
	
	// Init windows
	if (status = ippsSet_64f(1, ones, MAX_WLEN), status)
		goto cleanup;	
	for (int i = 0; i < log2(MAX_WLEN) - log2(MIN_WLEN) + 1; ++i) {
		if (status = ippsWinHann_64f(ones, arr[i * 5], MAX_WLEN / pow(2, i)), status)				// Hanning
			goto cleanup;
		if (status = ippsWinHamming_64f(ones, arr[i * 5 + 1], MAX_WLEN / pow(2, i)), status)		// Hamming
			goto cleanup;
		cblas_dcopy(MAX_WLEN / pow(2, i), ones, 1, arr[i * 5 + 2], 1);								// Rectangle
		if (status = ippsWinBlackmanOpt_64f(ones, arr[i * 5 + 3], MAX_WLEN / pow(2, i)), status)	// Blackman
			goto cleanup;		
		if (status = ippsWinBartlett_64f(ones, arr[i * 5 + 4], MAX_WLEN / pow(2, i)), status)		// Bartlett
			goto cleanup;
	}

cleanup:
	if (!status) {
		for (int i = 0; i < 5 * (log2(MAX_WLEN) - log2(MIN_WLEN) + 1); ++i) {
			ippsFree(arr[i]);
		}
		ippsFree(arr); arr = NULL;
	}
	ippFree(ones);

	return arr;		
}

/** ##############################################################################################################
	Create FFT context for calculation.

	Notes;
		Multiply with 1/N at forward transform.

	Input;
		fft		: FFT context
		buffer	: Buffer for calculations
		order	: FFT length (2^order)
	Output;
		retval	: Error status
*/
ERR_STATUS createFFT(IppsFFTSpec_R_64f** fft, Ipp8u** buffer, int order)
{
	ERR_STATUS status = ippStsNoErr;
	int buffSize = 0, specSize = 0, specbuffSize = 0;
	Ipp8u *arrSpec = NULL, *arrSpecBuff = NULL;

	// Calculate buffer sizes
	if (status = ippsFFTGetSize_R_64f(order, IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone, &specSize, &specbuffSize, &buffSize), status)
		return status;
	
	// Init memory
	arrSpec = ippsMalloc_8u(specSize);
	arrSpecBuff = ippsMalloc_8u(specbuffSize);
	*buffer = ippsMalloc_8u(buffSize);
	if (!(arrSpec && arrSpecBuff && *buffer)) {
		ippFree(arrSpec);
		ippFree(arrSpecBuff);
		ippFree(*buffer); *buffer = NULL;
		return ippStsMemAllocErr;
	}
	
	// Init context
	status = ippsFFTInit_R_64f(fft, order, IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone, arrSpec, arrSpecBuff);
	if (!status) {
		ippFree(*buffer);
		*buffer = NULL;
	}
	ippFree(arrSpec);
	ippFree(arrSpecBuff);

	return status;
}

/** ##############################################################################################################
	Create a FIR filter structure

	Input;
		pSpec		: Filter structure
		filterType	: Filter type
		rFreq		: Normalized cutoff frequency (If filter type is equal to ippsBandPass or ippsBandStop it is an two element array)
		tapslen		: Degree of filter
		windowType	: Window type for design
	Output;
		retval		: Error status
*/
ERR_STATUS createFilter(IppsFIRSpec_64f **pSpec, IppFilterType filterType, double *rFreq, int tapsLen, IppWinType windowType)
{
	ERR_STATUS status = ippStsNoErr;
	
	int specSize, bufSize;
	Ipp8u *pBuffer = NULL;
	Ipp64f *taps = NULL;

	// Generate buffers for library
	if (status = ippsFIRGenGetBufferSize(tapsLen, &bufSize), status)
		goto cleanup;

	taps = ippsMalloc_64f(tapsLen);
	pBuffer = ippsMalloc_8u(bufSize);
	if (!(taps && pBuffer)) { // Check allocated memory
		status = ippStsMemAllocErr;
		goto cleanup;
	}

	// Generate filter coeff
	switch (filterType)
	{
	case ippsLowPass:
		status = ippsFIRGenLowpass_64f(*rFreq, taps, tapsLen, windowType, ippTrue, pBuffer);
		break;
	case ippsHighPass:
		status = ippsFIRGenHighpass_64f(*rFreq, taps, tapsLen, windowType, ippTrue, pBuffer);
		break;
	case ippsBandPass:
		status = ippsFIRGenBandpass_64f(rFreq[0], rFreq[1], taps, tapsLen, windowType, ippTrue, pBuffer);
		break;
	case ippsBandStop:
		status = ippsFIRGenBandstop_64f(rFreq[0], rFreq[1], taps, tapsLen, windowType, ippTrue, pBuffer);
		break;
	default:
		status = ippStsErr;
	}	
	if (status)
		goto cleanup;

	// Generate structure
	if (status = ippsFIRSRGetSize(tapsLen, ipp64f, &specSize, &bufSize), status)
		goto cleanup;

	*pSpec = (IppsFIRSpec_64f*)ippsMalloc_8u(specSize);
	pBuffer = ippsMalloc_8u(bufSize);
	if (!(pSpec && pBuffer)) { // Check allocated memory
		status = ippStsMemAllocErr;
		goto cleanup;
	}

	status = ippsFIRSRInit_64f(taps, tapsLen, ippAlgDirect, *pSpec);

cleanup:
	ippFree(taps);
	ippFree(pBuffer);

	return status;
}

/** ##############################################################################################################
	Calculate spectrogram for offline data

	Input;

	Output;

*/
ERR_STATUS spectrogram(Ipp64f* data, int dataLen, Ipp64f** output, Ipp64f* window, int wlen, int overlap)
{
	ERR_STATUS status = ippStsNoErr;
	int shift = wlen - overlap;
	int max_thread = omp_get_max_threads();
	int len = floor((dataLen - wlen) / (shift)) + 1;
	Ipp8u** buff = ippMalloc(max_thread * sizeof(Ipp8u*));
	IppsFFTSpec_R_64f** vfft = ippMalloc(max_thread * sizeof(IppsFFTSpec_R_64f*));
	
	if (!(buff && vfft)) {
		status = ippStsMemAllocErr;
		goto cleanup;
	}

	for (int i = 0; i < max_thread; ++i) {
		if (status = createFFT(&vfft[i], &buff[i], round(log2(wlen))), status)
			goto cleanup;
	}		
	
	#pragma omp parallel for
	for (int i = 0; i < len; ++i) {
		ERR_STATUS status_local = ippStsNoErr;
		
		int id = omp_get_thread_num();		

	}

cleanup:

	return status;
}

