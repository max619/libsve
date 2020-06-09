#include "Utils.h"

size_t AlignTo(size_t len, size_t alignment)
{
	if (len % alignment == 0)
		return len;
	return (1 + len / alignment) * alignment;
}
