#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class File_StaticFile_NonePackage : public EngineTest
{
public:
	File_StaticFile_NonePackage(bool isOpenGLMode)
		: EngineTest(ace::ToAString("StaticFile_NonePackage"), isOpenGLMode, 120)
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

		while (!reader.IsEmpty())
		{
			int8_t byteFromRaw = reader.Get<int8_t>();

			int8_t byteFromFile = 0; //ファイルから逐次読み込んで代入する

			if (byteFromRaw != byteFromFile)
			{
				printf("Reading failed!!\n");
				return;
			}
		}


		printf("Successfully read!!");
	}
};

ENGINE_TEST(File, StaticFile_NonePackage)
