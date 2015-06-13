

#include "../../EngineGraphics3DTest.h"

class Graphics_DrawSpriteAdditionally3D : public EngineGraphics3DTest
{
public:

	Graphics_DrawSpriteAdditionally3D(bool isOpenGLMode) :
		EngineGraphics3DTest(asd::ToAString("DrawSpriteAdditionally3D"), isOpenGLMode, 10, true)
	{}

protected:
	void OnStart() override
	{
		EngineGraphics3DTest::OnStart();	
	}

	void OnUpdating() override
	{
		EngineGraphics3DTest::OnUpdating();

		GetLayer3D()->DrawSpriteAdditionally(
			asd::Vector3DF(-1, 0, -1),
			asd::Vector3DF(1, 0, -1),
			asd::Vector3DF(1, 0, 1),
			asd::Vector3DF(-1, 0, 1),
			asd::Color(255, 0, 0, 255),
			asd::Color(0, 255, 0, 255),
			asd::Color(0, 0, 255, 255),
			asd::Color(255, 255, 255, 255),
			asd::Vector2DF(0, 0),
			asd::Vector2DF(0, 0),
			asd::Vector2DF(0, 0),
			asd::Vector2DF(0, 0),
			nullptr,
			asd::AlphaBlendMode::Blend,
			false,
			true
			);
	}
};

ENGINE_TEST(Graphics, DrawSpriteAdditionally3D)