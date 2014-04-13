#include "../../PCH/ace.UnitTestCpp.PCH.h"
#include <Graphics/Font/ace.AffIndexTable.h>
#include <memory>
#include <cstdint>

using namespace std;
using namespace ace;

void Font_IndexTable()
{
	static wstring fileName = L"test.aff";

	AffIndexTable table;
	table.AppendFontIndex(L'N', 0);
	table.AppendFontIndex(L'A', 1);
	table.AppendFontIndex(L'C', 2);

	BinaryWriter writer;
	table.Push(writer);
	ofstream out(fileName, ios::binary);
	writer.WriteOut(out);
	out.close();

	auto data = ace::GetBinaryData(fileName);
	BinaryReader reader;
	reader.ReadIn(data.begin(), data.end());
	auto result = AffIndexTable::Get(reader).GetIndexes();

	ASSERT_EQ(0, result[L'N']);
	ASSERT_EQ(1, result[L'A']);
	ASSERT_EQ(2, result[L'C']);
	ASSERT_EQ(AffIndexTable::NONAVAILABLE, result[L'X']);
}

TEST(Font, IndexTableIO)
{
	Font_IndexTable();
}