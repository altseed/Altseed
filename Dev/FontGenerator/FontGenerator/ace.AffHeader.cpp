#include "ace.AffHeader.h"
#include <Utility/ace.BinaryWriter.h>
#include <Utility/ace.BinaryReader.h>

using namespace std;
using namespace ace;

namespace FontGenerator
{
	AffHeader::AffHeader(wstring sheetName)
		: m_version(AFF_VERSION)
		, m_fontSize(FONT_SIZE_DEFAULT)
		, m_fontWidth(FONT_SIZE_DEFAULT)
		, m_fontHeight(FONT_SIZE_DEFAULT)
		, m_sheetCount(SHEET_COUNT_DEFAULT)
		, m_fontCount(FONT_COUNT_DEFAULT)
		, m_sheetName(sheetName)
	{
		m_guid[0] = 'A';
		m_guid[1] = 'F';
		m_guid[2] = 'F';
		m_guid[3] = '\0';
	}

	void AffHeader::WriteOut(ostream& stream)
	{
		BinaryWriter writer;

		for (int8_t c : m_guid)
		{
			writer.Push(c);
		}

		writer.Push(m_version);
		writer.Push(m_fontSize);
		writer.Push(m_fontWidth);
		writer.Push(m_fontHeight);
		writer.Push(m_sheetCount);
		writer.Push(m_fontCount);
		writer.Push(m_sheetName);

		writer.WriteOut(stream);
	}

	AffHeader AffHeader::ReadIn(vector<int8_t>& buffer)
	{
		BinaryReader reader;
		AffHeader temp(nullptr);

		reader.ReadIn(buffer.begin(), buffer.end());

		temp.m_guid[0] = reader.Get<char>();
		temp.m_guid[1] = reader.Get<char>();
		temp.m_guid[2] = reader.Get<char>();
		temp.m_guid[3] = reader.Get<char>();

		temp.m_version = reader.Get<int>();
		temp.m_fontSize = reader.Get<int>();
		temp.m_fontWidth = reader.Get<int>();
		temp.m_fontHeight = reader.Get<int>();
		temp.m_sheetCount = reader.Get<int>();
		temp.m_fontCount = reader.Get<int>();
		temp.m_sheetName = reader.Get<astring>();

		int count = sizeof(char)* 4 + sizeof(int)* 6 + sizeof(achar)* temp.m_sheetName.length();
		buffer.erase(buffer.begin(), buffer.begin() + count);

		return temp;
	}

#pragma region GetSet
	int AffHeader::GetFontSize() const
	{
		return m_fontSize;
	}

	void AffHeader::SetFontSize(int value)
	{
		m_fontSize = value;
	}

	int AffHeader::GetSheetCount() const
	{
		return m_sheetCount;
	}

	void AffHeader::SetSheetCount(int value)
	{
		m_sheetCount = value;
	}

	int AffHeader::GetFontCount() const
	{
		return m_fontCount;
	}

	void AffHeader::SetFontCount(int value)
	{
		m_fontCount = value;
	}
#pragma endregion
}