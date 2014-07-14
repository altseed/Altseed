#include "ace.File_Imp.h"
#include "ace.BaseFile_Imp.h"
#include "ace.BaseFile_Imp.h"
#include "ace.Path_Imp.h"
#include "ace.StaticFile_Imp.h"
#include "ace.StreamFile_Imp.h"
#include <array>
#include <algorithm>
#include <sys/stat.h>

namespace ace
{
	File_Imp::~File_Imp()
	{
	}

	void File_Imp::SetRootDirectories(const astring& path)
	{
		std::array<astring, 1> args = { path };
		SetRootDirectories(args.begin(), args.end());
	}

	void File_Imp::SetRootDirectories(const astring& path, const astring& path2)
	{
		std::array<astring, 2> args = { path, path2 };
		SetRootDirectories(args.begin(), args.end());
	}
	void File_Imp::SetRootDirectories(const astring& path, const astring& path2, const astring& path3)
	{
		std::array<astring, 3> args = { path, path2, path3 };
		SetRootDirectories(args.begin(), args.end());
	}
	template<typename _InIt>
	void File_Imp::SetRootDirectories(_InIt first, _InIt end)
	{
		m_rootPathes.clear();
		std::for_each(first, end, [&](const astring& path)
		{
			m_rootPathes.emplace(std::shared_ptr<RootPath_Imp>(new RootPath_Imp(path)));
		});
	}
	void File_Imp::GetRootDirectories(std::vector<std::reference_wrapper<Path>>& rootPathes) const
	{
		rootPathes.clear();
		for (const auto& root : m_rootPathes)
		{
			rootPathes.push_back(root->m_path);
		}
	}
	void File_Imp::EnumerateFiles(const astring& path) const
	{
	}
	void File_Imp::EnumerateFiles(const astring& path, const astring& searchPattern) const
	{
	}
	void File_Imp::EnumerateFiles(const astring& path, const astring& searchPattern, bool isRecursive) const
	{
	}
	bool File_Imp::Exists(const astring& path) const
	{
		assert(false);
		return true;
	}

	StaticFile* File_Imp::CreateStaticFile(const astring& path)
	{
		if (!Path_Imp::IsAbsolutePath(path))
		{
			for (const auto& root : m_rootPathes)
			{
				if (root->m_isPackFile)
				{
					assert(false);
				}
				else
				{
					ace::Path_Imp fullPath(root->m_path.ToAstring());
					fullPath /= path;
					fullPath.Normalize();
					const auto normalizedPath(fullPath.ToAstring());

					auto fileCash = m_staticFileCash.find(normalizedPath);
					if (fileCash != m_staticFileCash.end() &&
						Valid(fileCash->second))
					{
						return fileCash->second;
					}

					auto baseFileCash = m_fileCash.find(normalizedPath);
					StaticFile_Imp* pstaticFile(nullptr);

					if (baseFileCash == m_fileCash.end())
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
							std::shared_ptr<BaseFile_Imp> pBaseFile(new BaseFile_Imp(normalizedPath), [](BaseFile_Imp* p){ SafeRelease(p); });
							m_fileCash.emplace(normalizedPath, pBaseFile);

							pstaticFile = new StaticFile_Imp(pBaseFile);

							m_staticFileCash.emplace(normalizedPath, pstaticFile);

							return pstaticFile;
						}
					}
					else
					{
						pstaticFile = new StaticFile_Imp(baseFileCash->second);
						m_staticFileCash.emplace(normalizedPath, pstaticFile);

						return pstaticFile;
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

	StreamFile* File_Imp::CreateStreamFile(const astring& path)
	{
		if (!Path_Imp::IsAbsolutePath(path))
		{
			for (const auto& root : m_rootPathes)
			{
				if (root->m_isPackFile)
				{
					assert(false);
				}
				else
				{
					ace::Path_Imp fullPath(root->m_path.ToAstring());
					fullPath /= path;
					fullPath.Normalize();
					const auto normalizedPath(fullPath.ToAstring());

					auto fileCash = m_streamFileCash.find(normalizedPath);
					if (fileCash != m_streamFileCash.end() &&
						Valid(fileCash->second))
					{
						return fileCash->second;
					}

					auto baseFileCash = m_fileCash.find(normalizedPath);
					StreamFile_Imp* pstreamFile(nullptr);

					if (baseFileCash == m_fileCash.end())
					{
						std::shared_ptr<BaseFile_Imp> pBaseFile(new BaseFile_Imp(normalizedPath), [](BaseFile_Imp* p){ SafeRelease(p); });
						m_fileCash.emplace(normalizedPath, pBaseFile);

						pstreamFile = new StreamFile_Imp(pBaseFile);
					}
					else
					{
						pstreamFile = new StreamFile_Imp(baseFileCash->second);
					}

					m_streamFileCash.emplace(normalizedPath, pstreamFile);
					return pstreamFile;
				}
			}
		}
	}
}