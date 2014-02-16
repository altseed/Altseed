#pragma once
#include <string>
#include <vector>
#include "../../ace_cpp/common/ace.common.Base.h"

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

		std::vector<ace::achar> GetCharactors(ace::astring textPath);
	};
}