
#include "../../EngineTest.h"


class Graphics_SimpleMesh : public EngineTest
{
public:
	
	Graphics_SimpleMesh(bool isOpenGLMode) :
		EngineTest(ace::ToAString("SimpleMesh"), isOpenGLMode, 60)
	{}

protected:
	void OnStart() override
	{
		auto engine = ace::GetEngine();

		auto scene = std::make_shared<ace::Scene>();
		auto layer = std::make_shared<ace::Layer3D>();
		auto meshObj = std::make_shared<ace::MeshObject3D>();
		auto lightObj = std::make_shared<ace::DirectionalLightObject3D>();
		auto cameraObj = std::make_shared<ace::CameraObject3D>();

		scene->AddLayer(layer);
		layer->AddObject(meshObj);
		layer->AddObject(lightObj);
		layer->AddObject(cameraObj);
		engine->ChangeScene(scene);

		auto mesh = ace::GetGraphics()->CreateMesh();
		mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, 0.5), ace::Vector3DF(0, 0, 1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, 0.5), ace::Vector3DF(0, 0, 1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(0.5, 0.5, 0.5), ace::Vector3DF(0, 0, 1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(0.5, -0.5, 0.5), ace::Vector3DF(0, 0, 1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(0.5, 0.5, -0.5), ace::Vector3DF(1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(0.5, -0.5, -0.5), ace::Vector3DF(1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, -0.5), ace::Vector3DF(0, 0, -1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, -0.5), ace::Vector3DF(0, 0, -1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, 0.5), ace::Vector3DF(-1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, 0.5), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, 0.5), ace::Vector3DF(-1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, 0.5), ace::Vector3DF(0, -1, 0), ace::Vector3DF(0, 0, 1), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(0.5, 0.5, 0.5), ace::Vector3DF(1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(0.5, 0.5, 0.5), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(0.5, -0.5, 0.5), ace::Vector3DF(1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(0.5, -0.5, 0.5), ace::Vector3DF(0, -1, 0), ace::Vector3DF(0, 0, 1), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(0.5, 0.5, -0.5), ace::Vector3DF(0, 0, -1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(0.5, 0.5, -0.5), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(0.5, -0.5, -0.5), ace::Vector3DF(0, 0, -1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(0.5, -0.5, -0.5), ace::Vector3DF(0, -1, 0), ace::Vector3DF(0, 0, 1), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, -0.5), ace::Vector3DF(-1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, -0.5), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, -0.5), ace::Vector3DF(-1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), 0, 0);
		mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, -0.5), ace::Vector3DF(0, -1, 0), ace::Vector3DF(0, 0, 1), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), 0, 0);

		mesh->AddFace(0, 2, 3);
		mesh->AddFace(0, 3, 1);
		mesh->AddFace(12, 4, 5);
		mesh->AddFace(12, 5, 14);
		mesh->AddFace(16, 6, 7);
		mesh->AddFace(16, 7, 18);
		mesh->AddFace(20, 8, 10);
		mesh->AddFace(20, 10, 22);
		mesh->AddFace(21, 17, 13);
		mesh->AddFace(21, 13, 9);
		mesh->AddFace(11, 15, 19);
		mesh->AddFace(11, 19, 23);

		mesh->SendToGPUMemory();

		cameraObj->SetPosition(ace::Vector3DF(0, 0, 10));
		cameraObj->SetFocus(ace::Vector3DF(0, 0, 0));
		cameraObj->SetFieldOfView(20.0f);
		cameraObj->SetZNear(1.0f);
		cameraObj->SetZFar(20.0f);
		cameraObj->SetWindowSize(ace::Vector2DI(800, 600));

		meshObj->SetMesh(mesh);
		meshObj->SetRotation(ace::Vector3DF(20.0f, 20.0f, 0.0f));

		lightObj->SetRotation(ace::Vector3DF(30, 160, 0));
	}
};


TEST(Graphics, SimpleMesh_GL)
{
	Graphics_SimpleMesh(true).Run();
}

#if _WIN32
TEST(Graphics, SimpleMesh_DX)
{
	Graphics_SimpleMesh(false).Run();
}
#endif

void Graphics_SimpleMesh_(bool isGL)
{
	Graphics_SimpleMesh(isGL).Run();
}
