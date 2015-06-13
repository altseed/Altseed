
#include "../../EngineGraphics3DTest.h"

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

	mesh->AddFace(0, 2, 3, -1);
	mesh->AddFace(0, 3, 1, -1);
	mesh->AddFace(12, 4, 5, -1);
	mesh->AddFace(12, 5, 14, -1);
	mesh->AddFace(16, 6, 7, -1);
	mesh->AddFace(16, 7, 18, -1);
	mesh->AddFace(20, 8, 10, -1);
	mesh->AddFace(20, 10, 22, -1);
	mesh->AddFace(21, 17, 13, -1);
	mesh->AddFace(21, 13, 9, -1);
	mesh->AddFace(11, 15, 19, -1);
	mesh->AddFace(11, 19, 23, -1);

	mesh->SendToGPUMemory();
	return mesh;
}

static std::shared_ptr<asd::AnimationClip> CreateAnimation()
{
	auto factory = asd::Engine::GetAnimationSyatem();

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
	
	return clip;
}

class Graphics_SimpleMesh : public EngineGraphics3DTest
{
public:
	
	Graphics_SimpleMesh(bool isOpenGLMode) :
		EngineGraphics3DTest(asd::ToAString("SimpleMesh"), isOpenGLMode, 15, true)
	{}

protected:

	std::shared_ptr<asd::ModelObject3D> meshObj1;
	float rotation1 = 20.0f;
	float rotation2 = 20.0f;

	void OnStart() override
	{
		asd::RenderSettings settings;
		//settings.IsLightweightMode = true;
		SetRenderSettings(settings);

		EngineGraphics3DTest::OnStart();

		meshObj1 = std::make_shared<asd::ModelObject3D>();
		auto meshObj2 = std::make_shared<asd::ModelObject3D>();
		auto lightObj = std::make_shared<asd::DirectionalLightObject3D>();

		GetLayer3D()->AddObject(meshObj1);
		GetLayer3D()->AddObject(meshObj2);
		GetLayer3D()->AddObject(lightObj);
				
		auto mesh = CreateMesh(asd::Engine::GetGraphics());
		auto mesh2 = CreateMesh(asd::Engine::GetGraphics());
		auto animation = CreateAnimation();

		meshObj1->AddMesh(mesh);
		meshObj1->SetRotation(asd::Vector3DF(rotation1, rotation2, 0.0f));
	
		meshObj2->AddMesh(mesh);
		meshObj2->SetPosition(asd::Vector3DF(0.0f, -5.0f, 0.0f));
		meshObj2->SetScale(asd::Vector3DF(10.0f, 10.0f, 400.0f));

		lightObj->SetRotation(asd::Vector3DF(30, 160, 0));

		SetCameraParameter(10, 15, -15, 1, 2000, 20);
	}

	void OnUpdating()
	{
		EngineGraphics3DTest::OnUpdating();

		meshObj1->SetRotation(asd::Vector3DF(rotation1, rotation2, 0.0f));
		rotation1 += 0.1f;
		rotation2 += 0.3f;
	}
};

ENGINE_TEST(Graphics, SimpleMesh)