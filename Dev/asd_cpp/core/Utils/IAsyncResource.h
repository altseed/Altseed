#pragma once
#include "../asd.ReferenceObject.h"

namespace asd
{
	enum class LoadState
	{
		Loading, WaitSync, Loaded, Failed
	};

	class IAsyncResource
		: public IReference
	{
	public:
		virtual ~IAsyncResource(){}

		virtual LoadState GetLoadState() = 0;
	};
}