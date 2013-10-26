#include <fstream>
#include <cstdint>
#include "ace.Log_Imp.h"
using namespace std;

namespace ace
{
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
	bool Log_Imp::WritingIsRequired(int level)
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
		temp->m_outputLevel = LOG_ALL;
		temp->isInTable = false;

		temp->m_file << "<!DOCTYPE html>\r\n"
			<< "<html>\r\n<head>\r\n<meta charset=\"UTF-8\">\r\n"
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
	void Log_Imp::Write(const achar* text, int level)
	{
		if (WritingIsRequired(level))
		{
			m_file << ArrangeString(text);
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteLine(const achar* text, int level)
	{
		if (WritingIsRequired(level))
		{
			m_file << ArrangeString(text) << "<br />" << endl;
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteStrongly(const achar* text, int level)
	{
		if (WritingIsRequired(level))
		{
			m_file << "<strong>" << ArrangeString(text) << "</strong>";
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteLineStrongly(const achar* text, int level)
	{
		if (WritingIsRequired(level))
		{
			m_file << "<strong>" << ArrangeString(text) << "</strong>" << endl;
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteHeading(const achar* text, int level)
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
	void Log_Imp::Write(const wchar_t* text, int level)
	{
		Write( ToAString(text).c_str(), level );
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteLine(const wchar_t* text, int level)
	{
		WriteLine( ToAString(text).c_str(), level );
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteStrongly(const wchar_t* text, int level)
	{
		WriteStrongly( ToAString(text).c_str(), level );
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteLineStrongly(const wchar_t* text, int level)
	{
		WriteLineStrongly( ToAString(text).c_str(), level );
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteHeading(const wchar_t* text, int level)
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
	void Log_Imp::Write(const char* text, int level)
	{
		Write(ToAString(text).c_str(), level);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteLine(const char* text, int level)
	{
		WriteLine(ToAString(text).c_str(), level);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteStrongly(const char* text, int level)
	{
		WriteStrongly(ToAString(text).c_str(), level);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteLineStrongly(const char* text, int level)
	{
		WriteLineStrongly(ToAString(text).c_str(), level);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteHeading(const char* text, int level)
	{
		WriteHeading(ToAString(text).c_str(), level);
	}
#pragma endregion

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::WriteHorizontalRule(int level)
	{
		if (WritingIsRequired(level))
		{
			m_file << "<hr/>" << endl;
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::BeginTable(int level)
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
	void Log_Imp::EndTable(int level)
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
	void Log_Imp::ChangeRow(int level)
	{
		if (WritingIsRequired(level) && isInTable)
		{
			m_file << "</td></tr><tr><td>" << endl;
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void Log_Imp::ChangeColumn(int level)
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
	void Log_Imp::SetOutputLevel(int level)
	{
		m_outputLevel = (eLogLevel)level;
	}
}