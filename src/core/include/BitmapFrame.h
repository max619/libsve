#pragma once

#include "Frame.h"

enum class PixelFormat
{
	NONE,
	RGB24,
	BGR24,
	RGBA32,
	BGRA32,
	ARGB32,
	ABGR32,
	R8,
	R16
};

class PixelFormatInfo
{
public:
	static constexpr int GetBPP(PixelFormat fmt);
	static constexpr int GetChannels(PixelFormat fmt);
	static constexpr int GetPixelSize(PixelFormat fmt);
};

class BitmapFrame : public Frame
{
public:
	~BitmapFrame() override;
	BitmapFrame(const BitmapFrame& rhs);
	BitmapFrame(BitmapFrame&& rhs) noexcept;
	void* GetData() override;
	size_t GetDataSize() const override;
	size_t GetStride() const;
	int GetWidth() const;
	int GetHeight() const;
	PixelFormat GetPixelFormat() const;
	void CopyTo(BitmapFrame& dest) const;
	void CopyToStrict(BitmapFrame& dest) const;
	
	virtual BitmapFrame& operator=(const BitmapFrame& rhs);
	virtual BitmapFrame& operator=(BitmapFrame&& rhs);
private:
	void copyFrom(const BitmapFrame& src, bool isStrict);
	void allocate(int width, int height, PixelFormat fmt);
	void release();
	void* _data = nullptr;
	int _width = 0, _height = 0;
	size_t  _stride = 0;
	PixelFormat _fmt = PixelFormat::NONE;
	bool _ownMem = false;
};