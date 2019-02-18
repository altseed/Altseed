#include "../EngineTest.h"
#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>

using namespace std;
using namespace asd;

class IO_StreamFile_PackageWithKey : public EngineTest
{
public:
	IO_StreamFile_PackageWithKey(bool isOpenGLMode) : EngineTest(u"StreamFile_PackageWithKey", isOpenGLMode, 1) {}

protected:
	void OnStart()
	{
		//普通に読み込んだバイナリ
		BinaryReader reader;
		auto data = GetBinaryData(u"Data/Texture/Surface/Tile_Normal.png");
		reader.ReadIn(data.begin(), data.end());

		//ファイル機能で読み込んだバイナリ
		asd::Engine::GetFile()->AddRootPackageWithPassword(u"Data/Texture3_new.pack", u"pack3");
		auto streamFile = asd::Engine::GetFile()->CreateStreamFile(u"Surface/Tile_Normal.png");

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