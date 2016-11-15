#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class MyClass_RepeatedlyAddAndRemoveObject : public TextureObject2D
{
protected:
	void OnAdded() override
	{
		cout << "OnAdded" << endl;
		ASSERT_NE(nullptr, GetLayer());
	}

	void OnRemoved() override
	{
		cout << "OnRemoved" << endl;
	}
};

class BugFix_RepeatedlyAddAndRemoveObject : public EngineTest
{
private:
	Layer2D::Ptr layer;
	shared_ptr<MyClass_RepeatedlyAddAndRemoveObject> obj;

public:
	BugFix_RepeatedlyAddAndRemoveObject(bool isOpenGLMode)
		: EngineTest(asd::ToAString("RepeatedlyAddAndRemoveObject"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		layer = make_shared<Layer2D>();
		obj = make_shared<MyClass_RepeatedlyAddAndRemoveObject>();
	}

	void OnUpdating()
	{
		if (m_currentTime == 20)
		{
			for (size_t i = 0; i < 3; i++)
			{
				layer->AddObject(obj);
				layer->RemoveObject(obj);
			}
		}
	}

	void OnUpdated()
	{
	}

	void OnFinish()
	{
	}
};

ENGINE_TEST(BugFix, RepeatedlyAddAndRemoveObject)