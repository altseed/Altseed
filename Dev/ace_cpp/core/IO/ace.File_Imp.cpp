#include "ace.File_Imp.h"
#include "ace.BaseFile_Imp.h"
#include "ace.BaseFile_Imp.h"
#include "ace.Path_Imp.h"
#include "ace.StaticFile_Imp.h"
#include "ace.StreamFile_Imp.h"
#include <array>
#include <algorithm>
#include <sys/stat.h>
#include <unordered_set>

namespace ace
{
	File_Imp::~File_Imp()
	{
		for (auto tpl : m_packFileCash)
		{
			if (tpl.second)
				delete tpl.second;
		}
	}

	void File_Imp::AddRootDirectories(const achar* path)
	{
		std::array<astring, 1> args = { path };
		AddRootDirectories(args.begin(), args.end());
	}

	void File_Imp::AddRootPackage(const achar* path, const achar* key)
	{
		std::array<astring, 1> args = { path };
		AddRootDirectories(args.begin(), args.end());
	}
	
	void File_Imp::ClearRootDirectories()
	{
		m_rootPathes.clear();
		AddDefaultRootDirectory();
	}
	//void File_Imp::SetRootDirectories(const achar* path)
	//{
	//	std::array<astring, 1> args = { path };
	//	SetRootDirectories(args.begin(), args.end());
	//}
	//
	//void File_Imp::SetRootDirectories(const achar* path, const achar* path2)
	//{
	//	std::array<astring, 2> args = { path, path2 };
	//	SetRootDirectories(args.begin(), args.end());
	//}
	//void File_Imp::SetRootDirectories(const achar* path, const achar* path2, const achar* path3)
	//{
	//	std::array<astring, 3> args = { path, path2, path3 };
	//	SetRootDirectories(args.begin(), args.end());
	//}
	template<typename _InIt>
	void File_Imp::AddRootDirectories(_InIt first, _InIt end)
	{
		//m_rootPathes.clear();
		std::for_each(first, end, [&](const astring& path)
		{
			m_rootPathes.insert(m_rootPathes.begin(), std::shared_ptr<RootPath_Imp>(new RootPath_Imp(path)));
		});
	}
	//void File_Imp::GetRootDirectories(std::vector<std::reference_wrapper<Path>>& rootPathes) const
	//{
	//	rootPathes.clear();
	//	for (const auto& root : m_rootPathes)
	//	{
	//		rootPathes.push_back(root->m_path);
	//	}
	//}
	//void File_Imp::EnumerateFiles(const achar* path) const
	//{
	//}
	//void File_Imp::EnumerateFiles(const achar* path, const achar* searchPattern) const
	//{
	//}
	//void File_Imp::EnumerateFiles(const achar* path, const achar* searchPattern, bool isRecursive) const
	//{
	//}
	bool File_Imp::Exists(const achar* path) const
	{
		assert(false);
		return true;
	}

	StaticFile* File_Imp::CreateStaticFile_(const achar* path)
	{
		if (!Path_Imp::IsAbsolutePath(path))
		{
			std::unordered_set<astring> ignoreFiles;
			
			for (const auto& root : m_rootPathes)
			{
				if (root->m_isPackFile)
				{
					const auto& packPath = root->m_path;

					if (m_packFileCash.find(packPath.ToAstring()) == m_packFileCash.end())
					{
						StaticFile_Imp* pstaticFile(nullptr);
						{
							std::shared_ptr<BaseFile_Imp> pBaseFile(new BaseFile_Imp(packPath), [](BaseFile_Imp* p){ SafeRelease(p); });

							m_packFileCash.emplace(packPath.ToAstring(), new PackFile_Imp(pBaseFile));
						}
					}

					if (m_packFileCash[packPath.ToAstring()]->HaveFile(path) && ignoreFiles.find(path) == ignoreFiles.end() )
					{
						const auto internalHeader = m_packFileCash[packPath.ToAstring()]->GetInternalHeader(path);

						ace::Path_Imp fullPath(root->m_path.ToAstring());
						fullPath /= path;
						fullPath.Normalize();
						const auto normalizedPath(fullPath.ToAstring());

						//auto fileCash = m_staticFileCash.find(normalizedPath);
						//if (fileCash != m_staticFileCash.end() &&
						//	Valid(fileCash->second))
						//{
						//	return fileCash->second;
						//}

						StaticFile_Imp* pstaticFile(nullptr);

						{
							std::shared_ptr<BaseFile_Imp> pBaseFile(new BaseFile_Imp(packPath), [](BaseFile_Imp* p){ SafeRelease(p); });

							pstaticFile = new StaticFile_Imp(pBaseFile, *internalHeader);

							//m_staticFileCash.emplace(normalizedPath, pstaticFile);

							return pstaticFile;
						}
					}
					else
					{
						m_packFileCash[packPath.ToAstring()]->GetTopHeader()->AddIgnoreFiles(ignoreFiles);
					}
				}
				else
				{
					ace::Path_Imp fullPath(root->m_path.ToAstring());
					fullPath /= path;
					fullPath.Normalize();
					const auto normalizedPath(fullPath.ToAstring());

					//auto fileCash = m_staticFileCash.find(normalizedPath);
					//if (fileCash != m_staticFileCash.end() &&
					//	Valid(fileCash->second))
					//{
					//	return fileCash->second;
					//}

					StaticFile_Imp* pstaticFile(nullptr);

					{
						// ToDo あとで Exist何とか 関数に移動
#ifdef _WIN32
						struct _stat buf;
						if (!_wstat(normalizedPath.c_str(), &buf))
#else
						struct stat buf;
						if (!stat(ToUtf8String(normalizedPath.c_str()).c_str(), &buf))
#endif
						{
							std::shared_ptr<BaseFile_Imp> pBaseFile(new BaseFile_Imp(fullPath), [](BaseFile_Imp* p){ SafeRelease(p); });

							pstaticFile = new StaticFile_Imp(pBaseFile);

							//m_staticFileCash.emplace(normalizedPath, pstaticFile);

							return pstaticFile;
						}
					}
				}
			}
			return nullptr;
		}
		else // absolute path
		{
			assert(false);
		}
	}

	/*
	StreamFile* File_Imp::CreateStreamFile(const achar* path)
	{
		if (!Path_Imp::IsAbsolutePath(path))
		{
			std::vector<astring> ignoreFiles;

			for (const auto& root : m_rootPathes)
			{
				if (root->m_isPackFile)
				{
					const auto& packPath = root->m_path;

					if (m_packFileCash[packPath.ToAstring()]->HaveFile(path))
					{
						const auto internalHeader = m_packFileCash[packPath.ToAstring()]->GetInternalHeader(path);

						ace::Path_Imp fullPath(root->m_path.ToAstring());
						fullPath /= path;
						fullPath.Normalize();

						auto streamFile = CreateStreamFileDirectly(fullPath.ToAstring().c_str());
						if (streamFile)
							return streamFile;
					}
					else
					{
						m_packFileCash[packPath.ToAstring()]->GetTopHeader()->AddIgnoreFiles(ignoreFiles);
					}
				}
				else
				{
					ace::Path_Imp fullPath(root->m_path.ToAstring());
					fullPath /= path;
					fullPath.Normalize();

					auto streamFile = CreateStreamFileDirectly(fullPath.ToAstring().c_str());
					if (streamFile)
						return streamFile;
				}
			}
		}
		return nullptr;
	}
	*/
}