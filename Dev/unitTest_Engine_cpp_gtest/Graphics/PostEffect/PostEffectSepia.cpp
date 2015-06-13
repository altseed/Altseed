#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../../EngineTest.h"


class Graphics_PostEffectSepia : public EngineTest
{
public:
	float intensity;
	Graphics_PostEffectSepia(bool isOpenGLMode) :
		EngineTest(asd::ToAString("PostEffectSepia"), isOpenGLMode, 10)
	{}
protected:
	void OnStart() override
	{
		auto scene = std::make_shared<asd::Scene>();
		auto layer = std::make_shared<asd::Layer2D>();
		auto obj = std::make_shared<asd::TextureObject2D>();
		scene->AddLayer(layer);
		layer->AddObject(obj);
		asd::Engine::ChangeScene(scene);

		auto g = asd::Engine::GetGraphics();
		auto texture = g->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());
		obj->SetTexture(texture);
		obj->SetScale(asd::Vector2DF(2, 2));

		auto pe = std::make_shared<asd::PostEffectSepia>();
		layer->AddPostEffect(pe);
	}
};


ENGINE_TEST(Graphics, PostEffectSepia)
