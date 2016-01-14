#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class ObjectSystem_LifeCycle : public EngineTest
{
	class MessageObject : public TextureObject2D
	{
	protected:
		void OnAdded() override
		{
			isAdded = true;
		}

		void OnRemoved() override
		{
			isAdded = false;
		}

		void OnDispose() override
		{
			haveBeenDisposed = true;
		}

		void OnUpdate() override
		{
			SetAngle(GetAngle() + 2);
		}

		void OnDrawAdditionally() override
		{
			DrawCircleAdditionally(GetPosition(), 50, 10, Color(0, 0, 0, 255), 12, 0, nullptr, AlphaBlendMode::Blend, 2);
		}

	public:
		bool isAdded, haveBeenDisposed;

		MessageObject()
			: isAdded(false)
			, haveBeenDisposed(false)
		{
			auto texture = Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Textures/Cloud1.png").c_str());
			SetTexture(texture);
		}
	};

	class MessageLayer : public Layer2D
	{
	protected:
		void OnAdded() override
		{
			isAdded = true;
		}

		void OnRemoved() override
		{
			isAdded = false;
		}

		void OnUpdating() override
		{
			counter++;
		}

		void OnUpdated() override
		{
			if (counter % 30 == 0)
			{
				AddObject(make_shared<MessageObject>());
			}
		}

		void OnDrawAdditionally() override
		{
			DrawCircleAdditionally(Vector2DF(100, 100), 100, 10, Color(255, 0, 0, 255), 12, 0, nullptr, AlphaBlendMode::Blend, 2);
		}

		void OnVanish() override
		{
			ACE_ASSERT(false, "OnVanish is Obsolete.");
		}

		void OnDispose() override
		{
			haveBeenDisposed = false;
		}

	public:
		bool isAdded, haveBeenDisposed;
		int counter;

		MessageLayer()
			: isAdded(false)
			, haveBeenDisposed(false)
			, counter(0)
		{
		}
	};

public:
	ObjectSystem_LifeCycle(bool isOpenGLMode)
		: EngineTest(asd::ToAString("LifeCycle"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
	}

	void OnUpdating()
	{
	}

	void OnUpdated()
	{
	}

	void OnFinish()
	{
	}
};

ENGINE_TEST(ObjectSystem, LifeCycle)