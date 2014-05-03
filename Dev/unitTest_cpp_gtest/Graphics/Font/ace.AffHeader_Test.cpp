#include "../../PCH/ace.UnitTestCpp.PCH.h"
#include <Graphics/Font/ace.AffHeader.h>
#include <memory>
#include <cstdint>

using namespace std;
using namespace ace;

static void Font_AffHeader()
{
	static astring fileName = ToAString(L"test.aff");

	auto header = make_shared<AffHeader>(ToAString("test"));
	header->SetFontSize(10);
	header->SetFontCount(20);
	header->SetSheetCount(2);

	BinaryWriter writer;
	header->Push(writer);

	ofstream out(ToUtf8String(fileName.c_str()).c_str(), ios::binary);
	writer.WriteOut(out);
	out.close();

	auto data = ace::GetBinaryData(fileName);
	BinaryReader reader;
	reader.ReadIn(data.begin(), data.end());

	auto result = AffHeader::Get(reader);

	ASSERT_EQ(header->GetFontSize(), result.GetFontSize());
	ASSERT_EQ(header->GetFontCount(), result.GetFontCount());
	ASSERT_EQ(header->GetSheetCount(), result.GetSheetCount());
}

TEST(Font, AffHeaderIO)
{
	Font_AffHeader();
}