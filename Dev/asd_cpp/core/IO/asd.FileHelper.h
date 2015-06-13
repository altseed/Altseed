
#pragma once

#include "../asd.Core.Base.h"

namespace asd
{
	class FileHelper
	{
	private:
	public:
		static bool IsAbsolutePath(const astring& path);

		static astring ToLower(const astring& path);

		static astring ToPackedPath(const astring& path);

		static astring CombineRootPath(const astring& root, const astring& path);

		static astring GetDirectory(const astring& path);

		static astring ToRootDirectoryPath(const astring& path);
	};
}