#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class IO_StreamFile_Package 
	: public EngineTest
{
public:
	IO_StreamFile_Package(bool isOpenGLMode)
		: EngineTest(asd::ToAString("StreamFile_Package"), isOpenGLMode, 1)
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
		auto streamFile = asd::Engine::GetFile()->CreateStreamFile(asd::ToAString("Surface/Tile_Normal.png").c_str());
		
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

ENGINE_TEST(IO, StreamFile_Package)