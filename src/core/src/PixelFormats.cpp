#include "PixelFormats.h"
#include <exception>

constexpr int PixelFormatInfo::GetBPP(PixelFormat fmt)
{
	switch (fmt)
	{
	case PixelFormat::RGB24:
	case PixelFormat::BGR24:
		return 24;
	case PixelFormat::RGBA32:
	case PixelFormat::BGRA32:
	case PixelFormat::ARGB32:
	case PixelFormat::ABGR32:
		return 32;
	case PixelFormat::R8:
		return 8;
	case PixelFormat::R16:
		return 16;
	default:
		throw std::exception("Unsupported PixelFormat");
		break;
	}
}

constexpr int PixelFormatInfo::GetChannels(PixelFormat fmt)
{
	switch (fmt)
	{
	case PixelFormat::RGB24:
	case PixelFormat::BGR24:
		return 3;
	case PixelFormat::RGBA32:
	case PixelFormat::BGRA32:
	case PixelFormat::ARGB32:
	case PixelFormat::ABGR32:
		return 4;
	case PixelFormat::R8:
	case PixelFormat::R16:
		return 1;
	default:
		throw std::exception("Unsupported PixelFormat");
		break;
	}
}

constexpr int PixelFormatInfo::GetPixelSize(PixelFormat fmt)
{
	return GetBPP(fmt) / 8;
}
