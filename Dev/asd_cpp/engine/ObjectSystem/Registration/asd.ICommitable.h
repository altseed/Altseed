#pragma once

namespace asd
{
	class ICommitable
	{
	public:
		virtual void Commit() = 0;
	};
}