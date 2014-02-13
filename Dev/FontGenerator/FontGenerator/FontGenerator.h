#pragma once
#include <string>

namespace FontGenerator
{
	class FontGenerator
	{
	public:
		void GenerateFontFile(
			std::wstring fontPath,
			int fontSize,
			std::wstring textPath,
			std::wstring sheetName);
	};
}