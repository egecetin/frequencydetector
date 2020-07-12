#include "process.h"

/** ##############################################################################################################
	Init window vectors from MAX_WLEN to MIN_WLEN every power of 2 for Hanning, Hamming, Rectangle, Blackman and Bartlett. 

	Notes;
		Blackman optimum alpha=-0.5/(1+cos(2pi/(len-1))

	Input;

	Output;
		retval  : 2D window vector
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
	if (status = ippsSet_64f(1, ones, MAX_WLEN), !status)
		goto cleanup;	
	for (int i = 0; i < log2(MAX_WLEN) - log2(MIN_WLEN) + 1; ++i) {
		if (status = ippsWinHann_64f(ones, arr[i * 5], MAX_WLEN / pow(2, i)), !status)				// Hanning
			goto cleanup;
		if (status = ippsWinHamming_64f(ones, arr[i * 5 + 1], MAX_WLEN / pow(2, i)), !status)		// Hamming
			goto cleanup;
		cblas_dcopy(MAX_WLEN / pow(2, i), ones, 1, arr[i * 5 + 2], 1);								// Rectangle
		if (status = ippsWinBlackmanOpt_64f(ones, arr[i * 5 + 3], MAX_WLEN / pow(2, i)), !status)	// Blackman
			goto cleanup;		
		if (status = ippsWinBartlett_64f(ones, arr[i * 5 + 4], MAX_WLEN / pow(2, i)), !status)		// Bartlett
			goto cleanup;
	}

cleanup:
	if (!status) {
		for (int i = 0; i < 5 * (log2(MAX_WLEN) - log2(MIN_WLEN) + 1); ++i) {
			ippsFree(arr[i]);
		}
		ippsFree(arr); arr = 0;
	}
	ippFree(ones);

	return arr;		
}

/** ##############################################################################################################
	Create FFT context for calculation.

	Notes;
		Multiply with 1/N at forward transform.

	Input;

	Output;
		retval	: Error status
*/
ERR_STATUS createfft(IppsFFTSpec_R_64f** fft, int order)
{
	//IPP_FFT_DIV_FWD_BY_N
	return ippStsNoErr;
}
