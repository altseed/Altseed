#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class ObjectSystem_LayersMember : public EngineTest
{
public:
	ObjectSystem_LayersMember(bool isOpenGLMode)
		: EngineTest(asd::ToAString("LayersMember"), isOpenGLMode, 30)
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

ENGINE_TEST(ObjectSystem, LayersMember)
