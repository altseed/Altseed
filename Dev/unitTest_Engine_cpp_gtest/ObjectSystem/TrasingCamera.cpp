#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class ObjectSystem_TrasingCamera : public EngineTest
{
public:
	ObjectSystem_TrasingCamera(bool isOpenGLMode)
		: EngineTest(asd::ToAString("TrasingCamera"), isOpenGLMode, 60)
	{
	}

protected:
	CameraObject2D::Ptr camera;
	TextureObject2D::Ptr obj;

	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		obj = make_shared<TextureObject2D>();
		camera = make_shared<CameraObject2D>();

		auto path = ToAString(L"Data/Texture/Cloud1.png");
		auto texture = Engine::GetGraphics()->CreateTexture2D(ToAString(L"Data/Texture/Cloud1.png").c_str());
		obj->SetTexture(texture);

		camera->SetSrc(RectI(0, 0, 640, 480));
		camera->SetDst(RectI(0, 0, 640, 480));

		Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(obj);
		layer->AddObject(camera);
	}

	void OnUpdating()
	{
		auto p = obj->GetPosition();
		obj->SetPosition(Vector2DF(p.X + 1, p.Y + 1));

		auto p2 = obj->GetPosition();
		camera->SetSrc(RectI(p.X, p.Y, 640, 480));
	}
};

void Test_ObjectSystem_TrasingCamera(bool openGL)
{
	RunTest<ObjectSystem_TrasingCamera>(openGL);
}

TEST(ObjectSystem, TrasingCamera_GL)
{
	RunTest<ObjectSystem_TrasingCamera>(true);
}

#if _WIN32
TEST(ObjectSystem, TrasingCamera_DX)
{
	RunTest<ObjectSystem_TrasingCamera>(false);
}
#endif