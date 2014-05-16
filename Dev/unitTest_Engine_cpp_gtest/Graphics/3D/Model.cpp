
#include "../../EngineTest.h"
//#include "../../../ace_cpp/core/Graphics/Common/3D/ace.RenderedModelObject3D.h"

class Graphics_Model : public EngineTest
{
	std::shared_ptr<ace::ModelObject3D> meshObj;
	
public:

	Graphics_Model(bool isOpenGLMode) :
		EngineTest(ace::ToAString("Model"), isOpenGLMode, 180)
	{}

protected:
	void OnStart() override
	{
		auto scene = std::make_shared<ace::Scene>();
		auto layer = std::make_shared<ace::Layer3D>();
		meshObj = std::make_shared<ace::ModelObject3D>();
		auto lightObj = std::make_shared<ace::DirectionalLightObject3D>();
		auto cameraObj = std::make_shared<ace::CameraObject3D>();

		scene->AddLayer(layer);
		layer->AddObject(meshObj);
		layer->AddObject(lightObj);
		layer->AddObject(cameraObj);
		ace::Engine::ChangeScene(scene);


		auto graphics = ace::Engine::GetGraphics();

		auto model = graphics->CreateModel(ace::ToAString("Data/Model/out.mdl").c_str());

		const ace::Vector3DF origin = ace::Vector3DF(0, 0, 0);

		cameraObj->SetPosition(ace::Vector3DF(0, 0, 30));
		cameraObj->SetFocus(origin);
		cameraObj->SetFieldOfView(20.0f);
		cameraObj->SetZNear(1.0f);
		cameraObj->SetZFar(1000.0f);
		cameraObj->SetWindowSize(ace::Vector2DI(800, 600));

		meshObj->SetModel(model);
		meshObj->SetRotation(ace::Vector3DF(0, 0, 0));
		lightObj->SetRotation(ace::Vector3DF(180, 0, 0));
	}

	void OnUpdating() override
	{

	}


};


TEST(Graphics, Model_GL)
{
	Graphics_Model(true).Run();
	AssertMemoryDoesntLeak();
}

#if _WIN32
TEST(Graphics, Model_DX)
{
	Graphics_Model(false).Run();
	AssertMemoryDoesntLeak();
}
#endif

void Graphics_Model_(bool isGL)
{
	Graphics_Model(isGL).Run();
	AssertMemoryDoesntLeak();
}

