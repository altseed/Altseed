#pragma once

#include "../asd.ReferenceObject.h"
#include "../asd.Core.Base.h"
#include "asd.StaticFile.h"
#include "asd.StreamFile.h"
#include <iterator>
#include <vector>
#include <memory>

namespace asd
{
	/**
		@brief	ファイルの操作に関する処理を提供するクラス
	*/
	class File
		: public IReference
	{
		friend class Accessor;

	protected:


		File() {}
		virtual ~File() { }
	public:
		
		/**
		@brief	一括読込のファイルを生成する。
		@param	path	ファイルパス
		@return	ファイル
		*/
		virtual StaticFile* CreateStaticFile(const char16_t* path) = 0;

		/**
		@brief	一括読込のファイルを非同期に生成する。
		@param	path	ファイルパス
		@return	ファイル
		*/
		virtual StaticFile* CreateStaticFileAsync(const char16_t* path) = 0;

		/**
		@brief	逐次読込のファイルを生成する。
		@param	path	ファイルパス
		@return	ファイル
		*/
		virtual StreamFile* CreateStreamFile(const char16_t* path) = 0;

		/**
			@brief	ファイル操作する時のルートのディレクトリを追加する。
			@param	path	ルートのディレクトリのパス
		*/
		virtual void AddRootDirectory(const char16_t* path) = 0;

		/**
		@brief	ファイル操作する時のルートのパスワード付パッケージを追加する。
		@param	path		パッケージのパス
		@param	password	パスワード
		*/
		virtual void AddRootPackageWithPassword(const char16_t* path, const char16_t* password) = 0;
		
		/**
		@brief	ファイル操作する時のルートのパッケージを追加する。
		@param	path		パッケージのパス
		*/
		virtual void AddRootPackage(const char16_t* path) = 0;

		/**
		@brief	追加されたパッケージを全て消去する。
		*/
		virtual void ClearRootDirectories() = 0;

		/**
		@brief	ファイルが存在するか取得する。
		@param	path		ファイルのパス
		@return	ファイルが存在するか?
		*/
		virtual bool Exists(const char16_t* path) const = 0;
	};
}