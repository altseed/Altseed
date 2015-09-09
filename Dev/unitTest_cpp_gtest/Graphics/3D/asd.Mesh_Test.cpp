
#include "../asd.Graphics_Test_Utls.h"

#include <Graphics/3D/Renderer/asd.Renderer3D.h>
#include <Graphics/3D/Object/asd.RenderedModelObject3D.h>
#include <Graphics/3D/Object/asd.RenderedCameraObject3D.h>
#include <Graphics/3D/Object/asd.RenderedDirectionalLightObject3D.h>

#include <Graphics/3D/Resource/asd.Mesh.h>
#include <Graphics/3D/Resource/asd.Deformer.h>

#include <Graphics/3D/Resource/Animation/asd.AnimationSystem.h>
#include <Graphics/3D/Resource/Animation/asd.AnimationSystem_Imp.h>

#include <Graphics/3D/Resource/Animation/asd.AnimationClip.h>
#include <Graphics/3D/Resource/Animation/asd.AnimationSource.h>
#include <Graphics/3D/Resource/Animation/asd.KeyframeAnimation.h>

#include <Graphics/2D/asd.Renderer2D_Imp.h>

static std::shared_ptr<asd::Mesh> CreateMesh(asd::Graphics* graphics)
{
	uint8_t weights1[4];
	weights1[0] = 255;
	weights1[1] = 0;
	weights1[2] = 0;
	weights1[3] = 0;
	int* pweights1 = (int*) weights1;

	uint8_t weights2[4];
	weights2[0] = 0;
	weights2[1] = 255;
	weights2[2] = 0;
	weights2[3] = 0;
	int* pweights2 = (int*) weights2;

	uint8_t indexes[4];
	indexes[0] = 0;
	indexes[1] = 1;
	indexes[2] = 0;
	indexes[3] = 0;
	int* pindexes = (int*) indexes;

	auto mesh = graphics->CreateMesh();
	mesh->AddVertex(asd::Vector3DF(-0.5, 0.5, 0.5), asd::Vector3DF(0, 0, 1), asd::Vector3DF(0, 1, 0), asd::Vector2DF(0, 0), asd::Vector2DF(0, 0), asd::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(asd::Vector3DF(-0.5, -0.5, 0.5), asd::Vector3DF(0, 0, 1), asd::Vector3DF(0, 1, 0), asd::Vector2DF(0, 1), asd::Vector2DF(0, 1), asd::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(asd::Vector3DF(0.5, 0.5, 0.5), asd::Vector3DF(0, 0, 1), asd::Vector3DF(0, 1, 0), asd::Vector2DF(1, 0), asd::Vector2DF(1, 0), asd::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(asd::Vector3DF(0.5, -0.5, 0.5), asd::Vector3DF(0, 0, 1), asd::Vector3DF(0, 1, 0), asd::Vector2DF(1, 1), asd::Vector2DF(1, 1), asd::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(asd::Vector3DF(0.5, 0.5, -0.5), asd::Vector3DF(1, 0, 0), asd::Vector3DF(0, 1, 0), asd::Vector2DF(1, 0), asd::Vector2DF(1, 0), asd::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(asd::Vector3DF(0.5, -0.5, -0.5), asd::Vector3DF(1, 0, 0), asd::Vector3DF(0, 1, 0), asd::Vector2DF(1, 1), asd::Vector2DF(1, 1), asd::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(asd::Vector3DF(-0.5, 0.5, -0.5), asd::Vector3DF(0, 0, -1), asd::Vector3DF(0, 1, 0), asd::Vector2DF(1, 0), asd::Vector2DF(1, 0), asd::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(asd::Vector3DF(-0.5, -0.5, -0.5), asd::Vector3DF(0, 0, -1), asd::Vector3DF(0, 1, 0), asd::Vector2DF(1, 1), asd::Vector2DF(1, 1), asd::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(asd::Vector3DF(-0.5, 0.5, 0.5), asd::Vector3DF(-1, 0, 0), asd::Vector3DF(0, 1, 0), asd::Vector2DF(1, 0), asd::Vector2DF(1, 0), asd::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(asd::Vector3DF(-0.5, 0.5, 0.5), asd::Vector3DF(0, 1, 0), asd::Vector3DF(0, 0, -1), asd::Vector2DF(0, 1), asd::Vector2DF(0, 1), asd::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(asd::Vector3DF(-0.5, -0.5, 0.5), asd::Vector3DF(-1, 0, 0), asd::Vector3DF(0, 1, 0), asd::Vector2DF(1, 1), asd::Vector2DF(1, 1), asd::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(asd::Vector3DF(-0.5, -0.5, 0.5), asd::Vector3DF(0, -1, 0), asd::Vector3DF(0, 0, 1), asd::Vector2DF(0, 0), asd::Vector2DF(0, 0), asd::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(asd::Vector3DF(0.5, 0.5, 0.5), asd::Vector3DF(1, 0, 0), asd::Vector3DF(0, 1, 0), asd::Vector2DF(0, 0), asd::Vector2DF(0, 0), asd::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(asd::Vector3DF(0.5, 0.5, 0.5), asd::Vector3DF(0, 1, 0), asd::Vector3DF(0, 0, -1), asd::Vector2DF(1, 1), asd::Vector2DF(1, 1), asd::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(asd::Vector3DF(0.5, -0.5, 0.5), asd::Vector3DF(1, 0, 0), asd::Vector3DF(0, 1, 0), asd::Vector2DF(0, 1), asd::Vector2DF(0, 1), asd::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(asd::Vector3DF(0.5, -0.5, 0.5), asd::Vector3DF(0, -1, 0), asd::Vector3DF(0, 0, 1), asd::Vector2DF(1, 0), asd::Vector2DF(1, 0), asd::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(asd::Vector3DF(0.5, 0.5, -0.5), asd::Vector3DF(0, 0, -1), asd::Vector3DF(0, 1, 0), asd::Vector2DF(0, 0), asd::Vector2DF(0, 0), asd::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(asd::Vector3DF(0.5, 0.5, -0.5), asd::Vector3DF(0, 1, 0), asd::Vector3DF(0, 0, -1), asd::Vector2DF(1, 0), asd::Vector2DF(1, 0), asd::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(asd::Vector3DF(0.5, -0.5, -0.5), asd::Vector3DF(0, 0, -1), asd::Vector3DF(0, 1, 0), asd::Vector2DF(0, 1), asd::Vector2DF(0, 1), asd::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(asd::Vector3DF(0.5, -0.5, -0.5), asd::Vector3DF(0, -1, 0), asd::Vector3DF(0, 0, 1), asd::Vector2DF(1, 1), asd::Vector2DF(1, 1), asd::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(asd::Vector3DF(-0.5, 0.5, -0.5), asd::Vector3DF(-1, 0, 0), asd::Vector3DF(0, 1, 0), asd::Vector2DF(0, 0), asd::Vector2DF(0, 0), asd::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(asd::Vector3DF(-0.5, 0.5, -0.5), asd::Vector3DF(0, 1, 0), asd::Vector3DF(0, 0, -1), asd::Vector2DF(0, 0), asd::Vector2DF(0, 0), asd::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(asd::Vector3DF(-0.5, -0.5, -0.5), asd::Vector3DF(-1, 0, 0), asd::Vector3DF(0, 1, 0), asd::Vector2DF(0, 1), asd::Vector2DF(0, 1), asd::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(asd::Vector3DF(-0.5, -0.5, -0.5), asd::Vector3DF(0, -1, 0), asd::Vector3DF(0, 0, 1), asd::Vector2DF(0, 1), asd::Vector2DF(0, 1), asd::Color(255, 255, 255, 255), *pweights2, *pindexes);

	mesh->AddFace(0, 2, 3, 0);
	mesh->AddFace(0, 3, 1, 0);
	mesh->AddFace(12, 4, 5, 0);
	mesh->AddFace(12, 5, 14, 0);
	mesh->AddFace(16, 6, 7, 0);
	mesh->AddFace(16, 7, 18, 0);
	mesh->AddFace(20, 8, 10, 0);
	mesh->AddFace(20, 10, 22, 0);
	mesh->AddFace(21, 17, 13, 0);
	mesh->AddFace(21, 13, 9, 0);
	mesh->AddFace(11, 15, 19, 0);
	mesh->AddFace(11, 19, 23, 0);

	mesh->AddMaterial();

	auto texture = graphics->CreateTexture2D(asd::ToAString(L"Data/Texture/Sample1.png").c_str());

	mesh->SetColorTexture(0, texture.get());
	return mesh;
}

static std::shared_ptr<asd::Deformer> CreateDeformer( asd::Graphics* graphics)
{
	auto deformer = graphics->CreateDeformer();

	asd::Matrix44 mat1, mat2;
	mat1.SetTranslation(0, -0.5, 0);
	mat2.SetTranslation(0, 0.5, 0);

	asd::Matrix44 mat1_inv, mat2_inv;
	mat1_inv = mat1.GetInverted();
	
	asd::Matrix44::Mul(mat2_inv, mat2, mat1);
	mat2_inv = mat2_inv.GetInverted();
	
	deformer->AddBone(asd::ToAString("no1").c_str(), -1, asd::RotationOrder::ZXY, mat1, asd::Vector3DF(1.0f, 1.0f, 1.0f), asd::Vector4DF(0.0f, 0.0f, 0.0f, 0.0f), asd::Vector3DF(1.0f, 1.0f, 1.0f));
	deformer->AddBone(asd::ToAString("no2").c_str(), 0, asd::RotationOrder::ZXY, mat2, asd::Vector3DF(1.0f, 1.0f, 1.0f), asd::Vector4DF(0.0f, 0.0f, 0.0f, 0.0f), asd::Vector3DF(1.0f, 1.0f, 1.0f));

	return deformer;
}

static void SetMeshBone(std::shared_ptr<asd::Mesh> mesh)
{
	asd::Matrix44 mat1, mat2;
	mat1.SetTranslation(0, -0.5, 0);
	mat2.SetTranslation(0, 0.5, 0);

	asd::Matrix44 mat1_inv, mat2_inv;
	mat1_inv = mat1.GetInverted();

	asd::Matrix44::Mul(mat2_inv, mat2, mat1);
	mat2_inv = mat2_inv.GetInverted();

	mesh->AddBoneConnector(0, mat1_inv);
	mesh->AddBoneConnector(1, mat2_inv);
}

static std::shared_ptr<asd::AnimationClip> CreateAnimation()
{
	auto factory = new asd::AnimationSystem_Imp();

	auto clip = factory->CreateAnimationClip();
	auto source = factory->CreateAnimationSource();
	auto key = factory->CreateKeyframeAnimation();

	asd::FCurveKeyframe k1;
	k1.KeyValue.X = 0.0;
	k1.KeyValue.Y = 20;
	k1.LeftHandle = k1.KeyValue;
	k1.RightHandle = k1.KeyValue;

	asd::FCurveKeyframe k2;
	k2.KeyValue.X = 10.0;
	k2.KeyValue.Y = -20;
	k2.LeftHandle = k2.KeyValue;
	k2.RightHandle = k2.KeyValue;

	key->SetName(asd::ToAString("no2.rot.z").c_str());
	key->AddKeyframe(k1);
	key->AddKeyframe(k2);

	source->AddAnimation(key);
	clip->SetSource(source);
	delete factory;

	return clip;
}

void Graphics_Mesh(bool isOpenGLMode)
{
	StartGraphicsTest();
	SetGLEnable(isOpenGLMode);

	asd::Log* log = asd::Log_Imp::Create(L"graphics.html", L"メッシュ");

	auto window = asd::Window_Imp::Create(640, 480, asd::ToAString(L"メッシュ").c_str());
	ASSERT_TRUE(window != nullptr);

	auto file = asd::File_Imp::Create();
	ASSERT_TRUE(file != nullptr);

	auto graphics = asd::Graphics_Imp::Create(window, isOpenGLMode ? asd::GraphicsDeviceType::OpenGL : asd::GraphicsDeviceType::DirectX11, log, file, false, false);
	ASSERT_TRUE(graphics != nullptr);

	auto renderer3d = new asd::Renderer3D(graphics, asd::RenderSettings());
	ASSERT_TRUE(renderer3d != nullptr);
	renderer3d->SetWindowSize(asd::Vector2DI(640, 480));

	auto mesh1 = CreateMesh(graphics);
	auto mesh2 = CreateMesh(graphics);
	auto deformer = CreateDeformer(graphics);
	auto animation = CreateAnimation();

	SetMeshBone(mesh2);

	mesh1->SendToGPUMemory();
	mesh2->SendToGPUMemory();

	auto cameraObject = new asd::RenderedCameraObject3D(graphics);
	cameraObject->SetPosition(asd::Vector3DF(0, 0, 10));
	cameraObject->SetFocus(asd::Vector3DF(0, 0, 0));
	cameraObject->SetFieldOfView(20.0f);
	cameraObject->SetZNear(1.0f);
	cameraObject->SetZFar(20.0f);
	cameraObject->SetWindowSize(asd::Vector2DI(800, 600));

	auto meshObject1 = new asd::RenderedModelObject3D(graphics);
	meshObject1->AddMesh(mesh1);
	
	meshObject1->SetPosition(asd::Vector3DF(1, 0, 0));
	meshObject1->SetRotation(asd::Vector3DF(20.0f, 20.0f, 0.0f));

	auto meshObject2 = new asd::RenderedModelObject3D(graphics);
	meshObject2->AddMesh(mesh2);
	meshObject2->SetDeformer(deformer.get());
	meshObject2->SetPosition(asd::Vector3DF(-1, 0, 0));
	meshObject2->SetRotation(asd::Vector3DF(20.0f, 20.0f, 0.0f));
	meshObject2->AddAnimationClip(asd::ToAString("anime1").c_str(), animation.get());
	meshObject2->PlayAnimation(0, asd::ToAString("anime1").c_str());

	auto lightObject = new asd::RenderedDirectionalLightObject3D(graphics);
	lightObject->SetRotation(asd::Vector3DF(30, 160, 0));

	renderer3d->AddObject(cameraObject);
	renderer3d->AddObject(meshObject1);
	renderer3d->AddObject(meshObject2);
	renderer3d->AddObject(lightObject);

	int32_t time = 0;
	while (window->DoEvent())
	{
		graphics->Begin();
		graphics->Clear(true, false, asd::Color(0, 0, 0, 255));

		renderer3d->Flip(1);

		renderer3d->BeginRendering(1);
		asd::Sleep(100);
		renderer3d->EndRendering();

		graphics->SetRenderTarget(nullptr, nullptr);
		graphics->Clear(true, false, asd::Color(0, 0, 0, 255));

		renderer3d->RenderResult();

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

	meshObject1->Release();
	meshObject2->Release();

	cameraObject->Release();
	lightObject->Release();

	delete renderer3d;

	graphics->Release();
	file->Release();

	window->Release();
	delete log;
}

#ifdef _WIN32
TEST(Graphics, Mesh_DX)
{
	Graphics_Mesh(false);
}
#endif

TEST(Graphics, Mesh_GL)
{
	Graphics_Mesh(true);
}

