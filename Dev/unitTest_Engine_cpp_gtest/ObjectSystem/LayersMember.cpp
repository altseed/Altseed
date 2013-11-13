#include <gtest/gtest.h>
#include <ace.h>

using namespace std;
using namespace ace;

void AssertLayersMembers()
{
	auto layer = make_shared<Layer2D>();

	layer->SetDrawingPriority(1);
	ASSERT_EQ(layer->GetDrawingPriority(), 1);

	layer->SetIsDrawn(true);
	ASSERT_EQ(layer->GetIsDrawn(), true);

	layer->SetIsUpdated(true);
	ASSERT_EQ(layer->GetIsUpdated(), true);
}

void ObjectSystem_LayersMember(bool isOpenGLMode)
{
	int time = 0;
	ace::EngineOption option;
	option.GraphicsType = isOpenGLMode ? ace::GRAPHICS_TYPE_GL : ace::GRAPHICS_TYPE_DX11;

	auto engine = ace::GetEngine();
	engine->Initialize(ace::ToAString("LayersMembers").c_str(), 640, 480, option);

	{
		AssertLayersMembers();

		while (engine->DoEvents())
		{
			engine->Update();
			++time;
			if (time == 120)
			{
				break;
			}
		}
	}

	engine->Terminate();
}

TEST(ObjectSystem, LayersMember_GL)
{
	ObjectSystem_LayersMember(true);
}

#if _WIN32
TEST(ObjectSystem, LayersMember_DX)
{
	ObjectSystem_LayersMember(false);
}
#endif
