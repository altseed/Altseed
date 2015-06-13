#include <vector>
#include "asd.Decryptor.h"

namespace asd
{
	Decryptor::Decryptor(const astring& key)
	{
		if (key != astring())
		{
			// 古いバージョン
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
						keys[0].push_back((uint8_t) k);
					}
				}
			}
			
			// 新しいバージョン
			{
				// 線形合同法
				int32_t seed_ = 1;
				auto rand_ = [&]() -> int32_t
				{
					seed_ = seed_ * 214013 + 2531011;
					return(int) (seed_ >> 16) & 32767;
				};

				auto srand_ = [&](int32_t seed) -> void
				{
					seed_ = seed;
					if (seed == 0) rand_();
				};

				std::vector<int8_t> str_temp;
				std::vector<uint8_t> str_;

				Utf16ToUtf8(str_temp, (const int16_t*) key.c_str());

				str_.resize(str_temp.size());
				memcpy(str_.data(), str_temp.data(), str_temp.size());
				str_.pop_back();

				uint32_t r = 0xFFFFFF;
				for (size_t i = 0; i < str_.size(); i++)
				{
					r ^= (uint32_t) (str_[i] ^ 0x17);

					for (int32_t b = 0; b < 8; b++)
					{
						if ((b & 1) != 0)
						{
							r = (r >> 1) ^ 0x13579A;
						}
						else
						{
							r = r>> 1;
						}
					}
				}

				srand_((int32_t)r);

				for (int32_t i = 0; i < 2048; i++)
				{
					keys[1].push_back((uint8_t) rand_());
				}
			}
		}
	}

	void Decryptor::ChangeVersion(int32_t version)
	{
		if (this->version > 0)
		{
			assert(0);
		}

		this->version = version;
	}

	bool Decryptor::IsValid()
	{
		return keys[version].size() > 0;
	}

	void Decryptor::Decrypt(uint8_t* bytes, int64_t start, int64_t count, int64_t globalPos)
	{
		auto& keys_ = keys[version];

		if (keys_.size() > 0)
		{
			for (auto i = start; i < start+count; ++i)
			{
				bytes[i] = (bytes[i] ^ keys_[(i + globalPos) % keys_.size()]);
			}
		}
	}
}