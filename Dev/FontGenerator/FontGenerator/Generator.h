#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "../../ace_cpp/common/ace.common.Base.h"
#include "Rendering/SettingForRendering.h"
#include "PngGenerator.h"

namespace FontGenerator
{
	class Generator
	{
	private:
		const int INDEX_MAX = 65536;

	public:
		void GenerateFontFile(
			std::wstring fontPath,
			std::wstring textPath,
			std::wstring sheetName,
			SettingForRendering setting);

		std::vector<ace::achar> GetCharactors(ace::astring textPath);

		void WriteIndexTableOut(ResultOfGeneratingPng& result, std::ostream& stream);

		void WriteFontBalkOut(ResultOfGeneratingPng& result, std::ostream& stream);
	};
}