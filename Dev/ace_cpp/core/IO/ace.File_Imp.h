﻿#pragma once

#include "../ace.Core.Base.h"
#include "ace.RootPath_Imp.h"
#include "ace.File.h"
#include "ace.PackFile_Imp.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>

namespace std
{
	template<>
	struct hash < shared_ptr <ace::RootPath_Imp> >
	{
		size_t operator()(shared_ptr<ace::RootPath_Imp> ptr) const
		{
			return static_cast<size_t>(hash<ace::astring>()(ptr->m_path.ToAstring()));
		}
	};
}

namespace ace
{
	class BaseFile_Imp;

	class File_Imp : public File, public ReferenceObject
	{
	private:
		std::unordered_map<astring, std::shared_ptr<BaseFile_Imp>> m_fileCash;
		std::unordered_set<std::shared_ptr<RootPath_Imp>> m_rootPathes;

		std::unordered_map<astring, StaticFile_Imp*> m_staticFileCash;
		std::unordered_map<astring, StreamFile_Imp*> m_streamFileCash;

		std::unordered_map<astring, PackFile_Imp*> m_packFileCash;

		template<typename _T>
		inline bool Valid(_T* ptr) { return ptr && (0 < ptr->GetRef()); }

		File_Imp() { };

	protected:
		template<typename _InIt>
		void SetRootDirectories(_InIt first, _InIt end);
		StreamFile* CreateStreamFileDirectly(const astring& normalizedPath);

		bool ConsistOf(const std::vector<astring>& vec, const astring& element) const { for (const auto& v : vec) { if (v == element) return true; } return false; }

	public:
		static File_Imp* Create() { return new File_Imp(); };

		virtual ~File_Imp();
		virtual void SetRootDirectories(const astring& path);
		virtual void SetRootDirectories(const astring& path, const astring& path2);
		virtual void SetRootDirectories(const astring& path, const astring& path2, const astring& path3);
		virtual void GetRootDirectories(std::vector<std::reference_wrapper<Path>>& rootPathes) const;
		virtual void EnumerateFiles(const astring& path) const;
		virtual void EnumerateFiles(const astring& path, const astring& searchPattern) const;
		virtual void EnumerateFiles(const astring& path, const astring& searchPattern, bool isRecursive) const;
		virtual bool Exists(const astring& path) const;
		virtual StaticFile* CreateStaticFile(const astring& path);
		virtual StreamFile* CreateStreamFile(const astring& path);
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
	};
}