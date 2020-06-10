#pragma once
#include "PImpl.h"
#include <memory>

class GPUContext;
typedef void* GPUMemoryHandle;
struct GPUContextImpl;
struct GPUMemoryBufferImpl;
struct GPUCommandQueueImpl;

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


class GPUMemoryBuffer : PImplBase<GPUMemoryBufferImpl>
{
public:
	using PImplBase<GPUMemoryBufferImpl>::PImplBase;
	using PImplBase<GPUMemoryBufferImpl>::operator=;

	GPUMemoryBuffer(const GPUMemoryBuffer& lhs) = default;
	GPUMemoryBuffer(GPUMemoryBuffer&& lhs) = default;

	void* LockData(bool isWrite);
	void UnlockData();

	GPUMemoryBuffer& operator=(const GPUMemoryBuffer& lhs) = default;
	GPUMemoryBuffer& operator=(GPUMemoryBuffer&& lhs) = default;
private:

	GPUMemoryBuffer(GPUContext context, GPUMemoryBufferImpl* impl);
	GPUContext _context;

	friend class GPUContext;
};

class GPUCommandQueue
{
public:
	void* EnqueueMapBuffer(GPUMemoryBuffer& buffer, bool isRead);
	void* EnqueueUnmapBuffer(GPUMemoryBuffer& buffer, bool isRead);

private:
	GPUCommandQueue(GPUContext context, GPUCommandQueueImpl* impl);

	GPUContext _context;
	GPUCommandQueueImpl* _impl;
};

class GPUContext
{
public:
	GPUContext();
	GPUContext(int deviceId);
	GPUContext(AccelerationDeviceType type, int deviceId);
	GPUContext(const GPUContext& lhs);
	GPUContext(GPUContext&& lhs);
	~GPUContext();
	GPUMemoryBuffer AllocateBuffer(size_t size, GPUMemoryFlags flags);

	GPUContext& operator=(const GPUContext& lhs);
	GPUContext& operator=(GPUContext&& lhs);
private:
	GPUContextImpl* impl = nullptr;

	friend class GPUMemoryBuffer;
};