#include "process.h"

double **fBuffer = NULL;
double **position = NULL;
double *sumBuffer = NULL;
int fBuffLen;
int lagLen;
int currentSize;		
int desiredSize;
double threshold_alpha;
double thresh_influence;

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
	Initializes all global variable
	
	Input;
		wlen		: Window length of time data (Should be even)
		nwin		: Desired number of windows
		lagLen		: Lag amount for thresholding (Choose equal to number of filtered freq pts with high pass)
		thresh_alpha: Multiplier for threshold
		thresh_inf	: Divider for detected point
	Output;
		
*/
void init_globalvar(int wlen, int nwin, int lag, double thresh_alpha, double thresh_inf)
{
	if (fBuffer) { // Free if already allocated
		for (int i = 0; i < desiredSize; ++i) {
			free(fBuffer[i]);
		}
		free(fBuffer);
		fBuffer = NULL;
	}

	// Set variables
	threshold_alpha = thresh_alpha;
	thresh_influence = thresh_inf;
	fBuffLen = wlen / 2;
	lagLen = lag;
	currentSize = 0;
	desiredSize = nwin;

	// Allocate data
	fBuffer = (double**)malloc(desiredSize * sizeof(double*));
	for (int i = 0; i < desiredSize; ++i) {
		fBuffer[i] = (double*)malloc(fBuffLen * sizeof(double));
	}

	position = fBuffer;
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
		pBuffer		: Buffer for calculations
		retval		: Error status
*/
ERR_STATUS createFilter(IppsFIRSpec_64f **pSpec, IppFilterType filterType, double *rFreq, int tapsLen, IppWinType windowType, Ipp8u *pBuffer)
{
	ERR_STATUS status = ippStsNoErr;
	
	int specSize, bufSize;
	if (pBuffer) {
		ippsFree(pBuffer);
	}		
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
	for (int i = 0; i < outLen; ++i) {
		out[i] = (double*)MKL_malloc((wlen / 2 + 1) * sizeof(double), 64);
		if (!out[i]) { // Check allocated memory
			status = DFTI_MEMORY_ERROR;
			goto cleanup;
		}
	}

	// Init FFT
	for (int i = 0; i < max_thread; ++i) {
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
			for (int j = 0; j < wlen / 2 + 1; ++j) {
				if (ptr[j] < DBLIMIT)
					ptr[j] = DBLIMIT;
			}

			// Free buffers
			MKL_free(fdata);
			MKL_free(buff);
		}		
	}

	if (max_thread == 1)	// All PCs have more than 1 thread if it is 1 there is a problem (Assume as warning)
		status = DFTI_MULTITHREADED_ERROR;

cleanup:
	MKL_Set_Num_Threads_Local(nth); // Set thread to default
	if (status) {
		for (int i = 0; i < outLen; ++i)
			MKL_free(out[i]);
		MKL_free(out);
	}

	// Release FFT
	for (int i = 0; i < max_thread; ++i)
		status = DftiFreeDescriptor(&vfft[i]);
	MKL_free(vfft);

	return status;
}

/** ##############################################################################################################
	Threshold the data

	Input;
		data		: Input 1-D data
		dataLen		: Length of data vector
		lag			: Moving window length
		threshold	: Multiplier for thresholding
		influence	: Divider for accepted data
	Output;
		th_values	: (Optional) Threshold values. Can be NULL if not needed
		retval		: Indexes of accepted points
*/
double* thresholding(double *data, int dataLen, int lag, double threshold, double influence, int *n, double *th_values)
{
	double mean, std, mean_local, std_local;
	double *ptr = NULL;
	*n = 0;

	// Init buffers
	double *dbuff = (double*)MKL_malloc(dataLen * sizeof(double), 64);
	double *output = (double*)MKL_malloc(dataLen * sizeof(double), 64);
	if (!(dbuff && output)) // Check allocated memory
		goto cleanup;

	if (ippsMeanStdDev_64f(data, dataLen, &mean, &std)) // Global mean, std
		goto cleanup;
	if (ippsMeanStdDev_64f(data, lag, &mean_local, &std_local)) // Local mean, std
		goto cleanup;
	
	ptr = &dbuff[0];
	cblas_dcopy(dataLen, data, 1, dbuff, 1); // Copy data to buffer
	for (int i = lag + 1; i < dataLen; ++i) {
		if (th_values)
			th_values[i] = threshold * std_local + threshold * std + mean_local + mean;
		if (dbuff[i] > threshold*std_local + threshold * std + mean_local + mean) {
			output[*n] = i;
			*n += 1;
			dbuff[i] = influence * dbuff[i] + (1 - influence)*(dbuff[i - 1]);
		}
		++ptr;		
		if (ippsMeanStdDev_64f(ptr, lag, &mean_local, &std_local)) // Update local mean, std
			goto cleanup;
	}

	
cleanup:
	// Deallocation
	MKL_free(dbuff);
	MKL_realloc(output, (*n) * sizeof(double)); // Shrink output data
	if (*n == 0)
		n = NULL;

	return output;
}

/** ##############################################################################################################
	Estimate frequencies

	Input;
		data		: Input 1-D data
		pSpec		: Spec for filtering
		pBuffer		: Buffer for filtering
	Output;
		size		: Return size
		th_values	: (Optional) Threshold values. Can be NULL if not needed
		retval		: Indexes of accepted points
*/
double* estimate_freq(double *data, IppsFIRSpec_64f *pSpec, Ipp8u *pBuffer, int *size, double *th_values)
{
	if (currentSize < desiredSize) { // Initialization
		cblas_dcopy(fBuffLen, data, 1, *position, 1);								// Copy data to vector
		ippsFIRSR_64f(*position, *position, fBuffLen, pSpec, NULL, NULL, pBuffer);  // Filter data
		vdAdd(fBuffLen, sumBuffer, data, sumBuffer);								// Add to sum
		
		++currentSize;
		if (++position - fBuffer > desiredSize)
			position = fBuffer;
		
		*size = 0;
		return NULL;
	}
	else { // If buffer is full
		double *out = thresholding(sumBuffer, fBuffLen, lagLen, threshold_alpha, thresh_influence, size, th_values);

		vdSub(fBuffLen, sumBuffer, *position, sumBuffer);							// Subtract old data from sum
		cblas_dcopy(fBuffLen, data, 1, *position, 1);								// Overwrite it
		ippsFIRSR_64f(*position, *position, fBuffLen, pSpec, NULL, NULL, pBuffer);	// Filter new data
		vdAdd(fBuffLen, sumBuffer, *position, sumBuffer);							// Add to sum
		if (++position - fBuffer > desiredSize)
			position = fBuffer;	
			
		return out;
	}
}

