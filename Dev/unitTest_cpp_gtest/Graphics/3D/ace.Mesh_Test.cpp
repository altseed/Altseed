
#include "../ace.Graphics_Test_Utls.h"

#include <Graphics/Common/3D/ace.Renderer3D.h>
#include <Graphics/Common/3D/ace.RenderedMeshObject3D.h>
#include <Graphics/Common/3D/ace.RenderedCameraObject3D.h>
#include <Graphics/Common/3D/ace.RenderedDirectionalLightObject3D.h>
#include <Graphics/Common/3D/ace.Mesh.h>


void Graphics_Mesh(bool isOpenGLMode)
{
	StartGraphicsTest();
	SetGLEnable(isOpenGLMode);

	ace::Log* log = ace::Log_Imp::Create(L"graphics.html", L"メッシュ");

	auto window = ace::Window_Imp::Create(640, 480, ace::ToAString(L"メッシュ").c_str());
	ASSERT_TRUE(window != nullptr);

	auto graphics = ace::Graphics_Imp::Create(window, isOpenGLMode, log);
	ASSERT_TRUE(graphics != nullptr);

	auto renderer = new ace::Renderer3D(graphics);
	ASSERT_TRUE(renderer != nullptr);

	auto mesh = new ace::Mesh(graphics);
	mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, 0.5), ace::Vector3DF(0, 0, 1), ace::Vector3DF(0, 1, 0), ace::Vector3DF(1, 0, 0), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, 0.5), ace::Vector3DF(0, 0, 1), ace::Vector3DF(0, 1, 0), ace::Vector3DF(1, 0, 0), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(0.5, 0.5, 0.5), ace::Vector3DF(0, 0, 1), ace::Vector3DF(0, 1, 0), ace::Vector3DF(1, 0, 0), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(0.5, -0.5, 0.5), ace::Vector3DF(0, 0, 1), ace::Vector3DF(0, 1, 0), ace::Vector3DF(1, 0, 0), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(0.5, 0.5, -0.5), ace::Vector3DF(1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(0.5, -0.5, -0.5), ace::Vector3DF(1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, -0.5), ace::Vector3DF(0, 0, -1), ace::Vector3DF(0, 1, 0), ace::Vector3DF(-1, 0, 0), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, -0.5), ace::Vector3DF(0, 0, -1), ace::Vector3DF(0, 1, 0), ace::Vector3DF(-1, 0, 0), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, 0.5), ace::Vector3DF(-1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, 1), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, 0.5), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector3DF(1, 0, 0), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, 0.5), ace::Vector3DF(-1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, 1), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, 0.5), ace::Vector3DF(0, -1, 0), ace::Vector3DF(0, 0, 1), ace::Vector3DF(1, 0, 0), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(0.5, 0.5, 0.5), ace::Vector3DF(1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(0.5, 0.5, 0.5), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector3DF(1, 0, 0), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(0.5, -0.5, 0.5), ace::Vector3DF(1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(0.5, -0.5, 0.5), ace::Vector3DF(0, -1, 0), ace::Vector3DF(0, 0, 1), ace::Vector3DF(1, 0, 0), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(0.5, 0.5, -0.5), ace::Vector3DF(0, 0, -1), ace::Vector3DF(0, 1, 0), ace::Vector3DF(-1, 0, 0), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(0.5, 0.5, -0.5), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector3DF(1, 0, 0), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(0.5, -0.5, -0.5), ace::Vector3DF(0, 0, -1), ace::Vector3DF(0, 1, 0), ace::Vector3DF(-1, 0, 0), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(0.5, -0.5, -0.5), ace::Vector3DF(0, -1, 0), ace::Vector3DF(0, 0, 1), ace::Vector3DF(1, 0, 0), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, -0.5), ace::Vector3DF(-1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, 1), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, -0.5), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector3DF(1, 0, 0), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, -0.5), ace::Vector3DF(-1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, 1), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), 0, 0);
	mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, -0.5), ace::Vector3DF(0, -1, 0), ace::Vector3DF(0, 0, 1), ace::Vector3DF(1, 0, 0), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), 0, 0);

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

	auto cameraObject = new ace::RenderedCameraObject3D(graphics);
	cameraObject->SetPosition(ace::Vector3DF(0, 0, 10));
	cameraObject->SetFocus(ace::Vector3DF(0, 0, 0));
	cameraObject->SetFieldOfView(20.0f);
	cameraObject->SetZNear(1.0f);
	cameraObject->SetZFar(20.0f);
	cameraObject->SetWindowSize(ace::Vector2DI(800, 600));

	auto meshObject = new ace::RenderedMeshObject3D(graphics);
	meshObject->SetMesh(mesh);
	meshObject->SetRotation(ace::Vector3DF(20.0f, 20.0f, 0.0f));

	auto lightObject = new ace::RenderedDirectionalLightObject3D(graphics);
	lightObject->SetRotation(ace::Vector3DF(30, 160, 0));

	renderer->AddObject(cameraObject);
	renderer->AddObject(meshObject);
	renderer->AddObject(lightObject);

	int32_t time = 0;
	while (window->DoEvent())
	{
		graphics->Begin();
		graphics->Clear(true, false, ace::Color(0, 0, 0, 255));

		renderer->Flip();

		renderer->Rendering();

		graphics->Present();

		graphics->End();

		if (time == 10)
		{
			SAVE_SCREEN_SHOT(graphics, 0);
		}

		if (time == 11)
		{
			window->Close();
		}
		time++;
	}

	mesh->Release();
	meshObject->Release();
	cameraObject->Release();
	lightObject->Release();

	delete renderer;
	graphics->Release();

	delete window;
	delete log;
}

TEST(Graphics, Mesh_DX)
{
	Graphics_Mesh(false);
}

TEST(Graphics, Mesh_GL)
{
	Graphics_Mesh(true);
}

