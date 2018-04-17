#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class IO_StaticFile_Package : public EngineTest
{
public:
	IO_StaticFile_Package(bool isOpenGLMode)
		: EngineTest(asd::ToAString("StaticFile_Package"), isOpenGLMode, 1)
	{

	}

protected:

	void OnStart()
	{
		//普通に読み込んだバイナリ
		BinaryReader reader;
		auto data = GetBinaryData(asd::ToAString("Data/Texture/Surface/Tile_Normal.png"));
		reader.ReadIn(data.begin(), data.end());

		//ファイル機能で読み込んだバイナリ
		asd::Engine::GetFile()->AddRootPackage(asd::ToAString("Data/Texture.pack").c_str());
		auto staticFile = asd::Engine::GetFile()->CreateStaticFile(u"Surface/Tile_Normal.png");
		auto staticFileData = staticFile->GetBuffer();

		int cnt = 0;
		while (!reader.IsEmpty())
		{
			int8_t byteFromRaw = reader.Get<int8_t>();

			int8_t byteFromFile = staticFileData[cnt++];

			ASSERT_EQ(byteFromRaw, byteFromFile);
		}

		ASSERT_EQ(cnt, staticFileData.size());
	}
};

ENGINE_TEST(IO, StaticFile_Package)