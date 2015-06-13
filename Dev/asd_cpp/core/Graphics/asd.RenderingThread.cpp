
#include "asd.RenderingThread.h"

namespace asd
{


	RenderingThreadEvent::RenderingThreadEvent()
		: m_isExited(true)
	{

	}

	RenderingThreadEvent::~RenderingThreadEvent()
	{

	}

	bool RenderingThreadEvent::IsExited()
	{
		return m_isExited;
	}


	void RenderingThread::ThreadFunc(RenderingThread* self)
	{
		self->m_threadID = ::asd::GetThreadID();

		if (self->m_self != nullptr && self->m_startFunc != nullptr)
		{
			self->m_startFunc(self->m_self);
		}

		while (true)
		{
			self->m_eventMutex.lock();
			if (self->m_events.size() == 0)
			{
				self->m_eventMutex.unlock();
				Sleep(1);
			}
			else
			{
				auto e = self->m_events.front();
				self->m_events.pop();
				self->m_eventMutex.unlock();
				
				if (e != nullptr)
				{
					e->Event();
					e->m_isExited = true;
				}
				else
				{
					break;
				}
			}
		}

		if (self->m_self != nullptr && self->m_endFunc != nullptr)
		{
			self->m_endFunc(self->m_self);
		}

		self->m_running = false;
	}

	RenderingThread::RenderingThread()
		: m_running(false)
		, m_self(nullptr)
		, m_startFunc(nullptr)
		, m_endFunc(nullptr)
		, m_threadID(-1)
	{
	}
	RenderingThread::~RenderingThread()
	{
		m_thread.join();
	}

	void RenderingThread::Run(void* self, RenderingThreadCallback startFunc, RenderingThreadCallback endFunc)
	{
		if (m_running) return;
		m_running = true;

		m_self = self;
		m_startFunc = startFunc;
		m_endFunc = endFunc;

		m_thread = std::thread([&]{ ThreadFunc(this); });
	}

	bool RenderingThread::IsRunning()
	{
		return m_running;
	}

	void RenderingThread::AddEvent(RenderingThreadEvent* e)
	{
		if (e != nullptr)
		{
			e->m_isExited = false;
		}

		m_eventMutex.lock();
		m_events.push(e);
		m_eventMutex.unlock();
	}

	int32_t RenderingThread::GetThreadID()
	{
		return m_threadID;
	}

	int32_t GetThreadID()
	{
#if _WIN32
		return GetCurrentThreadId();
#else
		return syscall(SYS_gettid);
#endif
	}
}
