#include "../core/PCH/asd.Core.PCH.h"
#include "asd.Synchronizer.h"

namespace asd
{
	SyncInfo::SyncInfo(IAsyncResource * resource, Continuation continuation, int id)
		: resource(resource)
		, continuation(continuation)
		, id(id)
	{
		resource->AddRef();
	}

	SyncInfo::~SyncInfo()
	{
		resource->Release();
	}

	Synchronizer::Synchronizer()
		: requests()
	{
	}

	void Synchronizer::Update()
	{
		auto removed = std::list<SyncInfo>();
		for (auto &info : requests)
		{
			if (info.GetResource()->GetLoadStateInternal() == LoadState::WaitSync
				|| info.GetResource()->GetLoadStateInternal() == LoadState::Loaded)
			{
				info.GetContinuation()();
				removed.push_back(info);
			}
		}

		listMutex.lock();
		for (auto& info : removed)
		{
			requests.remove(info);
		}
		listMutex.unlock();
	}

	void Synchronizer::Start(IAsyncResource * resource, Continuation continuation)
	{
		listMutex.lock();
		finalId++;
		auto info = SyncInfo(resource, continuation, finalId);
		requests.push_back(info);
		listMutex.unlock();
	}
}