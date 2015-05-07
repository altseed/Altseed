#include <vector>
#include "ace.Decryptor.h"

namespace ace
{
	Decryptor::Decryptor() { };

	void Decryptor::Decrypt(uint8_t* bytes, int64_t start, int64_t count, const astring& key, int64_t globalPos)
	{
		if (key != astring())
		{
			std::vector<int8_t> key_temp;
			std::vector<uint8_t> key_;

			Utf16ToUtf8(key_temp, (const int16_t*)key.c_str());

			key_.resize(key_temp.size());
			memcpy(key_.data(), key_temp.data(), key_temp.size());
			key_.pop_back();

			for (auto i = start; i < start+count; ++i)
			{
				bytes[i] = (bytes[i] ^ key_[(i+globalPos) % key_.size()]);
			}
		}
	}
}