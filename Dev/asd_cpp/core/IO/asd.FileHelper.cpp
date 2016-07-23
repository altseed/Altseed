
#include "asd.FileHelper.h"

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
		ret = CombinePath(ToAString("").c_str(), ret.c_str());

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