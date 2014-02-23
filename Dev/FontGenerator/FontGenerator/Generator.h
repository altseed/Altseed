#pragma once
#include <string>
#include <vector>
#include "../../ace_cpp/common/ace.common.Base.h"
#include "Rendering/SettingForRendering.h"

namespace FontGenerator
{
	class Generator
	{
	private:


	public:
		void GenerateFontFile(
			std::wstring fontPath,
			std::wstring textPath,
			std::wstring sheetName,
			SettingForRendering setting);

		std::vector<ace::achar> GetCharactors(ace::astring textPath);
	};
}