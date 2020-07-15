#pragma once

#include <math.h>
#include <ipp.h>
#include <mkl.h>
#include <omp.h>

#ifdef __cplusplus
extern "C" {
#endif
	
	/* ##################### Definitions ##################### */
	#define MAX_WLEN	32768
	#define MIN_WLEN	   32

	typedef int ERR_STATUS;
	typedef enum { ippsLowPass, ippsHighPass, ippsBandPass, ippsBandStop } IppFilterType;
	
	/* ##################### Functions ##################### */
	
	// Initialization
	Ipp64f** init_windows(void);
	void init_globalvar();
	ERR_STATUS createFFT_IPP(IppsFFTSpec_R_64f** fft, Ipp8u** buffer, int order);
	ERR_STATUS createFFT_MKL(DFTI_DESCRIPTOR_HANDLE *fft, int wlen);
	ERR_STATUS createFilter(IppsFIRSpec_64f** pSpec, IppFilterType filterType, double* rFreq, int tapsLen, IppWinType windowType);

	// Processing
	ERR_STATUS spectrogram(double* data, int dataLen, double** output, double* window, int wlen, int overlap, int bits);
	ERR_STATUS threshold();
	ERR_STATUS constant_freq();

#ifdef __cplusplus
}
#endif