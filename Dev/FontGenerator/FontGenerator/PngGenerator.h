#pragma once
#include <vector>
#include <ace.common.Base.h>
#include "Font.h"
#include "ResultOfGeneratingPng.h"

namespace FontGenerator
{
	class PngGenerator
	{
	private:
		ace::astring m_sheetName;
		int m_fontSize;
		int m_sheetSize;
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

		int GetFontSize() const;
		void SetFontSize(int value);

		int GetSheetSize() const;
		void SetSheetSize(int value);
#pragma endregion
	};
}