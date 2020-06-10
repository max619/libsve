#pragma once
#include "GPUContext.h"
#include "GPUKernelSourceProvider.h"

struct GPUKernelImpl;

class GPUKernel : PImplBase<GPUKernelImpl>
{
public:
	GPUKernel(GPUContext context, const GPUKernelSourceProvider& sourceProvider);

private:
	GPUContext _context;
};