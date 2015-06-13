#pragma once
#include <fstream>
#include "asd.Log.h"

namespace asd
{
	class Log_Imp : public Log
	{
	private:
		LogLevel m_outputLevel;
		std::ofstream m_file;
		bool isInTable;

		bool WritingIsRequired(LogLevel level);
		static asd::astring ReplaceSpecialChar(asd::astring text);
		static std::string ArrangeString(const achar* text);

	public:
		Log_Imp(const achar* path);
		virtual ~Log_Imp();

		static Log_Imp* Create(const achar* path, const achar* title);
		void Write(const achar* text, LogLevel level = LogLevel::All);
		void WriteLine(const achar* text, LogLevel level = LogLevel::All);
		void WriteStrongly(const achar* text, LogLevel level = LogLevel::All);
		void WriteLineStrongly(const achar* text, LogLevel level = LogLevel::All);
		void WriteHeading(const achar* text, LogLevel level = LogLevel::All);

		void WriteHorizontalRule(LogLevel level = LogLevel::All);
		void BeginTable(LogLevel level = LogLevel::All);
		void EndTable(LogLevel level = LogLevel::All);
		void ChangeRow(LogLevel level = LogLevel::All);
		void ChangeColumn(LogLevel level = LogLevel::All);
		bool GetIsValid() override;
		void SetOutputLevel(LogLevel level);

#if !_WIN32 && !SWIG
	public:
		static Log_Imp* Create(const wchar_t* path, const wchar_t* title);
		void Write(const wchar_t* text, LogLevel level = LogLevel::All);
		void WriteLine(const wchar_t* text, LogLevel level = LogLevel::All);
		void WriteStrongly(const wchar_t* text, LogLevel level = LogLevel::All);
		void WriteLineStrongly(const wchar_t* text, LogLevel level = LogLevel::All);
		void WriteHeading(const wchar_t* text, LogLevel level = LogLevel::All);
#endif

#ifndef SWIG
	public:
		static Log_Imp* Create(const char* path, const char* title);
		void Write(const char* text, LogLevel level = LogLevel::All);
		void WriteLine(const char* text, LogLevel level = LogLevel::All);
		void WriteStrongly(const char* text, LogLevel level = LogLevel::All);
		void WriteLineStrongly(const char* text, LogLevel level = LogLevel::All);
		void WriteHeading(const char* text, LogLevel level = LogLevel::All);
#endif
	};
}