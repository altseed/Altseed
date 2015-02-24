#include <fstream>
#include <cstdint>
#include "ace.Log_Imp.h"
using namespace std;

namespace ace
{
	static const char* css = R"(
@charset "utf-8";

body
{
	line-height: 1.5em;
}

*::selection
{
	background-color: rgba(255, 210, 130, 0.5);
/*	background-color: rgba(160, 220, 150, 0.5);*/
}

h1, h2, h3, h4
{
	font-family: "Meiryo UI";
	display: block;
}

h1, h2, h3
{
	padding: 0.2em 0.3em;
}

h1
{
	font-size: 2em;
	background-color: #ddeeee;
	border-bottom: 1px solid #999999;
	line-height: 1.2em;
}
h2
{
	font-size: 1.5em;
	background-color: #eef3f3;
	border-bottom: 1px solid #999999;
	line-height: 1.2em;
}
h3{
	font-size: 1.3em;
	background-color: #eeeeee;
}
h4
{
	font-size: 1.1em;
	margin-top: 0.3em;
	margin-bottom: 0em;
}

p
{
	margin-bottom: 0.9em;
}

p code {
  border: 1px solid #ccc;
  border-radius: 3px;
  background-color: #f9f9f9;
  padding: 1px 5px;
  display: inline-block;
}

pre
{
  margin-top: 0.1em;
  margin-left: 1em;
  border: 1px solid #ccc;
  background-color: #f9f9f9;
  padding: 3px 5px;
  display: block;
  font: 1em "Consolas";
}

pre code
{
  font: 0.9em "Consolas";
}

code
{
  color: #52595d;
  font: 0.95em "Consolas";
  margin: 0em 0.3em;
}

hr
{
	border: 0;
	border-bottom: 2px dashed #acc;
	background-color: #fff;
}

ul
{
	list-style-type: none;
}

li
{
	border-left: 2px solid #cdd;
	margin-bottom: 2px;
	padding-left: 0.5em;
}

em
{
	display: none;
}

table
{
	border-collapse: collapse;
}

td, th
{
	border: 1px solid #8bb;
	padding: 2px 5px;
	vertical-align: middle;
}
)";

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	Log_Imp::~Log_Imp()
	{
		m_file << "</body>\r\n</html>";
		m_file.close();
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	bool Log_Imp::WritingIsRequired(LogLevel level)
	{
		return !m_file.fail() && level <= m_outputLevel;
	}

#pragma region achar
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	Log_Imp::Log_Imp(const achar* path)
		: m_file(ToUtf8String(path).c_str(), ios::out)
	{
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	Log_Imp* Log_Imp::Create(const achar* path, const achar* title)
	{
		auto temp = new Log_Imp(path);
		temp->m_outputLevel = LogLevel::All;
		temp->isInTable = false;

		temp->m_file << "<!DOCTYPE html>\r\n"
			<< "<html>\r\n<head>\r\n<meta charset=\"UTF-8\">\r\n"
			<< "<STYLE type=\"text/css\">\r\n<!--\r\n" << ArrangeString(ToAString(css).c_str()) << "\r\n-->\r\n</STYLE>\r\n"
			<< "<title>" << ArrangeString(title) << "</title>\r\n"
			<< "</head>\r\n<body>";

		return temp;
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	astring Log_Imp::ReplaceSpecialChar(astring text)
	{
		astring result = text;

		result = ReplaceAll(result, L"<", L"&lt;");
		result = ReplaceAll(result, L">", L"&gt;");

		return result;
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	string Log_Imp::ArrangeString(const achar* text)
	{
		astring replaced = ReplaceSpecialChar(astring(text));
		return ToUtf8String(replaced.c_str());
	}


	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::Write(const achar* text, LogLevel level)
	{
		if (WritingIsRequired(level))
		{
			m_file << ArrangeString(text);
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteLine(const achar* text, LogLevel level)
	{
		if (WritingIsRequired(level))
		{
			m_file << ArrangeString(text) << "<br />" << endl;
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteStrongly(const achar* text, LogLevel level)
	{
		if (WritingIsRequired(level))
		{
			m_file << "<strong>" << ArrangeString(text) << "</strong>";
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteLineStrongly(const achar* text, LogLevel level)
	{
		if (WritingIsRequired(level))
		{
			m_file << "<strong>" << ArrangeString(text) << "</strong><br />" << endl;
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteHeading(const achar* text, LogLevel level)
	{
		if (WritingIsRequired(level))
		{
			m_file << "<h2>" << ArrangeString(text) << "</h2>" << endl;
		}
	}
#pragma endregion

#pragma region wchar_t
#if !_WIN32 && !SWIG
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	Log_Imp* Log_Imp::Create(const wchar_t* path, const wchar_t* title)
	{
		return Create(ToAString(path).c_str(),ToAString(path).c_str());
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::Write(const wchar_t* text, LogLevel level)
	{
		Write( ToAString(text).c_str(), level );
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteLine(const wchar_t* text, LogLevel level)
	{
		WriteLine( ToAString(text).c_str(), level );
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteStrongly(const wchar_t* text, LogLevel level)
	{
		WriteStrongly( ToAString(text).c_str(), level );
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteLineStrongly(const wchar_t* text, LogLevel level)
	{
		WriteLineStrongly( ToAString(text).c_str(), level );
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteHeading(const wchar_t* text, LogLevel level)
	{
		WriteHeading( ToAString(text).c_str(), level );
	}
#endif
#pragma endregion

#pragma region char
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	Log_Imp* Log_Imp::Create(const char* path, const char* title)
	{
		return Create(ToAString(path).c_str(), ToAString(title).c_str());
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::Write(const char* text, LogLevel level)
	{
		Write(ToAString(text).c_str(), level);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteLine(const char* text, LogLevel level)
	{
		WriteLine(ToAString(text).c_str(), level);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteStrongly(const char* text, LogLevel level)
	{
		WriteStrongly(ToAString(text).c_str(), level);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteLineStrongly(const char* text, LogLevel level)
	{
		WriteLineStrongly(ToAString(text).c_str(), level);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteHeading(const char* text, LogLevel level)
	{
		WriteHeading(ToAString(text).c_str(), level);
	}
#pragma endregion

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteHorizontalRule(LogLevel level)
	{
		if (WritingIsRequired(level))
		{
			m_file << "<hr/>" << endl;
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::BeginTable(LogLevel level)
	{
		if (WritingIsRequired(level))
		{
			m_file << "<table><tr><td>" << endl;
			isInTable = true;
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::EndTable(LogLevel level)
	{
		if (WritingIsRequired(level) && isInTable)
		{
			m_file << "</td></tr></table>" << endl;
			isInTable = false;
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::ChangeRow(LogLevel level)
	{
		if (WritingIsRequired(level) && isInTable)
		{
			m_file << "</td></tr><tr><td>" << endl;
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::ChangeColumn(LogLevel level)
	{
		if (WritingIsRequired(level) && isInTable)
		{
			m_file << "</td><td>" << endl;
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	bool Log_Imp::IsValid()
	{
		return !m_file.fail();
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::SetOutputLevel(LogLevel level)
	{
		m_outputLevel = level;
	}
}