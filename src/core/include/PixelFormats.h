#pragma once 

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