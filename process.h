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

	extern double **fBuffer;		// All frequency data
	extern double **position;
	extern double *sumBuffer;		// Sum of data	
	extern int fBuffLen;			
	extern int lagLen;
	extern int currentSize;			
	extern int desiredSize;	
		
	
	/* ##################### Functions ##################### */
	
	// Initialization
	Ipp64f** init_windows(void);
	void init_globalvar();
	ERR_STATUS createFFT_IPP(IppsFFTSpec_R_64f** fft, Ipp8u** buffer, int order);
	ERR_STATUS createFFT_MKL(DFTI_DESCRIPTOR_HANDLE *fft, int wlen);
	ERR_STATUS createFilter(IppsFIRSpec_64f** pSpec, IppFilterType filterType, double* rFreq, int tapsLen, IppWinType windowType);

	// Processing
	ERR_STATUS spectrogram(double* data, int dataLen, double** output, double* window, int wlen, int overlap, int bits);
	double* thresholding(double *data, int dataLen, int lag, double threshold, double influence, int *n);
	double* estimate_freq(double *data, int* dataLen);

#ifdef __cplusplus
}
#endif