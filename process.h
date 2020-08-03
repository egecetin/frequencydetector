#pragma once

#include <math.h>
#include <ipp.h>
#include <mkl.h>
#include <omp.h>

//#include "logger.h"

#ifdef __cplusplus
extern "C" {
#endif
	
	/* ##################### Definitions ##################### */
	#define MAX_WLEN		32768
	#define MIN_WLEN		   32
	#define DBLIMIT			 -150

	typedef int ERR_STATUS;
	typedef enum { ippsLowPass, ippsHighPass, ippsBandPass, ippsBandStop } IppFilterType;

	extern double **fBuffer;		// Frequency data
	extern double **position;		// Pointer for pop/push
	extern double *sumBuffer;		// Summation of vectors
	extern int fBuffLen;			// Length of every element fBuffer
	extern int lagLen;				// Start lag for thresholding
	extern int currentSize;			// Current number of windows
	extern int desiredSize;			// Total number of windows
	
	extern double threshold_alpha;	// Multiplier for threshold
	extern double thresh_influence;	// Divider for detected point

	extern DFTI_DESCRIPTOR_HANDLE *fft; // FFT descriptor		
	
	/* ##################### Functions ##################### */
	
	// Initialization
	Ipp64f** init_windows(void);
	void init_globalvar(int wlen, int nwin, int lag, double thresh_alpha, double thresh_inf);
	ERR_STATUS createFFT_IPP(IppsFFTSpec_R_64f** fft, Ipp8u** buffer, int order);
	ERR_STATUS createFFT_MKL(DFTI_DESCRIPTOR_HANDLE *fft, int wlen);
	ERR_STATUS createFilter(IppsFIRSpec_64f **pSpec, IppFilterType filterType, double *rFreq, int tapsLen, IppWinType windowType, Ipp8u **pBuffer);

	// Processing
	ERR_STATUS spectrogram(double* data, int dataLen, double*** output, double* window, int wlen, int overlap, int bits);
	double* calculateFFT_MKL(double *data, double *window, int dataLen, DFTI_DESCRIPTOR_HANDLE *fft);
	double* thresholding(double *data, int dataLen, int lag, double threshold, double influence, int *n, double *th_values);
	double* estimate_freq(double *data, double *window, IppsFIRSpec_64f *pSpec, Ipp8u *pBuffer, int *size, double *th_values);

	// Helper
	ERR_STATUS seperateData(double **data, int ndata, int dataLen);

#ifdef __cplusplus
}
#endif