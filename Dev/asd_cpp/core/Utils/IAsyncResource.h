#pragma once
#include "../asd.ReferenceObject.h"

namespace asd
{
	class IAsyncResource
		: public IReference
	{
	public:
		virtual ~IAsyncResource(){}

		virtual LoadState GetLoadStateInternal() const = 0;
	};
}