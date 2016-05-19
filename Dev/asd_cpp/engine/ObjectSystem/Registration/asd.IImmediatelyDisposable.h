#pragma once

namespace asd
{
	class IImmediatelyDisposable
	{
	public:
		virtual ~IImmediatelyDisposable()
		{
		}

		virtual void Dispose() = 0;
		virtual void DisposeImmediately() = 0;
	};
}