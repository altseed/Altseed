#include "../../PCH/asd.UnitTestCpp.PCH.h"
#include <Graphics/Font/asd.AffIndexTable.h>
#include <memory>
#include <cstdint>

using namespace std;
using namespace asd;

void Font_IndexTable()
{
	static astring fileName = ToAString(u"test.aff");

	AffIndexTable table;
	table.AppendFontIndex(L'N', 0);
	table.AppendFontIndex(L'A', 1);
	table.AppendFontIndex(L'C', 2);

	BinaryWriter writer;
	table.Push(writer);
	ofstream out(ToUtf8String(fileName.c_str()).c_str(), ios::binary);
	writer.WriteOut(out);
	out.close();

	auto data = asd::GetBinaryData(fileName);
	BinaryReader reader;
	reader.ReadIn(data.begin(), data.end());
	auto result = AffIndexTable::Get(reader).GetIndexes();

	ASSERT_EQ(0, result[L'N']);
	ASSERT_EQ(1, result[L'A']);
	ASSERT_EQ(2, result[L'C']);

	// Linux対策
	auto NONAVAILABLE = AffIndexTable::NONAVAILABLE;
	ASSERT_EQ(NONAVAILABLE, result[L'X']);
}

TEST(Font, IndexTableIO)
{
	Font_IndexTable();
}