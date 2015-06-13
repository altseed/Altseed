#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <asd.common.Base.h>
#include <Utility/asd.BinaryWriter.h>
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

		asd::astring m_sheetName;
		int m_sheetSize;
		SettingForRendering m_setting;

	public:
		Generator();

		void GenerateFontFile(
			asd::astring fontPath,
			asd::astring textPath);

		std::vector<asd::achar> GetCharactors(asd::astring textPath);
		ResultOfGeneratingPng RenderPng(asd::astring fontPath, asd::astring textPath);
		void PushAff(asd::BinaryWriter& writer, ResultOfGeneratingPng& result);

		asd::astring GetSheetName() const;
		void SetSheetName(asd::astring value);

		int GetSheetSize() const;
		void SetSheetSize(int value);

		SettingForRendering GetSetting() const;
		void SetSetting(SettingForRendering value);
	};
}