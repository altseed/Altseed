
#pragma once

#include "Generator.h"

namespace FontGenerator
{
	class DLL
	{
	private:
		int32_t			m_fontSize = 32;
		Color			m_fontColor = Color(255, 255, 255, 255);
		ace::astring	m_fontName;
		ace::astring	m_textFilePath;
		ace::astring	m_exportPath;
		int				m_sheetSize;

	public:
		DLL();
		virtual ~DLL();

		bool Run();

		void SetFontSize(int32_t fontSize)
		{
			m_fontSize = fontSize;
		}

		void SetFontColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			m_fontColor = Color(r, g, b, a);
		}

		void SetFontName(const ace::achar* name)
		{
			m_fontName = name;
		}

		void SetTextFilePath(const ace::achar* path)
		{
			m_textFilePath = path;
		}

		void SetExportPath(const ace::achar* path)
		{
			m_exportPath = path;
		}

		void SetSheetSize(int sheetSize)
		{
			m_sheetSize = sheetSize;
		}
	};
}