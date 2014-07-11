#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <ace.common.Base.h>
#include <Utility/ace.BinaryWriter.h>
#include "Rendering/SettingForRendering.h"
#include "PngGenerator.h"
#include "ResultOfGeneratingPng.h"

namespace FontGenerator
{
	class Generator
	{
	private:
		const int INDEX_MAX = 65536;
		const int DEFAULT_SIZE = 256;

		ace::astring m_sheetName;
		int m_sheetSize;
		SettingForRendering m_setting;

	public:
		Generator();

		void GenerateFontFile(
			ace::astring fontPath,
			ace::astring textPath);

		std::vector<ace::achar> GetCharactors(ace::astring textPath);
		ResultOfGeneratingPng RenderPng(ace::astring fontPath, ace::astring textPath);
		void PushAff(ace::BinaryWriter& writer, ResultOfGeneratingPng& result);

		ace::astring GetSheetName() const;
		void SetSheetName(ace::astring value);

		int GetSheetSize() const;
		void SetSheetSize(int value);

		SettingForRendering GetSetting() const;
		void SetSetting(SettingForRendering value);
	};
}