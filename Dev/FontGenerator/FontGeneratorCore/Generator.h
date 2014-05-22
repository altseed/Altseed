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
		std::wstring m_sheetName;
		SettingForRendering m_setting;

	public:
		Generator();

		void GenerateFontFile(
			std::wstring fontPath,
			std::wstring textPath);

		std::vector<ace::achar> GetCharactors(ace::astring textPath);
		ResultOfGeneratingPng RenderPng(std::wstring fontPath, std::wstring textPath);
		void PushAff(ace::BinaryWriter& writer, ResultOfGeneratingPng& result);

		std::wstring GetSheetName() const;
		void SetSheetName(std::wstring value);

		SettingForRendering GetSetting() const;
		void SetSetting(SettingForRendering value);
	};
}