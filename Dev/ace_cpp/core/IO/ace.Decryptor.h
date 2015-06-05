#pragma once

#include "../ace.Core.Base.h"

namespace ace
{
	class Decryptor
	{
	private:
		std::vector<uint8_t> keys[2];
		int32_t version = 0;

	public:
		Decryptor(const astring& key);

		/**
			@brief	バージョンごとに暗号化方法が違うためバージョン変更
		*/
		void ChangeVersion(int32_t version);

		bool IsValid();

		void Decrypt(uint8_t* bytes, int64_t start, int64_t count, int64_t globalPos);
	};
}