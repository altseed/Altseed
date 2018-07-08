#pragma once

#include "../asd.Core.Base.h"

#include "asd.FileRoot.h"
#include "asd.File.h"
#include "asd.PackFile.h"
#include "asd.FileHelper.h"
#include "StaticFile/StaticFileLoader.h"
#include "../Utils/asd.Synchronizer.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>
#include <mutex>

namespace std
{
	template<>
	struct hash < shared_ptr <asd::FileRoot> >
	{
		size_t operator()(shared_ptr<asd::FileRoot> ptr) const
		{
			return static_cast<size_t>(hash<asd::astring>()(ptr->m_path));
		}
	};
}

namespace asd
{
	class BaseFile;

	class File_Imp
		: public File
		, public ReferenceObject
	{
	private:
		std::vector<std::shared_ptr<FileRoot>> m_roots;

		std::unordered_map<astring, StreamFile_Imp*> streamFiles;
		
		std::recursive_mutex		mtx_;

		StaticFileLoader::Ptr staticFileLoader;
		StaticFileLoader::Ptr asyncFileLoader;
		StaticFile* CreateStaticFile(const char16_t* path, StaticFileLoader::Ptr loader);

	public:
		static File_Imp* Create(Synchronizer::Ptr sync) { return new File_Imp(sync); };

		File_Imp(Synchronizer::Ptr sync);
		virtual ~File_Imp();

		void AddDefaultRootDirectory();

		virtual void AddRootDirectory(const char16_t* path) override;
		virtual void AddRootPackageWithPassword(const char16_t* path, const char16_t* password)  override;
		virtual void AddRootPackage(const char16_t* path)  override;
		virtual void ClearRootDirectories() override;
		virtual bool Exists(const char16_t* path) const override;

#ifndef SWIG
		std::mutex packedFileMutex;

		virtual StaticFile* CreateStaticFile(const char16_t* path) override;
		virtual StaticFile* CreateStaticFileAsync(const char16_t* path) override;
		virtual StreamFile* CreateStreamFile(const char16_t* path) override;

		void UnregisterStaticFile(const astring& key);
		void UnregisterStreamFile(const astring& key);

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}
