#pragma once
#include <string>
#include <iostream>
#include <cstdint>
#include "../../ace_cpp/core

namespace FontGenerator
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
		int m_version;
		int m_fontSize;
		int m_fontWidth;
		int m_fontHeight;
		int m_sheetCount;
		int m_fontCount;
		ace::astring m_sheetName;

	public:
		AffHeader(ace::astring sheetName);

		void WriteOut(std::ostream& stream);

		/**
			@brief	affファイルのバイナリデータからAffHeaderを復元し、読んだ分だけバイナリデータを削る。
			@param	buffer	バイナリデータ。
		*/
		AffHeader ReadIn(std::vector<int8_t>& buffer);

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