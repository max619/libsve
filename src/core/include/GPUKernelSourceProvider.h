#pragma once
#include <string>

class GPUKernelSourceProvider
{
public:
	virtual std::wstring GetSource() const = 0;
};