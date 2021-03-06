
#pragma once

#include "Generator.h"

#include <string>
#include <vector>
#include <stdint.h>

#include <Font/asd.FontRasterizer.h>
#include <Font/asd.InstalledFontList.h>



namespace FontGenerator
{
	class DLL
	{
	private:
		int32_t			m_fontSize = 32;
		Color			m_fontColor = Color(255, 255, 255, 255);
		Color			m_outlineColor = Color(255, 255, 255, 255);
		int32_t			m_outlineSize = 0;
		int32_t			m_outlineSampling = 1;
		asd::astring	m_fontName;
		asd::astring	m_textFilePath;
		asd::astring	m_exportPath;
		int				m_sheetSize;

		
		std::vector < asd::astring > fontNames;
		std::vector < asd::astring > fontPathes;

	public:
		DLL();
		virtual ~DLL();

		void LoadFontNames();
		int32_t GetFontCount();
		const asd::achar* GetFontName(int32_t index);
		const asd::achar* GetFontPath(int32_t index);
		const asd::achar* SavePreview();

		bool Run();

		void SetFontSize(int32_t fontSize)
		{
			m_fontSize = fontSize;
		}

		void SetFontColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			m_fontColor = Color(r, g, b, a);
		}

		void SetFontName(const asd::achar* name)
		{
			m_fontName = name;
		}

		void SetTextFilePath(const asd::achar* path)
		{
			m_textFilePath = path;
		}

		void SetExportPath(const asd::achar* path)
		{
			m_exportPath = path;
		}

		void SetSheetSize(int sheetSize)
		{
			m_sheetSize = sheetSize;
		}

		void SetOutlineColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			m_outlineColor = Color(r, g, b, a);
		}

		void SetOutlineSize(int32_t size)
		{
			m_outlineSize = size;
		}

		void SetOutlineSampling(int32_t sampling)
		{
			m_outlineSampling = sampling;
		}
	};
}