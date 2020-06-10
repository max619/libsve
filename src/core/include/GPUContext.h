#pragma once
#include "PImpl.h"
#include <memory>

typedef void* GPUMemoryHandle;
struct GPUContextImpl;
struct GPUMemoryBufferImpl;
struct GPUCommandQueueImpl;
class GPUMemoryBuffer;
class GPUCommandQueue;

enum class AccelerationDeviceType
{
	NONE,
	CPU,
	GPU
};

enum class GPUMemoryFlags : uint64_t
{
	MEM_READ_WRITE = (1 << 0),
	MEM_WRITE_ONLY = (1 << 1),
	MEM_READ_ONLY = (1 << 2),
	//MEM_USE_HOST_PTR = (1 << 3),
	//MEM_ALLOC_HOST_PTR = (1 << 4),
	//MEM_COPY_HOST_PTR = (1 << 5),
	MEM_HOST_WRITE_ONLY = (1 << 7),
	MEM_HOST_READ_ONLY = (1 << 8),
	MEM_HOST_NO_ACCESS = (1 << 9),
};


enum class GPUCommandQueueProperties : uint64_t
{
	QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE = (1 << 0),
	QUEUE_PROFILING_ENABLE = (1 << 1)
};


class GPUContext : public  PImplBase<GPUContextImpl>
{
public:
	GPUContext();
	GPUContext(int deviceId);
	GPUContext(AccelerationDeviceType type, int deviceId);
	GPUMemoryBuffer AllocateBuffer(size_t size, GPUMemoryFlags flags);

	GPUCommandQueue CreateCommandQueue(GPUCommandQueueProperties props);
};


class GPUMemoryBuffer : public PImplBase<GPUMemoryBufferImpl>
{
public:
	GPUMemoryBuffer(GPUContext context, size_t size, GPUMemoryFlags flags);
	GPUMemoryBuffer(GPUContext context, size_t size, GPUMemoryFlags flags, void* data);
	void* LockData(bool isWrite);
	void UnlockData();
	void Resize(size_t size);
	size_t GetDataSize() const;
private:

	GPUContext _context;
};

class GPUCommandQueue : public PImplBase<GPUCommandQueueImpl>
{
public:
	GPUCommandQueue(GPUContext context, GPUCommandQueueProperties props = (GPUCommandQueueProperties)0);
	void* EnqueueMapBuffer(GPUMemoryBuffer& buffer, bool isRead, bool blocking);
	void* EnqueueMapBuffer(GPUMemoryBuffer& buffer, bool isRead, bool blocking, size_t offset, size_t size);
	void EnqueueUnmapMemory(GPUMemoryBuffer& buffer, bool isRead, bool blocking);

private:

	GPUContext _context;
};
