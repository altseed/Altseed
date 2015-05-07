#pragma once

#include "../ace.Core.Base.h"

namespace ace
{
	class Decryptor
	{
	public:
		Decryptor();

		static void Decrypt(uint8_t* bytes, int64_t start, int64_t count, const astring& key, int64_t globalPos);
	};
}