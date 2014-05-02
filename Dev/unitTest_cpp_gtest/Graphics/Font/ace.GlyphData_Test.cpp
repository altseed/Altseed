#include "../../PCH/ace.UnitTestCpp.PCH.h"
#include <Graphics/Font/ace.GlyphData.h>
#include <memory>
#include <cstdint>

using namespace std;
using namespace ace;

void AssertGlyphData(GlyphData& expected, GlyphData& actual)
{
	ASSERT_EQ(expected.GetCharactor(), actual.GetCharactor());
	ASSERT_EQ(expected.GetSheetNum(), actual.GetSheetNum());
	//ASSERT_EQ(expected.GetSrc(), actual.GetSrc());
	ASSERT_TRUE(expected.GetSrc() == actual.GetSrc());
}

void Font_GlyphDataSerialize()
{
	static wstring fileName = L"test.aff";
	static int count = 3;

	vector<GlyphData> glyphs;
	wstring str = L"NumAni";
	for (int i = 0; i < count; ++i)
	{
		RectI rect(i*20, 0, 20, 40);
		glyphs.push_back(GlyphData(str[i], 0, rect));
	}

	BinaryWriter writer;
	for (auto& x : glyphs)
	{
		x.Push(writer);
	}
	ofstream out(fileName, ios::binary);
	writer.WriteOut(out);
	out.close();

	BinaryReader reader;
	auto data = ace::GetBinaryData(fileName);
	reader.ReadIn(data.begin(), data.end());

	for (int i = 0; i < count; ++i)
	{
		auto g = GlyphData::Get(reader, str[i]);
		AssertGlyphData(glyphs[i], g);
	}
}

TEST(Font, GlyphDataSerialize)
{
	Font_GlyphDataSerialize();
}