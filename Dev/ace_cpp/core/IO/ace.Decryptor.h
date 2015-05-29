#pragma once

#include "../ace.Core.Base.h"

namespace ace
{
	class Decryptor
	{
	private:
		std::vector<uint8_t> keys;

	public:
		Decryptor(const astring& key);

		bool IsValid();

		void Decrypt(uint8_t* bytes, int64_t start, int64_t count, int64_t globalPos);
	};
}