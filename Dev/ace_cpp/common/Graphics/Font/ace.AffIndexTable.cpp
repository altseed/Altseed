#include <array>
#include "ace.AffIndexTable.h"

using namespace std;

namespace ace
{
	AffIndexTable::AffIndexTable()
	{
		for (int i = 0; i < CHARCODE_MAX; ++i)
		{
			m_indexes[i] = NONAVAILABLE;
		}
	}

	void AffIndexTable::AppendFontIndex(achar charactor, std::int16_t index)
	{
		m_indexes[charactor] = index;
	}

	array<int16_t, AffIndexTable::CHARCODE_MAX>& AffIndexTable::GetIndexes()
	{
		return m_indexes;
	}

	void AffIndexTable::Push(BinaryWriter& writer)
	{
		for (auto& x : m_indexes)
		{
			writer.Push(x);
		}
	}

	AffIndexTable AffIndexTable::Get(BinaryReader& reader)
	{
		AffIndexTable result;

		for (int i = 0; i < CHARCODE_MAX; ++i)
		{
			result.m_indexes[(achar)i] = reader.Get<int16_t>();
		}

		return result;
	}
}