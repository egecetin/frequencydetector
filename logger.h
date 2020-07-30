#pragma once

#include <mkl.h>
#include <ipp.h>
#include <libavutil/error.h>

#include <ctime>
#include <iostream>
#include <fstream>

typedef int ERR_STATUS;

/* Error Classifiers*/
#define DFTI_ID		1
#define VML_ID		2
#define VSLCC_ID	3
#define IPP_ID		4
#define FFMPEG_ID	5

namespace ege {

	/* Functions */
	inline const char* currentDateTime();
	inline const char* VMLErrorMessage(ERR_STATUS code);
	inline const char* VSLCCErrorMessage(ERR_STATUS code);
	inline const char* sterror(ERR_STATUS code, int id);
	void logger(ERR_STATUS code, int id);

}