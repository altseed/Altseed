
#pragma once

#include <asd.common.Base.h>

#if !(defined(_CONSOLE_GAME))

namespace asd
{
	class InstalledFontList
	{
	private:
		static bool isLoaded;

	public:

		struct Font
		{
			astring	Name;
			astring Path;

			Font() = default;
		};

		static void Load();

		static std::vector<Font> Fonts;
	};
}

#endif