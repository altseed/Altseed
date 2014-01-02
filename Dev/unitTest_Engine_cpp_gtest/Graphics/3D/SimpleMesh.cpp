
#include "../../EngineTest.h"

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
	return mesh;
}

static std::shared_ptr<ace::Deformer> CreateDeformer(ace::Graphics* graphics)
{
	auto deformer = graphics->CreateDeformer();

	ace::Matrix44 mat1, mat2;
	mat1.Translation(0, -0.5, 0);
	mat2.Translation(0, 0.5, 0);

	ace::Matrix44 mat1_inv, mat2_inv;
	mat1.Inverse(mat1_inv, mat1);

	ace::Matrix44::Mul(mat2_inv, mat2, mat1);
	ace::Matrix44::Inverse(mat2_inv, mat2_inv);

	deformer->AddBone(ace::ToAString("no1").c_str(), -1, ace::eRotationOrder::ROTATION_ORDER_ZXY, mat1, mat1_inv);
	deformer->AddBone(ace::ToAString("no2").c_str(), 0, ace::eRotationOrder::ROTATION_ORDER_ZXY, mat2, mat2_inv);

	return deformer;
}

static std::shared_ptr<ace::AnimationClip> CreateAnimation()
{
	auto factory = ace::GetAnimationSyatem();

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
	
	return clip;
}

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

		
		auto mesh = CreateMesh(ace::GetGraphics());
		auto deformer = CreateDeformer(ace::GetGraphics());
		auto animation = CreateAnimation();

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
