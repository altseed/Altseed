#pragma once
#include <map>
#include <cstdint>
#include "../../Utility/ace.BinaryWriter.h"
#include "../../Utility/ace.BinaryReader.h"

namespace ace
{
	class AffIndexTable
	{
	private:
		std::map<achar, std::int16_t> m_indexes;

	public:
		static const int CHARCODE_MAX = 65536;

		AffIndexTable();

		void AppendFontIndex(achar charactor, std::int16_t index);

		const std::map<achar, std::int16_t>& GetIndexes() const;

		void Push(BinaryWriter& writer);

		static AffIndexTable Get(BinaryReader& reader);
	};
}