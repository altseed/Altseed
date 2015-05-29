#include <vector>
#include "ace.Decryptor.h"

namespace ace
{
	Decryptor::Decryptor(const astring& key)
	{
		if (key != astring())
		{
			std::vector<int8_t> key_temp;
			std::vector<uint8_t> key_;

			Utf16ToUtf8(key_temp, (const int16_t*) key.c_str());

			key_.resize(key_temp.size());
			memcpy(key_.data(), key_temp.data(), key_temp.size());
			key_.pop_back();

			for (int loop = 0; loop < 40; loop++)
			{
				for (size_t i = 0; i < key_.size(); i++)
				{
					int k = (int) key_[i];
					k = (k + (loop + i)) % 255;
					keys.push_back((uint8_t) k);
				}
			}
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