

#include "../EngineTest.h"

using namespace std;
using namespace asd;

class Sound_Sound : public EngineTest
{
public:
	Sound_Sound(bool isOpenGLMode)
		: EngineTest(asd::ToAString("Sound"), isOpenGLMode, 10)
	{
	}

protected:
	void OnStart()
	{
		// ファイルを読み込む。
		std::shared_ptr<asd::SoundSource> se1 = asd::Engine::GetSound()->CreateSoundSource(asd::ToAString("Data/Sound/se1.wav").c_str(), true);
		std::shared_ptr<asd::SoundSource> bgm1 = asd::Engine::GetSound()->CreateSoundSource(asd::ToAString("Data/Sound/bgm1.ogg").c_str(), false);

		// 再生する。
		int id_se1 = asd::Engine::GetSound()->Play(se1);
		int id_bgm1 = asd::Engine::GetSound()->Play(bgm1);
	}


};

ENGINE_TEST(Sound, Sound)
