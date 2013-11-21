#include <gtest/gtest.h>
#include <ace.h>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class ObjectSystem_LayersMember : public EngineTest
{
public:
	ObjectSystem_LayersMember(bool isOpenGLMode)
		: EngineTest(ace::ToAString("LayersMember"), isOpenGLMode, 30)
	{
	}

protected:
	void OnStart()
	{
		auto layer = make_shared<Layer2D>();

		layer->SetDrawingPriority(1);
		ASSERT_EQ(layer->GetDrawingPriority(), 1);

		layer->SetIsDrawn(true);
		ASSERT_EQ(layer->GetIsDrawn(), true);

		layer->SetIsUpdated(true);
		ASSERT_EQ(layer->GetIsUpdated(), true);
	}
};

TEST(ObjectSystem, LayersMember_GL)
{
	RunTest<ObjectSystem_LayersMember>(true);
}

#if _WIN32
TEST(ObjectSystem, LayersMember_DX)
{
	RunTest<ObjectSystem_LayersMember>(false);
}
#endif
