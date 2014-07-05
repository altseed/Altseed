#pragma once
#include <map>
#include <cstdint>
#include "../../Utility/ace.BinaryWriter.h"
#include "../../Utility/ace.BinaryReader.h"

namespace ace
{
	class AffIndexTable
	{
	public:
		static const int CHARCODE_MAX = 65536;
		static const int16_t NONAVAILABLE = 0xff;

	private:
		std::array<int16_t, CHARCODE_MAX> m_indexes;

	public:
		AffIndexTable();

		void AppendFontIndex(achar charactor, std::int16_t index);

		std::array<int16_t, CHARCODE_MAX>& GetIndexes();

		void Push(BinaryWriter& writer);

		static AffIndexTable Get(BinaryReader& reader);
	};
}