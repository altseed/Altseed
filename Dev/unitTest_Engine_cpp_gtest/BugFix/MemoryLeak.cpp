#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class BugFix_MemoryLeak : public EngineTest
{
public:
	BugFix_MemoryLeak(bool isOpenGLMode)
		: EngineTest(asd::ToAString("MemoryLeak"), isOpenGLMode, 60)
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

ENGINE_TEST(BugFix, MemoryLeak)