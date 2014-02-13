#pragma once
#include <array>
#include <vector>
#include <string>

namespace FontGenerator
{
	struct AffHeader
	{
	private:
		static const int GUID_LENGTH = 4;
		static const int AFF_VERSION = 1;
		static const int SHEET_NAME_LENGTH = 64;

	public:
		AffHeader(int fontSize, std::wstring sheetName);

		std::array<char, GUID_LENGTH> guid;
		int version;
		int fontSize;
		int fontWidth;
		int fontHeight;
		int sheetCount;
		int fontCount;
		std::array<wchar_t, SHEET_NAME_LENGTH> sheetName;

		std::vector<char> GetBytes();
	};
}