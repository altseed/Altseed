
#include "ace.RenderingThread.h"

namespace ace
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
		self->m_running = false;
	}

	RenderingThread::RenderingThread()
		: m_running(false)
	{
	}
	RenderingThread::~RenderingThread()
	{
	}

	void RenderingThread::Run()
	{
		if (m_running) return;
		m_running = true;
	}

	bool RenderingThread::IsRunning()
	{
		return false;
	}

	void RenderingThread::AddEvent(RenderingThreadEvent* e)
	{
	}
}