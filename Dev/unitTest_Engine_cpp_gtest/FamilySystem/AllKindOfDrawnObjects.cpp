#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class FamilySystem_AllKindOfDrawnObjects : public EngineTest
{
public:
	FamilySystem_AllKindOfDrawnObjects(bool isOpenGLMode)
		: EngineTest(asd::ToAString("AllKindOfDrawnObjects"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
	}

	void OnUpdating()
	{
	}

	void OnUpdated()
	{
	}

	void OnFinish()
	{
	}
};

ENGINE_TEST(FamilySystem, AllKindOfDrawnObjects)