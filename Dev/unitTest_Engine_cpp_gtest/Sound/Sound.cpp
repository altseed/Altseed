

#include "../EngineTest.h"

using namespace std;
using namespace ace;

class Sound_Sound : public EngineTest
{
public:
	Sound_Sound(bool isOpenGLMode)
		: EngineTest(ace::ToAString("Sound"), isOpenGLMode, 10)
	{
	}

protected:
	void OnStart()
	{
		// ファイルを読み込む。
		std::shared_ptr<ace::SoundSource> se1 = ace::Engine::GetSound()->CreateSoundSource(ace::ToAString("Data/Sound/se1.wav").c_str(), true);
		std::shared_ptr<ace::SoundSource> bgm1 = ace::Engine::GetSound()->CreateSoundSource(ace::ToAString("Data/Sound/bgm1.ogg").c_str(), false);

		// 再生する。
		int id_se1 = ace::Engine::GetSound()->Play(se1);
		int id_bgm1 = ace::Engine::GetSound()->Play(bgm1);
	}


};

ENGINE_TEST(Sound, Sound)
