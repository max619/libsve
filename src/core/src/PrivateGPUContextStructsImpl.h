#pragma once
#include <CL\cl.hpp>

struct GPUContextImplBag
{
	cl::Platform platform;
	cl::Device device;
	cl::Context context;
	cl::CommandQueue commandQueue;
};

struct GPUMemoryBufferImpl
{
	cl::Buffer buffer;
	size_t size;
	bool locked;
	void* pinnedData;
	cl_mem_flags flags;
};

struct GPUCommandQueueImpl
{
	cl::CommandQueue commandQueue;
	cl_command_queue_properties props;
};

struct GPUContextImpl
{
	std::shared_ptr<GPUContextImplBag> ptr;
};

struct GPUKernelImpl
{
	cl::Kernel kernel;
};