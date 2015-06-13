#pragma once
#include <vector>
#include <asd.common.Base.h>
#include "ResultOfGeneratingPng.h"
#include "Rendering/Font.h"
#include "Rendering/Setting.h"
#include "Rendering/SettingForRendering.h"

namespace FontGenerator
{
	class PngGenerator
	{
	private:
		asd::astring m_sheetName;
		int m_sheetSize;
		SettingForRendering m_setting;

		std::vector<FontData> m_fonts;

		std::vector<asd::achar> GetCharactors(asd::astring textPath);

	public:
		PngGenerator();

		ResultOfGeneratingPng Generate(
			asd::astring fontPath,
			std::vector<asd::achar>& charactors);

		void GeneratePreview(asd::astring fontPath, asd::astring path);

#pragma region GetSet
		asd::astring GetSheetName() const;
		void SetSheetName(asd::astring value);

		int GetSheetSize() const;
		void SetSheetSize(int value);

		SettingForRendering GetSetting() const;
		void SetSetting(SettingForRendering value);

#pragma endregion
	};
}