#pragma once
#include <string>
#include <memory>
#include <ace.common.Base.h>

namespace ace
{
	enum class LogLevel
	{
		Error = 1 << 1,
		Critical = 1 << 2,
		Warning = 1 << 3,
		Information = 1 << 4,
		All = 2147483647	// INT_MAXと等しい
	};

	class Log
	{
	public:
		virtual ~Log()
		{
		}

		/**
			@brief	ログ ファイルへ文字列を書き込む。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void Write(const achar* text, LogLevel level = LogLevel::All) = 0;

		/**
			@brief	ログ ファイルへ文字列を書き込み、改行する。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLine(const achar* text, LogLevel level = LogLevel::All) = 0;

		/**
			@brief	ログ ファイルへ強調された文字列を書き込む。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteStrongly(const achar* text, LogLevel level = LogLevel::All) = 0;

		/**
			@brief	ログ ファイルへ強調された文字列を書き込み、改行する。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLineStrongly(const achar* text, LogLevel level = LogLevel::All) = 0;

		/**
			@brief	ログ ファイルへ見出しとして文字列を書き込む。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHeading(const achar* text, LogLevel level = LogLevel::All) = 0;

#if !_WIN32 && !SWIG
		/**
		@brief	ログ ファイルへ文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void Write(const wchar_t* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLine(const wchar_t* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteStrongly(const wchar_t* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLineStrongly(const wchar_t* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ見出しとして文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHeading(const wchar_t* text, LogLevel level = LogLevel::All) = 0;
#endif

#ifndef SWIG
	public:
		/**
		@brief	ログ ファイルへ文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void Write(const char* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLine(const char* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteStrongly(const char* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLineStrongly(const char* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ見出しとして文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHeading(const char* text, LogLevel level = LogLevel::All) = 0;
#endif

		/**
			@brief	ログ ファイルへ水平線を書き込む。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHorizontalRule(LogLevel level = LogLevel::All) = 0;

		/**
			@brief	ログ ファイルへ表形式(<table> 要素)での書き込みを開始する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void BeginTable(LogLevel level = LogLevel::All) = 0;

		/**
			@brief	ログ ファイルへの表形式(<table> 要素)での書き込みを終了する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void EndTable(LogLevel level = LogLevel::All) = 0;

		/**
			@brief	表形式での出力において、新しい行に移動する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void ChangeRow(LogLevel level = LogLevel::All) = 0;

		/**
			@brief	表形式での出力において、新しい列に移動する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void ChangeColumn(LogLevel level = LogLevel::All) = 0;

		/**
			@brief	ログの出力が可能であるかどうかを表す真偽値を取得する。
			@param	level	[in]	ログの出力が可能か?
		*/
		virtual bool IsValid() = 0;

		/**
			@brief	ログの出力レベルを設定する。ログ出力メソッドに指定した出力レベルがこの値より小さければログが出力される。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void SetOutputLevel(LogLevel level) = 0;
	};
}