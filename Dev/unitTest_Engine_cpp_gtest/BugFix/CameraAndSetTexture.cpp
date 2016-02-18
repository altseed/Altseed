#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class ObjectSystem_CameraAndSetTexture : public EngineTest
{
public:
	ObjectSystem_CameraAndSetTexture(bool isOpenGLMode)
		: EngineTest(asd::ToAString("CameraAndSetTexture"), isOpenGLMode, 30)
	{
	}

protected:
	int count = 0;
	shared_ptr<TextureObject2D> obj;
	shared_ptr<CameraObject2D> camera;

	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		obj = make_shared<TextureObject2D>();
		camera = make_shared<CameraObject2D>();

		Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(obj);

		obj->SetTexture(Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Sample1.png").c_str()));
		camera->SetSrc(RectI(0, 0, 640, 480));
		camera->SetDst(RectI(0, 0, 640, 480));
	}

	void OnUpdating()
	{
		camera->SetSrc(RectI(0, 0, 640, 480));
		++count;
		if (count == 10)
		{
			auto graphics = Engine::GetGraphics();
			obj->SetTexture(graphics->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str()));
		}
	}
};

ENGINE_TEST(ObjectSystem, CameraAndSetTexture)
