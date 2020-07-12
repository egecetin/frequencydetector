#pragma once

#include <math.h>
#include <ipp.h>
#include <mkl.h>

#define MAX_WLEN	32768
#define MIN_WLEN	   32

typedef int ERR_STATUS;

#ifdef __cplusplus
extern "C" {
#endif
	
	// Initialization
	Ipp64f** init_windows(void);
	void init_globalvar();
	ERR_STATUS createfft();

	// Processing	
	ERR_STATUS calcfft();
	ERR_STATUS HPfilter(double* data, int len, double rFreq, int tapslen, IppWinType windowtype);
	ERR_STATUS threshold();
	ERR_STATUS constant_freq();

#ifdef __cplusplus
}
#endif