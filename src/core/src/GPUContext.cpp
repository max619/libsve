#include "GPUContext.h"
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
};

struct GPUContextImpl
{
	std::shared_ptr<GPUContextImplBag> ptr;
	GPUContextImplBag& operator->() const
	{
		return *ptr;
	}
};

bool IsCLErr(cl_int err)
{
	return err != 0;
}

void InitContext(GPUContextImpl* impl, cl::Platform& platform, int deviceId, cl_device_type type)
{
	impl->ptr->platform = platform;

	std::vector<cl::Device> devices;
	platform.getDevices(type, &devices);
	if (deviceId >= devices.size())
		throw std::exception("Failed to select device");
	
	impl->ptr->device = devices[deviceId];
	impl->ptr->context = cl::Context(impl->ptr->device);
	impl->ptr->commandQueue = cl::CommandQueue(impl->ptr->context);
}

GPUContext::GPUContext() : GPUContext(0)
{
}

GPUContext::GPUContext(int deviceId)
{
	cl_int err = 0;
	auto platform = cl::Platform::getDefault(&err);


	impl = new GPUContextImpl();
	try
	{
		InitContext(impl, platform, deviceId);
	}
	catch (...)
	{
		delete impl;
		throw;
	}
}

GPUContext::GPUContext(AccelerationDeviceType type, int deviceId)
{
	cl_int err = 0;
	std::vector<cl::Platform> platforms;
	err = cl::Platform::get(&platforms);

	impl = new GPUContextImpl();
	try
	{
		//TODO: Add selection of platforms
		InitContext(impl, platforms[0], 0);
	}
	catch (...)
	{
		delete impl;
		throw;
	}
}

GPUContext::GPUContext(const GPUContext& lhs)
{
	impl = new GPUContextImpl(*lhs.impl);
}

GPUContext::GPUContext(GPUContext&& lhs)
{
	impl = lhs.impl;
	lhs.impl = nullptr;
}


GPUContext::~GPUContext()
{
	if (impl != nullptr)
		delete impl;
}

GPUMemoryBuffer GPUContext::AllocateBuffer(size_t size, GPUMemoryFlags flags)
{
	cl_mem_flags clFlags = static_cast<cl_mem_flags>(flags) | CL_MEM_ALLOC_HOST_PTR;
	cl_int err = 0;
	GPUMemoryBufferImpl* impl = new GPUMemoryBufferImpl();
	if (impl == nullptr)
		throw std::exception("Out of memory");
	try
	{
		impl->flags = clFlags;
		impl->size = size;
		impl->locked = false;
		impl->buffer = cl::Buffer(this->impl->ptr->context, clFlags, size, nullptr, &err);
		impl->pinnedData = nullptr;

		if (IsCLErr(err))
			throw std::exception("Failed to allocate buffer");
		return GPUMemoryBuffer(*this, impl);
	}
	catch (...)
	{
		delete impl;
		throw;
	}
}

GPUContext& GPUContext::operator=(const GPUContext& lhs)
{
	if (this == &lhs)
		return *this;

	if (impl != nullptr)
		delete impl;

	impl = new GPUContextImpl(*lhs.impl);
	return *this;
}

GPUContext& GPUContext::operator=(GPUContext&& lhs)
{
	if (this == &lhs)
		return *this;

	if (impl != nullptr)
		delete impl;

	impl = lhs.impl;
	lhs.impl = nullptr;
	return *this;
}

GPUMemoryBuffer::GPUMemoryBuffer(GPUContext context, GPUMemoryBufferImpl* impl) :
	PImplBase<GPUMemoryBufferImpl>(impl),
	_context(context)
{
}

GPUMemoryBuffer::GPUMemoryBuffer(const GPUMemoryBuffer& lhs) :
	PImplBase<GPUMemoryBufferImpl>(lhs),
	_context(lhs._context)
{
}

GPUMemoryBuffer::GPUMemoryBuffer(GPUMemoryBuffer&& lhs) :
	PImplBase<GPUMemoryBufferImpl>(lhs),
	_context(std::move(lhs._context))
{
}

void* GPUMemoryBuffer::LockData(bool isWrite)
{
	if (_impl->locked)
	{
		if (_impl->pinnedData == nullptr)
			throw std::exception("Buffer is locked bit pinned data is null");

		return _impl->pinnedData;
	}
}

void GPUMemoryBuffer::UnlockData()
{
}

GPUMemoryBuffer& GPUMemoryBuffer::operator=(const GPUMemoryBuffer& lhs)
{
	if (this == &lhs)
		return *this;

	_context = lhs._context;
	
	PImplBase<GPUMemoryBufferImpl>::operator=(lhs);

	return *this;
}

GPUMemoryBuffer& GPUMemoryBuffer::operator=(GPUMemoryBuffer&& lhs)
{
	if (this == &lhs)
		return *this;

	_context = std::move(lhs._context);

	PImplBase<GPUMemoryBufferImpl>::operator=(lhs);

	return *this;
}

