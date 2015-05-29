
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
		m_roots.push_back(std::shared_ptr<FileRoot>(new FileRoot(ToAString("./"), std::shared_ptr<Decryptor>())));
	}

	void File_Imp::AddRootDirectory(const achar* path)
	{
		m_roots.insert(m_roots.begin(), std::shared_ptr<FileRoot>(new FileRoot(path, std::shared_ptr<Decryptor>())));
	}

	void File_Imp::AddRootPackageWithPassword(const achar* path, const achar* password)
	{
		std::shared_ptr<Decryptor> dec = std::shared_ptr<Decryptor>(new Decryptor(astring(password)));

		m_roots.insert(m_roots.begin(), std::shared_ptr<FileRoot>(new FileRoot(path, dec)));
	}

	void File_Imp::AddRootPackage(const achar* path)
	{
		m_roots.insert(m_roots.begin(), std::shared_ptr<FileRoot>(new FileRoot(path, std::shared_ptr<Decryptor>())));
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

			auto cacheKey = astring(path);
			auto it = staticFiles.find(cacheKey);
			if (it != staticFiles.end())
			{
				auto ret = it->second;
				SafeAddRef(ret);
				return ret;
			}

			auto file = CreateSharedPtr(new BaseFile(path));
			if (file->IsValid())
			{
				auto staticFile = new StaticFile_Imp(this, cacheKey, file);
				staticFiles[cacheKey] = staticFile;
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
					auto cacheKey = packedPath;
					auto it = staticFiles.find(cacheKey);
					if (it != staticFiles.end())
					{
						auto ret = it->second;
						SafeAddRef(ret);
						return ret;
					}

					auto packFile = root->GetPackFile();

					if (packFile->HaveFile(packedPath))
					{
						const auto& internalHeader = packFile->GetInternalHeader(packedPath);

						StaticFile_Imp* staticFile = nullptr;

						{
							staticFile = new StaticFile_Imp(this, astring(packedPath), packFile->RawFile(), *internalHeader, root->decryptor);
							staticFiles[cacheKey] = staticFile;
							return staticFile;
						}
					}
				}
				else
				{
					auto combinedPath = FileHelper::CombineRootPath(root->m_path, path);

					auto cacheKey = combinedPath;
					auto it = staticFiles.find(cacheKey);
					if (it != staticFiles.end())
					{
						auto ret = it->second;
						SafeAddRef(ret);
						return ret;
					}

					StaticFile_Imp* staticFile = nullptr;

					auto file = CreateSharedPtr(new BaseFile(combinedPath));
					if (file->IsValid())
					{
						auto staticFile = new StaticFile_Imp(this, astring(combinedPath), file);
						staticFiles[cacheKey] = staticFile;
						return staticFile;
					}
				}
			}
		}

		return nullptr;
	}

	StreamFile* File_Imp::CreateStreamFile_(const achar* path)
	{
		if (FileHelper::IsAbsolutePath(path))
		{
			// 絶対パス
			auto cacheKey = astring(path);
			auto it = streamFiles.find(cacheKey);
			if (it != streamFiles.end())
			{
				auto ret = it->second;
				SafeAddRef(ret);
				return ret;
			}

			auto file = CreateSharedPtr(new BaseFile(path));
			if (file->IsValid())
			{
				auto streamFile = new StreamFile_Imp(this, astring(path), file);
				streamFiles[cacheKey] = streamFile;
				return streamFile;
			}
		}
		else
		{
			auto packedPath = FileHelper::ToPackedPath(path);

			for (const auto& root : m_roots)
			{
				if (root->IsPackFile())
				{
					auto cacheKey = packedPath;
					auto it = streamFiles.find(cacheKey);
					if (it != streamFiles.end())
					{
						auto ret = it->second;
						SafeAddRef(ret);
						return ret;
					}

					auto packFile = root->GetPackFile();

					if (packFile->HaveFile(packedPath))
					{
						const auto& internalHeader = packFile->GetInternalHeader(packedPath);

						StreamFile_Imp* streamFile = nullptr;

						{
							streamFile = new StreamFile_Imp(this, astring(packedPath), packFile->RawFile(), *internalHeader, root->decryptor);
							streamFiles[cacheKey] = streamFile;
							return streamFile;
						}
					}
				}
				else
				{
					auto combinedPath = FileHelper::CombineRootPath(root->m_path, path);

					auto cacheKey = combinedPath;
					auto it = streamFiles.find(cacheKey);
					if (it != streamFiles.end())
					{
						auto ret = it->second;
						SafeAddRef(ret);
						return ret;
					}

					StreamFile_Imp* streamFile = nullptr;

					auto file = CreateSharedPtr(new BaseFile(combinedPath));
					if (file->IsValid())
					{
						auto streamFile = new StreamFile_Imp(this, cacheKey, file);
						streamFiles[cacheKey] = streamFile;
						return streamFile;
					}
				}
			}
		}

		return nullptr;
	}

	void File_Imp::UnregisterStaticFile(const astring& key)
	{
		staticFiles.erase(key);
	}

	void File_Imp::UnregisterStreamFile(const astring& key)
	{
		streamFiles.erase(key);
	}
}