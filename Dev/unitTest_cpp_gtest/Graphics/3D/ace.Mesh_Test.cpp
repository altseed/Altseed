
#include "../ace.Graphics_Test_Utls.h"

#include <Graphics/3D/Renderer/ace.Renderer3D.h>
#include <Graphics/3D/Object/ace.RenderedModelObject3D.h>
#include <Graphics/3D/Object/ace.RenderedCameraObject3D.h>
#include <Graphics/3D/Object/ace.RenderedDirectionalLightObject3D.h>

#include <Graphics/3D/Resource/ace.Mesh.h>
#include <Graphics/3D/Resource/ace.Deformer.h>

#include <Graphics/3D/Resource/Animation/ace.AnimationSystem.h>
#include <Graphics/3D/Resource/Animation/ace.AnimationSystem_Imp.h>

#include <Graphics/3D/Resource/Animation/ace.AnimationClip.h>
#include <Graphics/3D/Resource/Animation/ace.AnimationSource.h>
#include <Graphics/3D/Resource/Animation/ace.KeyframeAnimation.h>

#include <Graphics/2D/ace.Renderer2D_Imp.h>

static std::shared_ptr<ace::Mesh> CreateMesh(ace::Graphics* graphics)
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
	mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, 0.5), ace::Vector3DF(0, 0, 1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, 0.5), ace::Vector3DF(0, 0, 1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(ace::Vector3DF(0.5, 0.5, 0.5), ace::Vector3DF(0, 0, 1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(ace::Vector3DF(0.5, -0.5, 0.5), ace::Vector3DF(0, 0, 1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(ace::Vector3DF(0.5, 0.5, -0.5), ace::Vector3DF(1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(ace::Vector3DF(0.5, -0.5, -0.5), ace::Vector3DF(1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, -0.5), ace::Vector3DF(0, 0, -1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, -0.5), ace::Vector3DF(0, 0, -1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, 0.5), ace::Vector3DF(-1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, 0.5), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, 0.5), ace::Vector3DF(-1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, 0.5), ace::Vector3DF(0, -1, 0), ace::Vector3DF(0, 0, 1), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(ace::Vector3DF(0.5, 0.5, 0.5), ace::Vector3DF(1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(ace::Vector3DF(0.5, 0.5, 0.5), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(ace::Vector3DF(0.5, -0.5, 0.5), ace::Vector3DF(1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(ace::Vector3DF(0.5, -0.5, 0.5), ace::Vector3DF(0, -1, 0), ace::Vector3DF(0, 0, 1), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(ace::Vector3DF(0.5, 0.5, -0.5), ace::Vector3DF(0, 0, -1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(ace::Vector3DF(0.5, 0.5, -0.5), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector2DF(1, 0), ace::Vector2DF(1, 0), ace::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(ace::Vector3DF(0.5, -0.5, -0.5), ace::Vector3DF(0, 0, -1), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(ace::Vector3DF(0.5, -0.5, -0.5), ace::Vector3DF(0, -1, 0), ace::Vector3DF(0, 0, 1), ace::Vector2DF(1, 1), ace::Vector2DF(1, 1), ace::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, -0.5), ace::Vector3DF(-1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(ace::Vector3DF(-0.5, 0.5, -0.5), ace::Vector3DF(0, 1, 0), ace::Vector3DF(0, 0, -1), ace::Vector2DF(0, 0), ace::Vector2DF(0, 0), ace::Color(255, 255, 255, 255), *pweights1, *pindexes);
	mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, -0.5), ace::Vector3DF(-1, 0, 0), ace::Vector3DF(0, 1, 0), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), *pweights2, *pindexes);
	mesh->AddVertex(ace::Vector3DF(-0.5, -0.5, -0.5), ace::Vector3DF(0, -1, 0), ace::Vector3DF(0, 0, 1), ace::Vector2DF(0, 1), ace::Vector2DF(0, 1), ace::Color(255, 255, 255, 255), *pweights2, *pindexes);

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

	auto texture = graphics->CreateTexture2D(ace::ToAString(L"Data/Texture/Sample1.png").c_str());

	mesh->SetColorTexture(0, texture.get());
	return mesh;
}

static std::shared_ptr<ace::Deformer> CreateDeformer( ace::Graphics* graphics)
{
	auto deformer = graphics->CreateDeformer();

	ace::Matrix44 mat1, mat2;
	mat1.SetTranslation(0, -0.5, 0);
	mat2.SetTranslation(0, 0.5, 0);

	ace::Matrix44 mat1_inv, mat2_inv;
	mat1_inv = mat1.GetInverted();
	
	ace::Matrix44::Mul(mat2_inv, mat2, mat1);
	mat2_inv = mat2_inv.GetInverted();
	
	deformer->AddBone(ace::ToAString("no1").c_str(), -1, ace::RotationOrder::ZXY, mat1);
	deformer->AddBone(ace::ToAString("no2").c_str(), 0, ace::RotationOrder::ZXY, mat2);

	return deformer;
}

static void SetMeshBone(std::shared_ptr<ace::Mesh> mesh)
{
	ace::Matrix44 mat1, mat2;
	mat1.SetTranslation(0, -0.5, 0);
	mat2.SetTranslation(0, 0.5, 0);

	ace::Matrix44 mat1_inv, mat2_inv;
	mat1_inv = mat1.GetInverted();

	ace::Matrix44::Mul(mat2_inv, mat2, mat1);
	mat2_inv = mat2_inv.GetInverted();

	mesh->AddBoneConnector(0, mat1_inv);
	mesh->AddBoneConnector(1, mat2_inv);
}

static std::shared_ptr<ace::AnimationClip> CreateAnimation()
{
	auto factory = new ace::AnimationSystem_Imp();

	auto clip = factory->CreateAnimationClip();
	auto source = factory->CreateAnimationSource();
	auto key = factory->CreateKeyframeAnimation();

	ace::FCurveKeyframe k1;
	k1.KeyValue.X = 0.0;
	k1.KeyValue.Y = 20;
	k1.LeftHandle = k1.KeyValue;
	k1.RightHandle = k1.KeyValue;

	ace::FCurveKeyframe k2;
	k2.KeyValue.X = 10.0;
	k2.KeyValue.Y = -20;
	k2.LeftHandle = k2.KeyValue;
	k2.RightHandle = k2.KeyValue;

	key->SetName(ace::ToAString("no2.rot.z").c_str());
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

	ace::Log* log = ace::Log_Imp::Create(L"graphics.html", L"メッシュ");

	auto window = ace::Window_Imp::Create(640, 480, ace::ToAString(L"メッシュ").c_str());
	ASSERT_TRUE(window != nullptr);

	auto file = ace::File_Imp::Create();
	ASSERT_TRUE(file != nullptr);

	auto graphics = ace::Graphics_Imp::Create(window, isOpenGLMode ? ace::GraphicsDeviceType::OpenGL : ace::GraphicsDeviceType::DirectX11, log, file, false, false);
	ASSERT_TRUE(graphics != nullptr);

	auto renderer3d = new ace::Renderer3D(graphics, ace::RenderSettings());
	ASSERT_TRUE(renderer3d != nullptr);
	renderer3d->SetWindowSize(ace::Vector2DI(640, 480));

	auto mesh1 = CreateMesh(graphics);
	auto mesh2 = CreateMesh(graphics);
	auto deformer = CreateDeformer(graphics);
	auto animation = CreateAnimation();

	SetMeshBone(mesh2);

	mesh1->SendToGPUMemory();
	mesh2->SendToGPUMemory();

	auto cameraObject = new ace::RenderedCameraObject3D(graphics);
	cameraObject->SetPosition(ace::Vector3DF(0, 0, 10));
	cameraObject->SetFocus(ace::Vector3DF(0, 0, 0));
	cameraObject->SetFieldOfView(20.0f);
	cameraObject->SetZNear(1.0f);
	cameraObject->SetZFar(20.0f);
	cameraObject->SetWindowSize(ace::Vector2DI(800, 600));

	auto meshObject1 = new ace::RenderedModelObject3D(graphics);
	meshObject1->AddMesh(mesh1);
	
	meshObject1->SetPosition(ace::Vector3DF(1, 0, 0));
	meshObject1->SetRotation(ace::Vector3DF(20.0f, 20.0f, 0.0f));

	auto meshObject2 = new ace::RenderedModelObject3D(graphics);
	meshObject2->AddMesh(mesh2);
	meshObject2->SetDeformer(deformer.get());
	meshObject2->SetPosition(ace::Vector3DF(-1, 0, 0));
	meshObject2->SetRotation(ace::Vector3DF(20.0f, 20.0f, 0.0f));
	meshObject2->AddAnimationClip(ace::ToAString("anime1").c_str(), animation.get());
	meshObject2->PlayAnimation(0, ace::ToAString("anime1").c_str());

	auto lightObject = new ace::RenderedDirectionalLightObject3D(graphics);
	lightObject->SetRotation(ace::Vector3DF(30, 160, 0));

	renderer3d->AddObject(cameraObject);
	renderer3d->AddObject(meshObject1);
	renderer3d->AddObject(meshObject2);
	renderer3d->AddObject(lightObject);

	auto renderer2d = new ace::Renderer2D_Imp(graphics, log);


	int32_t time = 0;
	while (window->DoEvent())
	{
		graphics->Begin();
		graphics->Clear(true, false, ace::Color(0, 0, 0, 255));

		renderer3d->Flip(1);

		renderer3d->BeginRendering(1);
		ace::Sleep(100);
		renderer3d->EndRendering();

		graphics->SetRenderTarget(nullptr, nullptr);

		ace::Vector2DF positions[4];
		ace::Color colors[4];
		ace::Vector2DF uvs[4];

		colors[0] = ace::Color(255, 255, 255, 255);
		colors[1] = ace::Color(255, 255, 255, 255);
		colors[2] = ace::Color(255, 255, 255, 255);
		colors[3] = ace::Color(255, 255, 255, 255);

		positions[0].X = 0;
		positions[0].Y = 0;
		positions[1].X = 640;
		positions[1].Y = 0;
		positions[2].X = 640;
		positions[2].Y = 480;
		positions[3].X = 0;
		positions[3].Y = 480;

		uvs[0].X = 0;
		uvs[0].Y = 0;
		uvs[1].X = 1;
		uvs[1].Y = 0;
		uvs[2].X = 1;
		uvs[2].Y = 1;
		uvs[3].X = 0;
		uvs[3].Y = 1;

		renderer2d->AddSprite(positions, colors, uvs, renderer3d->GetRenderTarget(), ace::AlphaBlend::Blend, 0);
		renderer2d->SetArea(ace::RectF(0, 0, 640, 480), 0);
		renderer2d->DrawCache();
		renderer2d->ClearCache();

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

	delete renderer2d;
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

