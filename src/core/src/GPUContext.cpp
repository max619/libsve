#include "GPUContext.h"
#include "PrivateGPUContextStructsImpl.h"

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

GPUContext::GPUContext(int deviceId) : PImplBase()
{
	cl_int err = 0;
	auto platform = cl::Platform::getDefault(&err);


	InitContext(_impl, platform, deviceId);
}

GPUContext::GPUContext(AccelerationDeviceType type, int deviceId) : PImplBase()
{
	cl_int err = 0;
	std::vector<cl::Platform> platforms;
	err = cl::Platform::get(&platforms);

	InitContext(_impl, platforms[0], 0);
}


GPUMemoryBuffer GPUContext::AllocateBuffer(size_t size, GPUMemoryFlags flags)
{
	return GPUMemoryBuffer(*this, size, flags);
}

GPUCommandQueue GPUContext::CreateCommandQueue(GPUCommandQueueProperties props)
{
	return GPUCommandQueue(*this, props);
}

GPUMemoryBuffer::GPUMemoryBuffer(GPUContext context, size_t size, GPUMemoryFlags flags) : PImplBase()
{
	cl_mem_flags clFlags = static_cast<cl_mem_flags>(flags) | CL_MEM_ALLOC_HOST_PTR;
	cl_int err = 0;
	_impl->flags = clFlags;
	_impl->size = size;
	_impl->locked = false;
	_impl->buffer = cl::Buffer(context.GetImpl()->ptr->context, clFlags, size, nullptr, &err);
	_impl->pinnedData = nullptr;

	if (IsCLErr(err))
		throw std::exception("Failed to allocate buffer");
}

GPUMemoryBuffer::GPUMemoryBuffer(GPUContext context, size_t size, GPUMemoryFlags flags, void* data) : PImplBase()
{
	cl_mem_flags clFlags = static_cast<cl_mem_flags>(flags);
	if (clFlags & CL_MEM_HOST_NO_ACCESS)
		clFlags |= CL_MEM_COPY_HOST_PTR;
	else
		clFlags |= CL_MEM_USE_HOST_PTR;

	cl_int err = 0;
	_impl->flags = clFlags;
	_impl->size = size;
	_impl->locked = false;
	_impl->buffer = cl::Buffer(context.GetImpl()->ptr->context, clFlags, size, data, &err);
	_impl->pinnedData = nullptr;

	if (IsCLErr(err))
		throw std::exception("Failed to allocate buffer");
}

void* GPUMemoryBuffer::LockData(bool isWrite)
{
	if (_impl->flags & CL_MEM_HOST_NO_ACCESS)
		throw std::exception("No access to data on host");

	if (isWrite && _impl->flags & CL_MEM_HOST_READ_ONLY)
		throw std::exception("Buffer has only read access");

	if (!isWrite && _impl->flags & CL_MEM_HOST_WRITE_ONLY)
		throw std::exception("Buffer has only write access");

	if (_impl->locked)
	{
		if (_impl->pinnedData == nullptr)
			throw std::exception("Buffer is locked but pinned data is null");

		return _impl->pinnedData;
	}

	cl_int err;
	_impl->pinnedData = _context.GetImpl()->ptr->commandQueue.enqueueMapBuffer(_impl->buffer, true, isWrite ? CL_MAP_WRITE : CL_MAP_READ, 0, _impl->size, nullptr, nullptr, &err);
	if (IsCLErr(err))
		throw std::exception("Failed to map buffer");

	return _impl->pinnedData;
}

void GPUMemoryBuffer::UnlockData()
{
}

void GPUMemoryBuffer::Resize(size_t size)
{
}

size_t GPUMemoryBuffer::GetDataSize() const
{
	return _impl->size;
}

void* GPUCommandQueue::EnqueueMapBuffer(GPUMemoryBuffer& buffer, bool isRead, bool blocking)
{
	return EnqueueMapBuffer(buffer, isRead, blocking, 0, buffer.GetDataSize());
}

void* GPUCommandQueue::EnqueueMapBuffer(GPUMemoryBuffer& buffer, bool isRead, bool blocking, size_t offset, size_t size)
{
	cl_int err;
	void* h_Data = _impl->commandQueue.enqueueMapBuffer(buffer.GetImpl()->buffer, blocking, isRead ? CL_MAP_READ : CL_MAP_WRITE, offset, size, nullptr, nullptr, &err);
	if (IsCLErr(err))
		throw std::exception("Failed to map buffer");
	buffer.GetImpl()->pinnedData = h_Data;
	return h_Data;
}

void GPUCommandQueue::EnqueueUnmapMemory(GPUMemoryBuffer& buffer, bool isRead, bool blocking)
{
	cl_int err;
	if (buffer.GetImpl()->pinnedData == nullptr)
		throw std::exception("Buffer is not mapped");

	err = _impl->commandQueue.enqueueUnmapMemObject(buffer.GetImpl()->buffer, buffer.GetImpl()->pinnedData, nullptr, nullptr);
	if (IsCLErr(err))
		throw std::exception("Failed to unmap mem object");
}

GPUCommandQueue::GPUCommandQueue(GPUContext context, GPUCommandQueueProperties props) :
	PImplBase<GPUCommandQueueImpl>(), _context(context)
{
	cl_command_queue_properties clProps = static_cast<cl_command_queue_properties>(props);
	_impl->props = clProps;
	cl_int err = 0;
	_impl->commandQueue = cl::CommandQueue(context.GetImpl()->ptr->context, clProps, &err);
	if (IsCLErr(err))
		throw std::exception("Failed to create Command Queue");
}
