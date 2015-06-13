#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class IO_StaticFile_Package_Priority : public EngineTest
{
public:
	IO_StaticFile_Package_Priority(bool isOpenGLMode)
		: EngineTest(asd::ToAString("StaticFile_Package_Priority"), isOpenGLMode, 1)
	{

	}

protected:

	void OnStart()
	{
		{
			//普通に読み込んだバイナリ
			BinaryReader reader1;
			auto data1 = GetBinaryData(asd::ToAString("Data/Texture/Cloud1.png"));
			reader1.ReadIn(data1.begin(), data1.end());

			//ファイル機能で読み込んだバイナリ
			asd::Engine::GetFile()->AddRootDirectory(asd::ToAString("Data/Texture2.pack").c_str());
			asd::Engine::GetFile()->AddRootDirectory(asd::ToAString("Data/Texture.pack").c_str());
			auto staticFile = asd::Engine::GetFile()->CreateStaticFile(asd::ToAString("Cloud1.png").c_str());
			auto staticFileData = staticFile->GetBuffer();

			int cnt = 0;
			while (!reader1.IsEmpty())
			{
				int8_t byteFromRaw = reader1.Get<int8_t>();

				int8_t byteFromFile = staticFileData[cnt++];

				ASSERT_EQ(byteFromRaw, byteFromFile);
			}
			ASSERT_EQ(cnt, staticFileData.size());

			asd::Engine::GetFile()->ClearRootDirectories();
		}
		{
			//普通に読み込んだバイナリ
			BinaryReader reader2;
			auto data2 = GetBinaryData(asd::ToAString("Sample1.png"));
			reader2.ReadIn(data2.begin(), data2.end());

			//ファイル機能で読み込んだバイナリ
			asd::Engine::GetFile()->AddRootDirectory(asd::ToAString("Data/Texture.pack").c_str());
			asd::Engine::GetFile()->AddRootDirectory(asd::ToAString("Data/Texture2.pack").c_str());
			auto staticFile = asd::Engine::GetFile()->CreateStaticFile(asd::ToAString("Cloud1.png").c_str());
			auto staticFileData = staticFile->GetBuffer();

			int cnt = 0;
			while (!reader2.IsEmpty())
			{
				int8_t byteFromRaw = reader2.Get<int8_t>();

				int8_t byteFromFile = staticFileData[cnt++];

				ASSERT_EQ(byteFromRaw, byteFromFile);
			}
			//ASSERT_EQ(cnt, staticFileData.size());
		}
	}
};

ENGINE_TEST(IO, StaticFile_Package_Priority)