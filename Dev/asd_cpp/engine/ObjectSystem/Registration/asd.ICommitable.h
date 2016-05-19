#pragma once

namespace asd
{
	class ICommitable
	{
	public:
		virtual ~ICommitable()
		{
		}
		virtual void Commit() = 0;
	};
}