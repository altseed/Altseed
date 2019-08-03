
#include "asd.FileHelper.h"
#include <iostream>
#include <string>
#include <sstream>

namespace asd
{
	static achar tolower_(achar in)
	{
		if (in <= 'Z' && in >= 'A')
		{
			return in - ('Z' - 'z');
		}
		return in;
	}

	
static std::vector<std::u16string> Split(const std::u16string& s, char16_t delim)
{
	std::vector<std::u16string> elems;
	std::basic_stringstream<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>> ss(s);
	std::u16string item;
	while (getline(ss, item, delim))
	{
		if (!item.empty())
		{
			elems.push_back(item);
		}
	}
	return elems;
}

static std::u16string Replace(std::u16string target, std::u16string from_, std::u16string to_)
{
	std::u16string::size_type Pos(target.find(from_));

	while (Pos != std::u16string::npos)
	{
		target.replace(Pos, from_.length(), to_);
		Pos = target.find(from_, Pos + to_.length());
	}

	return target;
}

static std::u16string RemoveBackPath(const std::u16string path)
{
	if (path.size() == 0)
	{
		return path;
	}

	bool found = false;
	for (size_t i = 0; i < path.size() - 1; i++)
	{
		if (path[i + 0] == u'.' && path[i + 1] == u'.')
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		return path;
	}

	auto paths = Split(Replace(path, u"\\", u"/"), u'/');

	for (int32_t i = 0; i < paths.size();)
	{
		if (paths[i] == u"..")
		{
			if (i > 0)
			{
				paths.erase(paths.begin() + i - 1);
				paths.erase(paths.begin() + i - 1);
				i -= 1;
				continue;
			}
		}
		i++;
	}

	std::u16string ret;

	for (size_t i = 0; i < paths.size(); i++)
	{
		ret += paths[i];

		if (paths.size() - 1 != i)
		{
			ret += u"/";
		}
	}

	return ret;
}

	bool FileHelper::IsAbsolutePath(const astring& path)
	{
#ifdef _WIN32

		// Windows

		// 通常パス
		if (3 <= path.size())
		{
			if (path[1] == L':' && path[2] == '\\') return true;
		}

		// ネットワークパス
		if (2 <= path.size())
		{
			if (path[0] == L'\\' && path[1] == '\\') return true;
		}
#else
		
		// *NIX

		if (1 < path.size())
		{
			if (path[0] == u':') return true;
		}

		// ルート
		if (1 < path.size())
		{
			if (path[0] == u'/') return true;
		}
#endif

		return false;
	}

	astring FileHelper::ToLower(const astring& path)
	{


		astring ret(path);
		
		std::transform(path.begin(), path.end(), ret.begin(), tolower_);

		return ret;
	}


	astring FileHelper::ToPackedPath(const astring& path)
	{
		// 小文字化
		auto ret = ToLower(path);

		// セパレーター統一
		for (size_t i = 0; i < ret.size(); i++)
		{
#ifdef _WIN32
			if (ret[i] == L'\\') ret[i] = L'/';
#else
			if (ret[i] == u'\\') ret[i] = u'/';
#endif
		}

		// ../除去
		ret = RemoveBackPath(ret);

		return ret;
	}

	astring FileHelper::CombineRootPath(const astring& root, const astring& path)
	{
		return CombinePath(root.c_str(), path.c_str());
	}

	astring FileHelper::GetDirectory(const astring& path)
	{
		if (path.size() == 0) return path;

		int32_t sep = path.size() - 1;

		for (; sep >= 0; sep--)
		{
#ifdef _WIN32
			if (path[sep] == L'\\' || path[sep] == L'/') break;
#else
			if (path[sep] == u'\\' || path[sep] == u'/') break;
#endif
		}

		if (sep < 0)
		{
			return path;
		}
		else if (sep == 0)
		{
#ifdef _WIN32
			return ToAString("/");
#else
			return astring(u"/");
#endif
		}

		return path.substr(0, sep + 1);
	}


	astring FileHelper::ToRootDirectoryPath(const astring& path)
	{
		if (path.size() <= 2) return path;

		// セパレーターを付与
		auto ret = path;
		auto last = path[path.size() - 2];

#ifdef _WIN32
		if (last != L'\\' || last != L'/') ret.push_back(L'/');
#else
		if (last != u'\\' || last != u'/') ret.push_back(u'/');
#endif

		// ディレクトリ取得
		return GetDirectory(ret);
	}
}