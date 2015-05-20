#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class IO_StaticFile_PackageWithKey : public EngineTest
{
public:
	IO_StaticFile_PackageWithKey(bool isOpenGLMode)
		: EngineTest(ace::ToAString("StaticFile_PackageWithKey"), isOpenGLMode, 1)
	{

	}

protected:

	void OnStart()
	{
		//普通に読み込んだバイナリ
		BinaryReader reader;
		auto data = GetBinaryData(ace::ToAString("Data/Texture/Surface/Tile_Normal.png"));
		reader.ReadIn(data.begin(), data.end());

		//ファイル機能で読み込んだバイナリ
		ace::Engine::GetFile()->AddRootPackageWithPassword(ace::ToAString("Data/Texture3.pack").c_str(), ace::ToAString("pack3").c_str());
		auto staticFile = ace::Engine::GetFile()->CreateStaticFile(ace::ToAString("Surface/Tile_Normal.png").c_str());
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

ENGINE_TEST(IO, StaticFile_PackageWithKey)