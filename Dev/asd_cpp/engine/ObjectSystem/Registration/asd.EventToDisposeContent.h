#pragma once
#include <memory>
#include "asd.ICommitable.h"
#include "asd.IImmediatelyDisposable.h"

namespace asd
{
	class EventToDisposeContent : public ICommitable
	{
	private:
		std::shared_ptr<IImmediatelyDisposable> m_disposable;

	public:
		EventToDisposeContent(std::shared_ptr<IImmediatelyDisposable> disposable)
			: m_disposable(disposable)
		{
		}

		void Commit()
		{
			m_disposable->DisposeImmediately();
		}
	};
}