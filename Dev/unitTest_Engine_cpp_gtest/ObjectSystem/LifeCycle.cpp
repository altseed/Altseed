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
			DrawCircleAdditionally(GetPosition(), 50, 10, Color(0, 255, 0, 255), 12, 0, nullptr, AlphaBlendMode::Blend, 2);
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
			haveBeenDisposed = true;
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

	class MessageScene : public Scene
	{
	protected:
		void OnRegistered() override
		{
			ASSERT_TRUE(phase == 0 || phase == 6);
			phase = 1;
		}

		void OnStartUpdating() override
		{
			ASSERT_EQ(phase, 1);
			phase = 2;
		}

		void OnTransitionFinished() override
		{
			ASSERT_EQ(phase, 2);
			phase = 3;
		}

		void OnChanging() override
		{
			ASSERT_EQ(phase, 3);
			phase = 4;
		}

		void OnStopUpdating() override
		{
			ASSERT_EQ(phase, 4);
			phase = 5;
		}

		void OnUnregistered() override
		{
			ASSERT_EQ(phase, 5);
			phase = 6;
		}

		void OnDispose() override
		{
			haveBeenDisposed = true;
		}

	public:
		int phase;
		bool haveBeenDisposed;

		MessageScene()
			: phase(0)
			, haveBeenDisposed(false)
		{
		}
	};

public:
	ObjectSystem_LifeCycle(bool isOpenGLMode)
		: EngineTest(asd::ToAString("LifeCycle"), isOpenGLMode, 120)
	{
	}

private:
	shared_ptr<MessageScene> scene;
	shared_ptr<MessageLayer> layer;
	shared_ptr<MessageObject> object;

protected:
	void OnStart()
	{
		scene = make_shared<MessageScene>();
		layer = make_shared<MessageLayer>();
		object = make_shared<MessageObject>();

		ASSERT_EQ(scene->phase, 0);
		ASSERT_EQ(scene->haveBeenDisposed, false);
		ASSERT_EQ(layer->haveBeenDisposed, false);
		ASSERT_EQ(layer->isAdded, false);
		ASSERT_EQ(object->isAdded, false);
		ASSERT_EQ(object->haveBeenDisposed, false);

		layer->AddObject(object);
		ASSERT_EQ(object->isAdded, true);
		layer->RemoveObject(object);
		ASSERT_EQ(object->isAdded, false);
		layer->AddObject(object);

		scene->AddLayer(layer);
		ASSERT_EQ(layer->isAdded, true);
		scene->RemoveLayer(layer);
		ASSERT_EQ(layer->isAdded, false);
		scene->AddLayer(layer);

		Engine::ChangeSceneWithTransition(scene, make_shared<TransitionFade>(0.3f, 0.3f));
	}

	void OnUpdating()
	{
		if (m_currentTime == 65)
		{
			Engine::ChangeSceneWithTransition(nullptr, make_shared<TransitionFade>(0.3f, 0.3f));
		}
	}

	void OnUpdated()
	{
	}

	void OnFinish()
	{
		scene->Dispose();
		ASSERT_EQ(scene->phase, 6);
		ASSERT_EQ(scene->haveBeenDisposed, true);
		ASSERT_EQ(layer->haveBeenDisposed, true);
		ASSERT_EQ(object->haveBeenDisposed, true);
	}
};

ENGINE_TEST(ObjectSystem, LifeCycle)