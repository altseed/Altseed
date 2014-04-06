#include <array>
#include "ace.AffIndexTable.h"

using namespace std;

namespace ace
{
	AffIndexTable::AffIndexTable()
		: m_indexes(map<achar, int16_t>())
	{
	}

	void AffIndexTable::AppendFontIndex(achar charactor, std::int16_t index)
	{
		m_indexes[charactor] = index;
	}

	const map<achar, int16_t>& AffIndexTable::GetIndexes() const
	{
		return m_indexes;
	}

	void AffIndexTable::Push(BinaryWriter& writer)
	{
		array<int16_t, CHARCODE_MAX> indexes;

		for (auto& x : m_indexes)
		{
			indexes[(int)(x.first)] = x.second;
		}

		for (auto& x : indexes)
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