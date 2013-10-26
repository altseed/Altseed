#pragma once
#include <fstream>
#include "ace.Log.h"

namespace ace
{
	class Log_Imp : public Log
	{
	private:
		eLogLevel m_outputLevel;
		std::ofstream m_file;
		bool isInTable;

		bool WritingIsRequired( int level );
		static ace::astring ReplaceSpecialChar(ace::astring text);
		static std::string ArrangeString(const achar* text);

	public:
		Log_Imp(const achar* path);
		virtual ~Log_Imp();

		static Log_Imp* Create(const achar* path, const achar* title);
		void Write(const achar* text, int level = LOG_ALL);
		void WriteLine(const achar* text, int level = LOG_ALL);
		void WriteStrongly(const achar* text, int level = LOG_ALL);
		void WriteLineStrongly(const achar* text, int level = LOG_ALL);
		void WriteHeading(const achar* text, int level = LOG_ALL);

		void WriteHorizontalRule(int level = LOG_ALL);
		void BeginTable(int level = LOG_ALL);
		void EndTable(int level = LOG_ALL);
		void ChangeRow(int level = LOG_ALL);
		void ChangeColumn(int level = LOG_ALL);
		bool IsValid();
		void SetOutputLevel(int level);

#if !_WIN32 && !SWIG
	public:
		static Log_Imp* Create(const wchar_t* path, const wchar_t* title);
		void Write(const wchar_t* text, int level = LOG_ALL);
		void WriteLine(const wchar_t* text, int level = LOG_ALL);
		void WriteStrongly(const wchar_t* text, int level = LOG_ALL);
		void WriteLineStrongly(const wchar_t* text, int level = LOG_ALL);
		void WriteHeading(const wchar_t* text, int level = LOG_ALL);
#endif

#ifndef SWIG
	public:
		static Log_Imp* Create(const char* path, const char* title);
		void Write(const char* text, int level = LOG_ALL);
		void WriteLine(const char* text, int level = LOG_ALL);
		void WriteStrongly(const char* text, int level = LOG_ALL);
		void WriteLineStrongly(const char* text, int level = LOG_ALL);
		void WriteHeading(const char* text, int level = LOG_ALL);
#endif
	};
}