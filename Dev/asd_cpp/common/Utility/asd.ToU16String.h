#pragma once
#include<locale>
#include<codecvt>
#include<string>

#ifdef _MSC_VER
#include <uchar.h>
#endif

namespace asd {
	// 参照: http://stackoverflow.com/questions/34211134/how-to-convert-an-integer-to-stdu16string-c11

	/**
		@brief	数値などをu16文字列に変換して返す
		@details	std::to_stringによって変換できる型をu16文字列に変換する
		@param	arg	[in]	変換したい値
		@return  変換後のu16文字列
	*/
	template<typename T> std::u16string ToU16String(T const &arg) {
		
#ifndef _MSC_VER
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> c;
		return c.from_bytes(std::to_string(arg));
#else
		// workaround for msvc's bug
		auto const src = std::to_string(arg);
		char const* p = src.data();
		auto const size = src.size();

		std::u16string ret;
		ret.reserve(size * 2);
		char16_t tmp;

		mbstate_t stat{};

		for (int itr = 0; (itr = mbrtoc16(&tmp, p, size, &stat)) != 0; p += itr) {
			if (itr == -1) {
				assert(!"illegal string at ToU16String");
				break;
			}
			ret.push_back(tmp);
		}

		ret.shrink_to_fit();
		return ret;
#endif
	}
}