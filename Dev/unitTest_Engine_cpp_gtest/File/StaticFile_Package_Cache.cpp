#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class File_StaticFile_Package_Cache : public EngineTest
{
public:
	File_StaticFile_Package_Cache(bool isOpenGLMode)
		: EngineTest(ace::ToAString("StaticFile_Package_Cache"), isOpenGLMode, 1)
	{

	}

protected:

	void OnStart()
	{
		ace::Engine::GetFile()->AddRootDirectories(ace::ToAString("Data/Texture.pack").c_str());

		for (auto loop = 0; loop < 2; loop++)
		{
			//普通に読み込んだバイナリ
			BinaryReader reader;
			auto data = GetBinaryData(ace::ToAString("Data/Texture/Surface/Tile_Normal.png"));
			reader.ReadIn(data.begin(), data.end());

			//ファイル機能で読み込んだバイナリ
			auto staticFile = ace::Engine::GetFile()->CreateStaticFile(ace::ToAString("Surface/Tile_Normal.png").c_str());
			auto staticFileData = staticFile->ReadAllBytes();

			int cnt = 0;
			while (!reader.IsEmpty())
			{
				int8_t byteFromRaw = reader.Get<int8_t>();

				int8_t byteFromFile = staticFileData[cnt++];

				ASSERT_EQ(byteFromRaw, byteFromFile);
			}

			ASSERT_EQ(cnt, staticFileData.size());
		}
	}
};

ENGINE_TEST(File, StaticFile_Package_Cache)