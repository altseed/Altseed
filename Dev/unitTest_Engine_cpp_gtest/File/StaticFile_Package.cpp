#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class File_StaticFile_Package : public EngineTest
{
public:
	File_StaticFile_Package(bool isOpenGLMode)
		: EngineTest(ace::ToAString("StaticFile_Package"), isOpenGLMode, 1)
	{

	}

protected:

	void OnStart()
	{
		//普通に読み込んだバイナリ
		BinaryReader reader;
		auto data = GetBinaryData(ace::ToAString("Data/Texture/Sample1.png"));
		reader.ReadIn(data.begin(), data.end());

		//ファイル機能で読み込んだバイナリ
		ace::Engine::GetFile()->AddRootDirectories(ace::ToAString("Data/Texture.pack").c_str());
		auto staticFile = ace::Engine::GetFile()->CreateStaticFile(ace::ToAString("Sample1.png").c_str());
		auto staticFileData = staticFile->ReadAllBytes();

		int cnt = 0;
		while (!reader.IsEmpty())
		{
			int8_t byteFromRaw = reader.Get<int8_t>();

			int8_t byteFromFile = staticFileData[cnt++];

			ASSERT_EQ(byteFromRaw, byteFromFile);
		}
	}
};

ENGINE_TEST(File, StaticFile_Package)