#pragma once
#include <array>
#include <vector>

namespace FontGenerator
{
	template<typename T>
	std::vector<char> GetBytesOf(T& data)
	{
		std::vector<char> result;
		char* buf = reinterpret_cast<char*>(&data);
		int size = sizeof(T);
		for (int i = 0; i < size; ++i)
		{
			result.push_back(buf[i]);
		}
		return result;
	}

	void AddRange(std::vector<char>& dst, std::vector<char>& src)
	{
		dst.insert(dst.end(), src.begin(), src.end());
	}
}