#include "GPUKernel.h"
#include "PrivateGPUContextStructsImpl.h"

GPUKernel::GPUKernel(GPUContext context, const GPUKernelSourceProvider& sourceProvider) :
	PImplBase(), _context(context)
{
	auto sourceData = sourceProvider.GetSource();
}
