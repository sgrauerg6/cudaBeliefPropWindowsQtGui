/*
 * RunBpStereoOptimizedCPU.cpp
 *
 *  Created on: Jun 17, 2019
 *      Author: scott
 */

#include "RunBpStereoOptimizedCPU.h"
#include "ProcessOptimizedCPUBP.h"

template <typename T>
RunBpStereoOptimizedCPU<T>::RunBpStereoOptimizedCPU() {
}

template <typename T>
RunBpStereoOptimizedCPU<T>::~RunBpStereoOptimizedCPU() {
}

template<typename T>
float RunBpStereoOptimizedCPU<T>::operator()(const char* refImagePath,
		const char* testImagePath, BPsettings algSettings,
		const char* saveDisparityMapImagePath, FILE* resultsFile)
{
	fprintf(resultsFile, "CURRENT RUN: OPTIMIZED CPU\n");
	int nthreads = 0;

	#pragma omp parallel
	{
		nthreads = omp_get_num_threads();
	}

	printf("Number of OMP threads: %d\n", nthreads);
	fprintf(resultsFile, "Number of OMP threads: %d\n", nthreads);
	SmoothImageCPU smoothImageCPU;
	ProcessOptimizedCPUBP<T> processImageCPU;
	return processStereoSet(refImagePath, testImagePath, algSettings,
			saveDisparityMapImagePath, resultsFile, &smoothImageCPU,
			&processImageCPU);
}

#if (CURRENT_DATA_TYPE_PROCESSING == DATA_TYPE_PROCESSING_FLOAT)

template class RunBpStereoOptimizedCPU<float>;

#elif (CURRENT_DATA_TYPE_PROCESSING == DATA_TYPE_PROCESSING_DOUBLE)

template class RunBpStereoOptimizedCPU<double>;

#elif (CURRENT_DATA_TYPE_PROCESSING == DATA_TYPE_PROCESSING_HALF)

#ifdef COMPILING_FOR_ARM
template class RunBpStereoOptimizedCPU<float16_t>;
#else
template class RunBpStereoOptimizedCPU<short>;
#endif //COMPILING_FOR_ARM

#endif
