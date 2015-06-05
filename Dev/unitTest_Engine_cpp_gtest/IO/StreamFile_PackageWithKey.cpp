#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class IO_StreamFile_PackageWithKey
	: public EngineTest
{
public:
	IO_StreamFile_PackageWithKey(bool isOpenGLMode)
		: EngineTest(ace::ToAString("StreamFile_PackageWithKey"), isOpenGLMode, 1)
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
		ace::Engine::GetFile()->AddRootPackageWithPassword(ace::ToAString("Data/Texture3_new.pack").c_str(), ace::ToAString("pack3").c_str());
		auto streamFile = ace::Engine::GetFile()->CreateStreamFile(ace::ToAString("Surface/Tile_Normal.png").c_str());
		
		std::vector<uint8_t> buffer;
		streamFile->Read(buffer, streamFile->GetSize());

		int cnt = 0;
		while (!reader.IsEmpty())
		{
			auto byteFromRaw = reader.Get<uint8_t>();

			auto byteFromFile = buffer[cnt++];

			ASSERT_EQ(byteFromRaw, byteFromFile);
		}

		ASSERT_EQ(cnt, buffer.size());
	}
};

ENGINE_TEST(IO, StreamFile_PackageWithKey)