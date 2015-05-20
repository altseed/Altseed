#pragma once

#include "../ace.ReferenceObject.h"
#include "../ace.Core.Base.h"
#include "ace.StaticFile.h"
#include "ace.StreamFile.h"
#include <iterator>
#include <vector>
#include <memory>

namespace ace
{
	/**
		@brief	ファイルの操作に関する処理を提供するクラス
	*/
	class File
		: public IReference
	{
		friend class Accessor;

	protected:
		virtual StaticFile* CreateStaticFile_(const achar* path) = 0;
		virtual StreamFile* CreateStreamFile_(const achar* path) = 0;

	public:
		virtual ~File() { }

		/**
			@brief	ファイル操作する時のルートのディレクトリを追加する。
			@param	path	ルートのディレクトリのパス
		*/
		virtual void AddRootDirectory(const achar* path) = 0;

		/**
		@brief	ファイル操作する時のルートのパスワード付パッケージを追加する。
		@param	path		パッケージのパス
		@param	password	パスワード
		*/
		virtual void AddRootPackageWithPassword(const achar* path, const achar* password) = 0;
		
		/**
		@brief	ファイル操作する時のルートのパッケージを追加する。
		@param	path		パッケージのパス
		*/
		virtual void AddRootPackage(const achar* path) = 0;

		/**
		@brief	追加されたパッケージを全て消去する。
		*/
		virtual void ClearRootDirectories() = 0;

		/**
		@brief	ファイルが存在するか取得する。
		@param	path		ファイルのパス
		@return	ファイルが存在するか?
		*/
		virtual bool Exists(const achar* path) const = 0;

#ifndef SWIG
		std::shared_ptr<StaticFile> CreateStaticFile(const achar* path)
		{
			return CreateSharedPtrWithReleaseDLL(CreateStaticFile_(path));
		}

		std::shared_ptr<StreamFile> CreateStreamFile(const achar* path)
		{
			return CreateSharedPtrWithReleaseDLL(CreateStreamFile_(path));
		}
#endif
	};
}