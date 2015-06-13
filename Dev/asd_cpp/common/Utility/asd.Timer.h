#pragma once
#include<cstdint>

#ifdef _WIN32
/* win */
#include<Windows.h>
#else
/* *nix */
#include <sys/time.h>
#endif

namespace asd
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
			// オーバーフロー対策
			// return count * 1000000 / freq; と等価

			int64_t ret = 0;
			ret = count / freq;
			count -= ret * freq;
			ret *= 1000000;
			ret += count * 1000000 / freq;
			return ret;
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