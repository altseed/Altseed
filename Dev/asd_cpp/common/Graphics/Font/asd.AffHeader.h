#pragma once
#include <string>
#include <iostream>
#include <cstdint>
#include "../../Utility/asd.BinaryWriter.h"
#include "../../Utility/asd.BinaryReader.h"

namespace asd
{
	struct AffHeader
	{
	private:
		static const int GUID_LENGTH = 4;
		static const int AFF_VERSION = 1;
		static const int SHEET_NAME_LENGTH = 64;
		static const int FONT_SIZE_DEFAULT = 16;
		static const int SHEET_COUNT_DEFAULT = 1;
		static const int FONT_COUNT_DEFAULT = 0;

		std::int8_t m_guid[GUID_LENGTH];
		std::int32_t m_version;
		std::int32_t m_fontSize;
		std::int32_t m_fontWidth;
		std::int32_t m_fontHeight;
		std::int32_t m_sheetCount;
		std::int32_t m_fontCount;
		astring m_sheetName;

	public:
		AffHeader(astring sheetName);

		void Push(BinaryWriter& writer);

		/**
			@brief	affファイルのバイナリデータからAffHeaderを復元し、読んだ分だけバイナリデータを削る。
			@param	buffer	バイナリデータ。
		*/
		static AffHeader Get(BinaryReader& reader);

#pragma region GetSet
		int GetFontSize() const;
		void SetFontSize(int value);

		int GetSheetCount() const;
		void SetSheetCount(int value);

		int GetFontCount() const;
		void SetFontCount(int value);
#pragma endregion
	};
}