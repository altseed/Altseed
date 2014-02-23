#pragma once
#include <vector>
#include <ace.common.Base.h>
#include "ResultOfGeneratingPng.h"
#include "Rendering/Font.h"
#include "Rendering/Setting.h"
#include "Rendering/SettingForRendering.h"

namespace FontGenerator
{
	class PngGenerator
	{
	private:
		ace::astring m_sheetName;
		int m_sheetSize;
		SettingForRendering m_setting;

		std::vector<FontData> m_fonts;

		std::vector<ace::achar> GetCharactors(ace::astring textPath);

	public:
		PngGenerator();

		ResultOfGeneratingPng Generate(
			ace::astring fontPath,
			std::vector<ace::achar>& textPath);

#pragma region GetSet
		ace::astring GetSheetName() const;
		void SetSheetName(ace::astring value);

		int GetSheetSize() const;
		void SetSheetSize(int value);

		SettingForRendering GetSetting() const;
		void SetSetting(SettingForRendering value);

#pragma endregion
	};
}