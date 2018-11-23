
#include <gtest/gtest.h>
#include <Altseed.h>
#include "../../EngineTest.h"

using namespace std;
using namespace asd;

class Graphics_MediaPlayer : public EngineTest
{
public:
	Graphics_MediaPlayer(bool isOpenGLMode)
		: EngineTest(asd::ToAString("MediaPlayer"), isOpenGLMode, 20)
	{
	}

private:
	std::shared_ptr<asd::Texture2D> texture;
	std::shared_ptr<asd::MediaPlayer> mp;
protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto textureObj = make_shared<TextureObject2D>();
		texture = asd::Engine::GetGraphics()->CreateEmptyTexture2D(640, 480, asd::TextureFormat::R8G8B8A8_UNORM_SRGB);

		layer->AddObject(textureObj);

		scene->AddLayer(layer);
		asd::Engine::ChangeScene(scene);

		textureObj->SetTexture(texture);

		mp = asd::Engine::GetGraphics()->CreateMediaPlayer();
		mp->Load(u"Data/Movie/Sample1.mp4");
		mp->Play(false);
	}

	void OnUpdating()
	{
		mp->WriteToTexture2D(texture);
	}
};


ENGINE_TEST(Graphics, MediaPlayer)
