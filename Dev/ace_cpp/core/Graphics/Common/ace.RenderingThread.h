
#pragma once

#include <thread>
#include <mutex>

#include <ace.common.Base.h>
#include "../../ace.ReferenceObject.h"
#include "../../ace.Core.Base.h"

namespace ace
{
	class RenderingThreadEvent;
	class RenderingThread;

	class RenderingThreadEvent
	{
		friend class RenderingThread;
	private:
		bool	m_isExited;

	public:
		RenderingThreadEvent();
		virtual ~RenderingThreadEvent();

		virtual void Event() = 0;
		bool IsExited();
	};

	class RenderingThread
	{
	private:
		std::thread	m_thread;
		std::vector<RenderingThreadEvent*>	m_events;
		bool	m_running;

		static void ThreadFunc(RenderingThread* self);

	public:
		RenderingThread();
		virtual ~RenderingThread();

		void Run();

		bool IsRunning();

		void AddEvent(RenderingThreadEvent* e);
	};
}