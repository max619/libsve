#pragma once
#include "PixelFormats.h"

enum class FrameType
{
	VIDEO_FRAME,
	AUDIO_FRAME,
	DATA_FRAME
};

class Frame
{
public:
	Frame(const Frame&) = delete;
	virtual ~Frame();
	virtual void* GetData() = 0;
	virtual size_t GetDataSize() const = 0;
	FrameType GetType() const;
	bool Is(FrameType type) const;
	size_t GetTimestamp() const;
	void SetTimestamp(size_t timeStamp);

	Frame& operator=(const Frame&) = delete;

protected:
	Frame(FrameType type);

private:
	FrameType _type;
	size_t _timestamp = 0;
};

class VideoFrame : public Frame
{
public:
	VideoFrame(const VideoFrame& rhs) = delete;
	VideoFrame(VideoFrame&& rhs) = delete;
	virtual size_t GetStride() const = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual PixelFormat GetPixelFormat() const = 0;
	virtual void CopyTo(VideoFrame& dest) const = 0;

	virtual VideoFrame& operator=(const VideoFrame& rhs) = delete;
	virtual VideoFrame& operator=(VideoFrame&& rhs) = delete;

protected:
	VideoFrame();
};