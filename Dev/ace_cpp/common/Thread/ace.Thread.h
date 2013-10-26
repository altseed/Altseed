#pragma once

#include "../ace.common.Base.h"
#include "ace.CriticalSection.h"

namespace ace
{

class Thread
{
private:
#ifdef _WIN32
	static DWORD ACE_STDCALL ThreadProc( void* arguments );
#else
	static void* ThreadProc( void* arguments );
#endif

private:
#ifdef _WIN32
	HANDLE m_thread;
#else
	pthread_t m_thread;
	bool m_running;
#endif

	void* m_data;
	void (*m_mainProc)( void* );
	CriticalSection m_cs;

public:

	Thread();
	~Thread();


	/**
		@brief スレッドを生成する。
		@param threadFunc	[in] スレッド関数
		@param pData		[in] スレッドに引き渡すデータポインタ
		@return	成否
	*/
	bool Create( void (*threadFunc)( void* ), void* data );

	/**
		@brief スレッド終了を確認する。
	*/
	bool IsExitThread() const;

	/**
		@brief スレッド終了を待つ。
	*/
	bool Wait() const;
};

}