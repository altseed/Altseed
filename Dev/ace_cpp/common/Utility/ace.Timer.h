#pragma once
#include<cstdint>

#ifdef _WIN32
/* win */
#include<Windows.h>
#else
/* *nix */
#include <sys/time.h>
#endif

namespace ace
{
/**
	@brief	パフォーマンスカウンタの現在の値をナノ秒単位で返す
*/

inline int64_t GetTime()
{
#ifdef _WIN32
	int64_t count, freq;
	if (QueryPerformanceCounter((LARGE_INTEGER*)&count))
	{
		if (QueryPerformanceFrequency((LARGE_INTEGER*)&freq))
		{
			return count * 1000000 / freq;
		}
	}
	return 0;
#else
	struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000000 + (int64_t)tv.tv_usec;
#endif
}
}