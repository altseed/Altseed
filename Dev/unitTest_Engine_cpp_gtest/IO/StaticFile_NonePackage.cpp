#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;
class IO_StaticFile_NonePackage : public EngineTest
{
public:
	IO_StaticFile_NonePackage(bool isOpenGLMode)
		: EngineTest(asd::ToAString("StaticFile_NonePackage"), isOpenGLMode, 1)
	{
		
	}

protected:

	void OnStart()
	{
		//普通に読み込んだバイナリ
		BinaryReader reader;
		auto data = GetBinaryData(asd::ToAString("Data/Texture/../Texture/Sample1.png"));
		reader.ReadIn(data.begin(), data.end());

		//ファイル機能で読み込んだバイナリ
		asd::Engine::GetFile()->AddRootDirectory(asd::ToAString("Data/Texture").c_str());
		auto staticFile = asd::Engine::GetFile()->CreateStaticFile(asd::ToAString("../Texture/Sample1.png").c_str());
		auto staticFileData = staticFile->GetBuffer();

		int cnt = 0;
		while (!reader.IsEmpty())
		{
			int8_t byteFromRaw = reader.Get<int8_t>();

			int8_t byteFromFile = staticFileData[cnt++];

			ASSERT_EQ(byteFromRaw,byteFromFile);
		}

		ASSERT_EQ(cnt, staticFileData.size());
	}
};

ENGINE_TEST(IO, StaticFile_NonePackage)
