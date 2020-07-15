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
	Create FFT context for calculation (IPP Based).

	Notes;
		Multiply with 1/N at forward transform. Data length can only equal to powers of two.

	Input;
		fft		: FFT context
		buffer	: Buffer for calculations
		order	: FFT length (2^order)
	Output;
		retval	: Error status
*/
ERR_STATUS createFFT_IPP(IppsFFTSpec_R_64f** fft, Ipp8u** buffer, int order)
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
	Create FFT context for calculation (MKL Based).

	Notes;
		Multiply with 1/N at forward transform.

	Input;
		fft		: FFT context
		wlen	: FFT length
	Output;
		retval	: Error status
*/
ERR_STATUS createFFT_MKL(DFTI_DESCRIPTOR_HANDLE *fft, int wlen) {
	ERR_STATUS status = DFTI_NO_ERROR;

	status = DftiCreateDescriptor_d_1d(fft, DFTI_REAL, wlen); // Create DFT Descriptor
	if (status && !DftiErrorClass(status, DFTI_NO_ERROR))
		return status;
	status = DftiSetValue(*fft, DFTI_CONJUGATE_EVEN_STORAGE, DFTI_COMPLEX_COMPLEX); // CCE Storage type
	if (status && !DftiErrorClass(status, DFTI_NO_ERROR))
		return status;
	status = DftiSetValue(*fft, DFTI_PLACEMENT, DFTI_NOT_INPLACE); // Out-of-place (Not replace input)
	if (status && !DftiErrorClass(status, DFTI_NO_ERROR))
		return status;
	return DftiSetValue(*fft, DFTI_FORWARD_SCALE, 1.0 / wlen); // Set forward scale 1/wlen
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
		data	: Input 1-D data
		dataLen	: Length of data vector
		window	: Vector for windowing
		wlen	: Window length
		overlap	: Overlap size
	Output;
		output	: 2-D time frequency data. Every row is a time window, every col is a frequency bin
		retval	: Error status		
*/
ERR_STATUS spectrogram(double* data, int dataLen, double** output, double* window, int wlen, int overlap, int bits)
{
	ERR_STATUS status = DFTI_NO_ERROR;			// Error descriptor	

	int nth = MKL_Set_Num_Threads_Local(1);		// Set MKL thread to 1
	int max_thread = omp_get_max_threads();		// Get max thread num
	
	int shift = wlen - overlap;
	int outLen = floor((dataLen - wlen) / (shift)) + 1;
	DFTI_DESCRIPTOR_HANDLE *vfft = MKL_malloc(max_thread * sizeof(DFTI_DESCRIPTOR_HANDLE), 64);	// FFT Descriptors

	// Init output	
	double **out = (double**)MKL_malloc(outLen * sizeof(double*), 64);
	if (!(out && vfft)) { // Check allocated memory
		status = DFTI_MEMORY_ERROR;
		goto cleanup;
	}
	for (size_t i = 0; i < outLen; ++i) {
		out[i] = (double*)MKL_malloc((wlen / 2 + 1) * sizeof(double), 64);
		if (!out[i]) { // Check allocated memory
			status = DFTI_MEMORY_ERROR;
			goto cleanup;
		}
	}

	// Init FFT
	for (size_t i = 0; i < max_thread; ++i) {
		status = createFFT_MKL(&vfft[i], wlen);
		if (status && !DftiErrorClass(status, DFTI_NO_ERROR)) {
			goto cleanup;
		}
	}

	// Compute FFT
	#pragma omp parallel for
	for (int i = 0; i < outLen; ++i) {
		if (!status) {
			ERR_STATUS status_local = DFTI_NO_ERROR;
			int id = omp_get_thread_num();

			double* ptr = NULL;
			double* buff = (double*)MKL_malloc(wlen * sizeof(double), 64);
			MKL_Complex16* fdata = (MKL_Complex16*)MKL_malloc((wlen / 2 + 1) * sizeof(MKL_Complex16), 64);
			if (!(buff && fdata)) {	// Check memory
				#pragma omp critical
				status = DFTI_MEMORY_ERROR;
				continue;
			}

			vdMul(wlen, &data[i*shift], window, buff);	// Multiply with window
			if (status_local = vmlGetErrStatus(), status_local) {
				#pragma omp critical
				status = status_local;
				continue;
			}
			
			status_local = DftiCommitDescriptor(vfft[id]); // Commit DFT
			if (status_local && !DftiErrorClass(status_local, DFTI_NO_ERROR)) {
				#pragma omp critical
				status = status_local;
				continue;
			}

			status_local = DftiComputeForward(vfft[id], buff, fdata); // Compute DFT
			if (status_local && !DftiErrorClass(status_local, DFTI_NO_ERROR)) {
				#pragma omp critical
				status = status_local;
				continue;
			}

			vzAbs(wlen / 2 + 1, fdata, out[i]); // Find magnitude
			if (status_local = vmlGetErrStatus(), status_local) {
				#pragma omp critical
				status = status_local;
				continue;
			}

			cblas_dscal(wlen / 2 + 1, pow(2, -bits + 2), out[i], 1); // Normalize
			vdLog10(wlen / 2 + 1, out[i], out[i]); // Log
			if (status_local = vmlGetErrStatus(), status_local) {
				#pragma omp critical
				status = status_local;
				continue;
			}
			cblas_dscal(wlen / 2 + 1, 20, out[i], 1); // Multiply 20

			ptr = out[i];
			for (size_t j = 0; j < wlen / 2 + 1; ++j) {
				if (ptr[j] < -150)
					ptr[j] = -150;
			}

			// Free buffers
			MKL_free(fdata);
			MKL_free(buff);
		}		
	}

cleanup:
	MKL_Set_Num_Threads_Local(nth); // Set thread to default
	if (status) {
		for (size_t i = 0; i < outLen; ++i)
			MKL_free(out[i]);
		MKL_free(out);
	}

	// Release FFT
	for (size_t i = 0; i < max_thread; ++i)
		status = DftiFreeDescriptor(&vfft[i]);
	MKL_free(vfft);

	return status;
}

