#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class DisposeOnTerminate_Scene : public Scene
{
public:
	bool m_alreadyDisposed = false;

protected:
	void OnDispose() override
	{
		m_alreadyDisposed = true;
	}
};

void Test_ObjectSystem_DisposeOnTerminate(bool openGL)
{
	auto option = EngineOption();
	option.GraphicsDevice = openGL ? GraphicsDeviceType::OpenGL : GraphicsDeviceType::DirectX11;
	Engine::Initialize(ToAString("DisposeOnTerminate").c_str(), 640, 480, option);

	auto scene = make_shared<DisposeOnTerminate_Scene>();
	Engine::ChangeScene(scene);

	while (Engine::DoEvents())
	{
		Engine::Update();
	}

	Engine::Terminate();

	ASSERT_EQ(true, scene->m_alreadyDisposed);
}