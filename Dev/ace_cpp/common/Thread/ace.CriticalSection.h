#pragma once


#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#endif


namespace ace
{
/**
	@brief	クリティカルセクション
*/
class CriticalSection
{
private:
#ifdef _WIN32
	mutable CRITICAL_SECTION m_criticalSection;
#else
	mutable pthread_mutex_t m_mutex;
#endif

public:

	CriticalSection();

	~CriticalSection();

	void Enter() const;

	void Leave() const;
};

}