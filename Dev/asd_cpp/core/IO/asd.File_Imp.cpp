
#include "asd.File_Imp.h"
#include "asd.BaseFile.h"
#include "asd.StaticFile_Imp.h"
#include "asd.StreamFile_Imp.h"
#include "StaticFile/BlockingStaticFileFactory.h"
#include "StaticFile/AsyncStaticFileFactory.h"

#include <array>
#include <algorithm>
#include <unordered_set>
#include <new>
#include <time.h>

#if defined(_CONSOLE_GAME)

#elif defined(_WIN32)

#else
#include <sys/stat.h>
#endif

namespace asd
{
	File_Imp::File_Imp(Synchronizer::Ptr sync)
	{
		AddDefaultRootDirectory();

		staticFileLoader = std::make_shared<StaticFileLoader>(this,
			std::make_shared<BlockingStaticFileFactory>(this));
		asyncFileLoader = std::make_shared<StaticFileLoader>(this,
			std::make_shared<AsyncStaticFileFactory>(this, sync));
	}

	File_Imp::~File_Imp()
	{
	}

	void File_Imp::AddDefaultRootDirectory()
	{
		m_roots.push_back(std::shared_ptr<FileRoot>(new FileRoot(ToAString("./"), std::shared_ptr<Decryptor>())));
	}

	void File_Imp::AddRootDirectory(const char16_t* path)
	{
		m_roots.insert(m_roots.begin(), std::shared_ptr<FileRoot>(new FileRoot(path, std::shared_ptr<Decryptor>())));
	}

	void File_Imp::AddRootPackageWithPassword(const char16_t* path, const char16_t* password)
	{
		std::shared_ptr<Decryptor> dec = std::shared_ptr<Decryptor>(new Decryptor(astring(password)));

		m_roots.insert(m_roots.begin(), std::shared_ptr<FileRoot>(new FileRoot(path, dec)));
	}

	void File_Imp::AddRootPackage(const char16_t* path)
	{
		m_roots.insert(m_roots.begin(), std::shared_ptr<FileRoot>(new FileRoot(path, std::shared_ptr<Decryptor>())));
	}
	
	void File_Imp::ClearRootDirectories()
	{
		m_roots.clear();
		AddDefaultRootDirectory();
	}

	bool File_Imp::Exists(const char16_t* path) const
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

	StaticFile* File_Imp::CreateStaticFile(const char16_t* path)
	{
		return CreateStaticFile(path, staticFileLoader);
	}

	StaticFile * File_Imp::CreateStaticFileAsync(const char16_t * path)
	{
		return CreateStaticFile(path, asyncFileLoader);
	}

	StaticFile* File_Imp::CreateStaticFile(const char16_t * path, StaticFileLoader::Ptr loader)
	{
		if (FileHelper::IsAbsolutePath(path))
		{
			return loader->LoadFromAbsolutePath(path);
		}
		else
		{
			auto packedPath = FileHelper::ToPackedPath(path);

			for (const auto& root : m_roots)
			{
				if (root->IsPackFile())
				{
					return loader->LoadFromPackedRoot(path, packedPath, root);
				}
				else
				{
					return loader->LoadFromNonPackedRoot(path, root);
				}
			}
		}

		return nullptr;
	}

	StreamFile* File_Imp::CreateStreamFile(const char16_t* path)
	{
		std::lock_guard<std::recursive_mutex> lock(mtx_);

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
		staticFileLoader->UnregisterStaticFile(key);
	}

	void File_Imp::UnregisterStreamFile(const astring& key)
	{
		std::lock_guard<std::recursive_mutex> lock(mtx_);

		streamFiles.erase(key);
	}
}
