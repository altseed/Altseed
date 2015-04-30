#pragma once

#include "../ace.Core.Base.h"

#include "ace.FileRoot.h"
#include "ace.File.h"
#include "ace.PackFile.h"
#include "ace.FileHelper.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>

namespace std
{
	template<>
	struct hash < shared_ptr <ace::FileRoot> >
	{
		size_t operator()(shared_ptr<ace::FileRoot> ptr) const
		{
			return static_cast<size_t>(hash<ace::astring>()(ptr->m_path));
		}
	};
}

namespace ace
{
	class BaseFile;

	class File_Imp
		: public File
		, public ReferenceObject
	{
	private:
		std::vector<std::shared_ptr<FileRoot>> m_roots;

		std::unordered_map<astring, StaticFile_Imp*> staticFiles;
		std::unordered_map<astring, StreamFile_Imp*> streamFiles;
		
	public:
		static File_Imp* Create() { return new File_Imp(); };

		File_Imp();
		virtual ~File_Imp();

		void AddDefaultRootDirectory();

		virtual void AddRootDirectory(const achar* path) override;
		virtual void AddRootPackageWithPassword(const achar* path, const achar* key)  override;
		virtual void AddRootPackage(const achar* path)  override;
		virtual void ClearRootDirectories() override;
		virtual bool Exists(const achar* path) const override;

#ifndef SWIG
		virtual StaticFile* CreateStaticFile_(const achar* path);
		virtual StreamFile* CreateStreamFile_(const achar* path);

		void UnregisterStaticFile(const astring& key);
		void UnregisterStreamFile(const astring& key);

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}