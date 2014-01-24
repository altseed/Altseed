#include <gtest/gtest.h>
#include <ace.h>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class ObjectSystem_Camera : public EngineTest
{
public:
	ObjectSystem_Camera(bool isOpenGLMode)
		: EngineTest(ace::ToAString("Camera"), isOpenGLMode, 60)
	{
	}

private:
	shared_ptr<CameraObject2D> m_camera;

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto obj = make_shared<TextureObject2D>();
		m_camera = make_shared<CameraObject2D>();

		obj->SetTexture(GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Sample1.png").c_str()));
		m_camera->SetSrc(RectI(100, 100, 312, 312));
		m_camera->SetDst(RectI(10, 10, 200, 200));

		layer->AddObject(obj);
		layer->AddObject(m_camera);
		scene->AddLayer(layer);
		GetEngine()->ChangeScene(scene);
	}

	void OnUpdating()
	{
		auto dst = m_camera->GetDst();
		dst.X += 3;
		m_camera->SetDst(dst);
	}
};

void TestCamera(bool openGL)
{
	RunTest<ObjectSystem_Camera>(openGL);
}

TEST(ObjectSystem, Camera_GL)
{
	RunTest<ObjectSystem_Camera>(true);
}

#if _WIN32
TEST(ObjectSystem, Camera_DX)
{
	RunTest<ObjectSystem_Camera>(false);
}
#endif