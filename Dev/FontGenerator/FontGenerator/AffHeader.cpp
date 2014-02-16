#include "AffHeader.h"
#include "utility.h"

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