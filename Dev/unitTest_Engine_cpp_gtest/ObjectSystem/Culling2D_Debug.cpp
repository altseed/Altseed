#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class ObjectSystem_Culling2D_Debug : public EngineTest
{
	std::shared_ptr<asd::CameraObject2D> cam;
	std::shared_ptr<asd::MapObject2D> map;
	std::shared_ptr<asd::Chip2D> chip;
	float X;
public:
	ObjectSystem_Culling2D_Debug(bool isOpenGLMode)
		: EngineTest(asd::ToAString("ObjectSystem_Culling2D_Debug"), isOpenGLMode, 30, 960, 640)
	{
	}

protected:
	void OnStart()
	{
		cam = std::make_shared<asd::CameraObject2D>();
		cam->SetDst(asd::RectI(0, 0, 960, 640));
		cam->SetSrc(asd::RectI(530, 0, 480, 320));
		asd::Engine::AddObject2D(cam);

		map = std::make_shared<asd::MapObject2D>();
		chip = std::make_shared<asd::Chip2D>();
		chip->SetTexture(asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/texture.png").c_str()));
		chip->SetPosition(asd::Vector2DF(620.0f, 0.0f));
		map->AddChip(chip);
		asd::Engine::AddObject2D(map);

		X = 1200.0f;
	}

	void OnUpdating()
	{
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::Right) == asd::KeyState::Hold)
			X += 1.0f;
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::Left) == asd::KeyState::Hold)
			X -= 1.0f;

		cam->SetSrc(asd::RectI((int)X - 240, 0, 480, 320));
		chip->SetPosition(asd::Vector2DF(X * 0.8f, 0.0f));

		//System.Console.WriteLine("{0}:{1} {2}", X, X - 240, X * 0.8);
	}
};

ENGINE_TEST(ObjectSystem, Culling2D_Debug)
