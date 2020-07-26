#pragma once

#include <math.h>
#include <ipp.h>
#include <mkl.h>
#include <omp.h>

#ifdef __cplusplus
extern "C" {
#endif
	
	/* ##################### Definitions ##################### */
	#define MAX_WLEN		32768
	#define MIN_WLEN		   32
	#define THRESHOLD_ALPHA	  3.5
	#define THRESH_INFLUENCE  0.5

	typedef int ERR_STATUS;
	typedef enum { ippsLowPass, ippsHighPass, ippsBandPass, ippsBandStop } IppFilterType;

	extern double **fBuffer;		// Frequency data
	extern double **position;		// Pointer for pop/push
	extern double *sumBuffer;		// Summation of vectors
	extern int fBuffLen;			// Length of every element fBuffer
	extern int lagLen;				// Start lag for thresholding
	extern int currentSize;			// Current number of windows
	extern int desiredSize;			// Total number of windows
		
	
	/* ##################### Functions ##################### */
	
	// Initialization
	Ipp64f** init_windows(void);
	void init_globalvar(int wlen, int nwin, int lag);
	ERR_STATUS createFFT_IPP(IppsFFTSpec_R_64f** fft, Ipp8u** buffer, int order);
	ERR_STATUS createFFT_MKL(DFTI_DESCRIPTOR_HANDLE *fft, int wlen);
	ERR_STATUS createFilter(IppsFIRSpec_64f **pSpec, IppFilterType filterType, double *rFreq, int tapsLen, IppWinType windowType, Ipp8u *pBuffer);

	// Processing
	ERR_STATUS spectrogram(double* data, int dataLen, double** output, double* window, int wlen, int overlap, int bits);
	double* thresholding(double *data, int dataLen, int lag, double threshold, double influence, int *n, double *th_values);
	double* estimate_freq(double *data, IppsFIRSpec_64f *pSpec, Ipp8u *pBuffer, int *size, double *th_values);

#ifdef __cplusplus
}
#endif