
#include "ace.File_Imp.h"
#include "ace.BaseFile.h"
#include "ace.StaticFile_Imp.h"
#include "ace.StreamFile_Imp.h"

#include <array>
#include <algorithm>
#include <sys/stat.h>
#include <unordered_set>

namespace ace
{
	File_Imp::File_Imp()
	{
		AddDefaultRootDirectory();
	}

	File_Imp::~File_Imp()
	{
	}

	void File_Imp::AddDefaultRootDirectory()
	{
		m_roots.push_back(std::shared_ptr<FileRoot>(new FileRoot(ToAString("./"))));
	}

	void File_Imp::AddRootDirectory(const achar* path)
	{
		m_roots.insert(m_roots.begin(), std::shared_ptr<FileRoot>(new FileRoot(path)));
	}

	void File_Imp::AddRootPackage(const achar* path, const achar* key)
	{
		// TODO key
		m_roots.insert(m_roots.begin(), std::shared_ptr<FileRoot>(new FileRoot(path)));
	}
	
	void File_Imp::ClearRootDirectories()
	{
		m_roots.clear();
		AddDefaultRootDirectory();
	}

	bool File_Imp::Exists(const achar* path) const
	{
		if (FileHelper::IsAbsolutePath(path))
		{
			// 絶対パス
			auto file = CreateSharedPtr(new BaseFile(path));
			if (file->IsValid())
			{
				return true;
			}
		}
		else
		{
			auto packedPath = FileHelper::ToPackedPath(path);

			for (const auto& root : m_roots)
			{
				if (root->IsPackFile())
				{
					auto packFile = root->GetPackFile();

					if (packFile->HaveFile(packedPath))
					{
						return true;
					}
				}
				else
				{
					auto rootDir = FileHelper::GetDirectory(root->m_path);
					auto combinedPath = FileHelper::CombineRootPath(rootDir, path);

					auto file = CreateSharedPtr(new BaseFile(combinedPath));
					if (file->IsValid())
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	StaticFile* File_Imp::CreateStaticFile_(const achar* path)
	{
		if (FileHelper::IsAbsolutePath(path))
		{
			// 絶対パス
			auto file = CreateSharedPtr(new BaseFile(path));
			if (file->IsValid())
			{
				auto staticFile = new StaticFile_Imp(file);
				return staticFile;
			}
		}
		else
		{
			auto packedPath = FileHelper::ToPackedPath(path);
	
			for (const auto& root : m_roots)
			{
				if (root->IsPackFile())
				{
					auto packFile = root->GetPackFile();

					if (packFile->HaveFile(packedPath))
					{
						const auto& internalHeader = packFile->GetInternalHeader(packedPath);

						StaticFile_Imp* staticFile = nullptr;

						{
							staticFile = new StaticFile_Imp(packFile->RawFile(), *internalHeader);
							return staticFile;
						}
					}
				}
				else
				{
					auto combinedPath = FileHelper::CombineRootPath(root->m_path, path);

					StaticFile_Imp* staticFile = nullptr;

					auto file = CreateSharedPtr(new BaseFile(combinedPath));
					if (file->IsValid())
					{
						auto staticFile = new StaticFile_Imp(file);
						return staticFile;
					}
				}
			}
		}

		return nullptr;
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