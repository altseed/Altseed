#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../../EngineTest.h"


class Graphics_PostEffectGaussianBlur : public EngineTest
{
public:
	float intensity;
	Graphics_PostEffectGaussianBlur(bool isOpenGLMode, float const intensity_ = 5.0f) :
		EngineTest(ace::ToAString("PostEffectGaussianBlur"), isOpenGLMode, 60),
		intensity(intensity_)
	{}
protected:
	void OnStart() override
	{
		auto engine = ace::GetEngine();

		auto scene = std::make_shared<ace::Scene>();
		auto layer = std::make_shared<ace::Layer2D>();
		auto object = std::make_shared<ace::TextureObject2D>();
		scene->AddLayer(layer);
		layer->AddObject(object);
		engine->ChangeScene(scene);

		auto g = ace::GetGraphics();
		auto texture = g->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());
		object->SetTexture(texture);
		object->SetScale(ace::Vector2DF(2, 2));

		auto pe = std::make_shared<ace::PostEffectGaussianBlur>(g);
		pe->SetIntensity(intensity);
		layer->AddPostEffect(pe);
	}


	
};


TEST(Graphics, PostEffectGaussianBlur_GL)
{
	Graphics_PostEffectGaussianBlur(true, 1.0f).Run();
	Graphics_PostEffectGaussianBlur(true, 3.0f).Run();
	Graphics_PostEffectGaussianBlur(true).Run();
	Graphics_PostEffectGaussianBlur(true, 7.0f).Run();
	Graphics_PostEffectGaussianBlur(true, 100.0f).Run();
	
}

#if _WIN32
TEST(Graphics, PostEffectGaussianBlur_DX)
{
	Graphics_PostEffectGaussianBlur(false, 1.0f).Run();
	Graphics_PostEffectGaussianBlur(false, 3.0f).Run();
	Graphics_PostEffectGaussianBlur(false).Run();
	Graphics_PostEffectGaussianBlur(false, 7.0f).Run();
	Graphics_PostEffectGaussianBlur(false, 100.0f).Run();
}
#endif

