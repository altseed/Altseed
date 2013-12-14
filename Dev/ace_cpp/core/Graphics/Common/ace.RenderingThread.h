
#pragma once

#include <thread>
#include <mutex>
#include <queue>

#include <ace.common.Base.h>
#include "../../ace.ReferenceObject.h"
#include "../../ace.Core.Base.h"

#if _WIN32
#include <process.h>
#else
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#endif

namespace ace
{
	typedef void (*RenderingThreadCallback)(void* self);

	/**
		@brief	描画スレッドで発生するイベントのクラス
	*/
	class RenderingThreadEvent
	{
		friend class RenderingThread;
	private:
		bool	m_isExited;

	public:
		RenderingThreadEvent();
		virtual ~RenderingThreadEvent();

		/**
			@brief	描画スレッドで実行される。
		*/
		virtual void Event() = 0;

		/**
			@brief	実行が終了したか取得する。
		*/
		bool IsExited();
	};

	/**
		@brief	描画スレッドのクラス
	*/
	class RenderingThread
	{
	private:
		std::thread	m_thread;
		std::mutex	m_eventMutex;
		std::queue<RenderingThreadEvent*>	m_events;
		bool	m_running;

		void*	m_self;
		RenderingThreadCallback	m_startFunc;
		RenderingThreadCallback	m_endFunc;

		int32_t					m_threadID;

		static void ThreadFunc(RenderingThread* self);

	public:
		RenderingThread();
		virtual ~RenderingThread();

		/**
			@brief	スレッドを実行する。
			@param	self	スレッドのコールバックに指定される引数
			@param	startFunc	スレッドの開始時に呼ばれるコールバック
			@param	endFunc		スレッドの終了時に呼ばれるコールバック
		*/
		void Run(void* self, RenderingThreadCallback startFunc, RenderingThreadCallback endFunc);

		/**
			@brief	スレッドが実行されているか取得する。
		*/
		bool IsRunning();

		/**
			@brief	実行するイベントを取得する。
		*/
		void AddEvent(RenderingThreadEvent* e);

		/**
		@brief	スレッドIDを取得する。
		@return	スレッドID
		*/
		int32_t GetThreadID();
	};

	/**
	@brief	呼び出し元のスレッドIDを取得する。
	@return	スレッドID
	*/
	int32_t GetThreadID();
}