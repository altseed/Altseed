#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../../EngineTest.h"


class Graphics_PostEffectSepia : public EngineTest
{
public:
	float intensity;
	Graphics_PostEffectSepia(bool isOpenGLMode) :
		EngineTest(ace::ToAString("PostEffectSepia"), isOpenGLMode, 10)
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

		auto pe = std::make_shared<ace::PostEffectSepia>(g);
		layer->AddPostEffect(pe);
	}
};


TEST(Graphics, PostEffectSepia_GL)
{
	Graphics_PostEffectSepia(true).Run();

}

#if _WIN32
TEST(Graphics, PostEffectSepia_DX)
{
	Graphics_PostEffectSepia(false).Run();
}
#endif

