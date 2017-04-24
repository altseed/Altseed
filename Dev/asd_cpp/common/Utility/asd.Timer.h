#pragma once

#include<cstdint>

#ifdef _WIN32
/* win */
#include <Windows.h>
#elif ( defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE) )
#include <chrono>
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
#elif ( defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE) )
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
#else
	struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000000 + (int64_t)tv.tv_usec;
#endif
}
}