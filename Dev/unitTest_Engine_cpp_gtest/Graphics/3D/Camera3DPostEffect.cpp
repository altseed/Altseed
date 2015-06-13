
#include "../../EngineTest.h"

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

class Graphics_Camera3DPostEffect : public EngineTest
{
public:

	Graphics_Camera3DPostEffect(bool isOpenGLMode) :
		EngineTest(asd::ToAString("Camera3DPostEffect"), isOpenGLMode, 20)
	{}

protected:
	void OnStart() override
	{
		auto scene = std::make_shared<asd::Scene>();
		auto layer = std::make_shared<asd::Layer3D>();
		auto meshObj = std::make_shared<asd::ModelObject3D>();
		auto lightObj = std::make_shared<asd::DirectionalLightObject3D>();
		auto cameraObj = std::make_shared<asd::CameraObject3D>();

		scene->AddLayer(layer);
		layer->AddObject(meshObj);
		layer->AddObject(lightObj);
		layer->AddObject(cameraObj);
		asd::Engine::ChangeScene(scene);


		auto mesh = CreateMesh(asd::Engine::GetGraphics());

		cameraObj->SetPosition(asd::Vector3DF(0, 0, 10));
		cameraObj->SetFocus(asd::Vector3DF(0, 0, 0));
		cameraObj->SetFieldOfView(20.0f);
		cameraObj->SetZNear(1.0f);
		cameraObj->SetZFar(20.0f);
		cameraObj->SetWindowSize(asd::Vector2DI(800, 600));

		meshObj->AddMesh(mesh);
		meshObj->SetRotation(asd::Vector3DF(20.0f, 20.0f, 0.0f));

		lightObj->SetRotation(asd::Vector3DF(30, 160, 0));

		auto pe = std::make_shared<asd::PostEffectGaussianBlur>();
		pe->SetIntensity(100.0f);
		cameraObj->AddPostEffect(pe);
	}
};


TEST(Graphics, Camera3DPostEffect_GL)
{
	Graphics_Camera3DPostEffect(true).Run();
	AssertMemoryDoesntLeak();
}

#if _WIN32
TEST(Graphics, Camera3DPostEffect_DX)
{
	Graphics_Camera3DPostEffect(false).Run();
	AssertMemoryDoesntLeak();
}
#endif

void Graphics_Camera3DPostEffect_(bool isGL)
{
	Graphics_Camera3DPostEffect(isGL).Run();
	AssertMemoryDoesntLeak();
}
