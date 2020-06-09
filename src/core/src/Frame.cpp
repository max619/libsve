#include "Frame.h"

Frame::~Frame()
{
}

FrameType Frame::GetType() const
{
	return _type;
}

size_t Frame::GetTimestamp() const
{
	return _timestamp;
}

void Frame::SetTimestamp(size_t timeStamp)
{
	_timestamp = timeStamp;
}

Frame::Frame(FrameType type) : _type(type)
{

}

bool Frame::Is(FrameType type) const
{
	return _type == type;
}

VideoFrame::VideoFrame() : Frame(FrameType::VIDEO_FRAME)
{
}
