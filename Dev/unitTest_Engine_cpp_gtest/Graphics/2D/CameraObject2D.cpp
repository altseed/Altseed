#include <gtest/gtest.h>
#include <ace.h>
#include "../../EngineTest.h"

using namespace std;
using namespace ace;

class Graphics_CameraObject2D : public EngineTest
{
public:
	Graphics_CameraObject2D(bool isOpenGLMode)
		: EngineTest(ace::ToAString("CameraObject2D"), isOpenGLMode, 60)
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

		obj->SetTexture(ace::Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Sample1.png").c_str()));
		m_camera->SetSrc(RectI(100, 100, 312, 312));
		m_camera->SetDst(RectI(10, 10, 200, 200));

		layer->AddObject(obj);
		layer->AddObject(m_camera);
		scene->AddLayer(layer);
		ace::Engine::ChangeScene(scene);
	}

	void OnUpdating()
	{
		auto dst = m_camera->GetDst();
		dst.X += 3;
		m_camera->SetDst(dst);
	}
};

ENGINE_TEST(Graphics, CameraObject2D)
