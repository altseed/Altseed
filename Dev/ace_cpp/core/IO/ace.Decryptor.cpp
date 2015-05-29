#include <vector>
#include "ace.Decryptor.h"

namespace ace
{
	Decryptor::Decryptor(const astring& key)
	{
		if (key != astring())
		{
			std::vector<int8_t> key_temp;

			Utf16ToUtf8(key_temp, (const int16_t*) key.c_str());

			keys.resize(key_temp.size());
			memcpy(keys.data(), key_temp.data(), key_temp.size());
			keys.pop_back();
		}
	}

	bool Decryptor::IsValid()
	{
		return keys.size() > 0;
	}

	void Decryptor::Decrypt(uint8_t* bytes, int64_t start, int64_t count, int64_t globalPos)
	{
		if (keys.size() > 0)
		{
			for (auto i = start; i < start+count; ++i)
			{
				bytes[i] = (bytes[i] ^ keys[(i+globalPos) % keys.size()]);
			}
		}
	}
}