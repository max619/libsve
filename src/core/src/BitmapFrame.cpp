#include "BitmapFrame.h"
#include "Utils.h"
#include <exception>
#include <cassert>
#include <memory>


void BitmapFrame::copyFrom(const BitmapFrame& src, bool isStrict)
{
	if (src._width != _width || src._height != _height || src._fmt != src._fmt)
	{
		if (isStrict)
			throw std::exception("Dimensions of images does not match");
		allocate(src._width, src._height, src._fmt);
	}

	const char* srcPointer = static_cast<const char*>(src._data);
	char* dstPointer = static_cast<char*>(_data);
	size_t copyStride = PixelFormatInfo::GetBPP(_fmt) * _width / 8;
	for (int row = 0; row < _height; row++, srcPointer += src._stride, dstPointer += _stride)
		memcpy(dstPointer, srcPointer, copyStride);
}

void BitmapFrame::allocate(int width, int height, PixelFormat fmt)
{
	assert(width > 0 && height > 0);
	auto bitStride = static_cast<size_t>(PixelFormatInfo::GetBPP(fmt))* width;
	bitStride = AlignTo(bitStride, 32);
	size_t newStride = bitStride / 8;

	size_t newDataSize = newStride * height;

	void* newData = realloc(_ownMem ? _data : nullptr, newDataSize);
	if (newData == nullptr)
		throw std::exception("Failed to resize data block");
	_data = newData;

	_stride = newStride;
	_width = width;
	_height = height;
	_fmt = fmt;
	_ownMem = true;
}

void BitmapFrame::release()
{
	if (_data != nullptr && _ownMem)
		free(_data);

	_data = nullptr;
	_width = 0;
	_height = 0;
	_stride = 0;
	_fmt = PixelFormat::NONE;
	_ownMem = false;
}

BitmapFrame::~BitmapFrame()
{
	release();
}

BitmapFrame::BitmapFrame(const BitmapFrame& rhs) : Frame(FrameType::VIDEO_FRAME)
{
	this->copyFrom(rhs, false);
}

BitmapFrame::BitmapFrame(BitmapFrame&& rhs) : Frame(FrameType::VIDEO_FRAME)
{
	_width = rhs._width;
	_height = rhs._height;
	_fmt = rhs._fmt;
	_data = rhs._data;
	_stride = rhs._stride;

	rhs._data = nullptr; 
	rhs._width = 0;
	rhs._height = 0;
	rhs._fmt = PixelFormat::NONE;
	rhs._stride = 0;
}

void* BitmapFrame::GetData()
{
	return _data;
}

size_t BitmapFrame::GetDataSize() const
{
	return _stride * _height;
}

size_t BitmapFrame::GetStride() const
{
	return _stride;
}

int BitmapFrame::GetWidth() const
{
	return _width;
}

int BitmapFrame::GetHeight() const
{
	return _height;
}

PixelFormat BitmapFrame::GetPixelFormat() const
{
	return _fmt;
}

void BitmapFrame::CopyTo(BitmapFrame& dest) const
{
	dest.copyFrom(*this, false);
}

void BitmapFrame::CopyToStrict(BitmapFrame& dest) const
{
	dest.copyFrom(*this, true);
}

BitmapFrame& BitmapFrame::operator=(const BitmapFrame& rhs)
{
	if (this == &rhs)
		return *this;

	this->copyFrom(rhs, false);
}

BitmapFrame& BitmapFrame::operator=(BitmapFrame&& rhs)
{
	if (this == &rhs)
		return *this;

	_width = rhs._width;
	_height = rhs._height;
	_fmt = rhs._fmt;
	_data = rhs._data;
	_stride = rhs._stride;

	rhs._data = nullptr;
	rhs._width = 0;
	rhs._height = 0;
	rhs._fmt = PixelFormat::NONE;
	rhs._stride = 0;
}
