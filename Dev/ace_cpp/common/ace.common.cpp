
#include "ace.common.Base.h"

namespace ace
{
	int32_t Utf16ToUtf8(int8_t* dst, int32_t dst_size, const int16_t* src)
	{
		int32_t cnt = 0;
		const int16_t* wp = src;
		int8_t* cp = dst;

		if (dst_size == 0) return 0;

		dst_size -= 3;

		for (cnt = 0; cnt < dst_size;)
		{
			int16_t wc = *wp++;
			if (wc == 0)
			{
				break;
			}
			if ((wc & ~0x7f) == 0)
			{
				*cp++ = wc & 0x7f;
				cnt += 1;
			}
			else if ((wc & ~0x7ff) == 0)
			{
				*cp++ = ((wc >> 6) & 0x1f) | 0xc0;
				*cp++ = ((wc) & 0x3f) | 0x80;
				cnt += 2;
			}
			else
			{
				*cp++ = ((wc >> 12) & 0xf) | 0xe0;
				*cp++ = ((wc >> 6) & 0x3f) | 0x80;
				*cp++ = ((wc) & 0x3f) | 0x80;
				cnt += 3;
			}
		}
		*cp = '\0';
		return cnt;
	}

	int32_t Utf8ToUtf16(int16_t* dst, int32_t dst_size, const int8_t* src)
	{
		int32_t i, code;
		int8_t c0, c1, c2;

		if (dst_size == 0) return 0;

		dst_size -= 1;

		for (i = 0; i < dst_size; i++)
		{
			int16_t wc;

			c0 = *src++;
			if (c0 == '\0')
			{
				break;
			}
			// UTF8からUTF16に変換
			code = (uint8_t) c0 >> 4;
			if (code <= 7)
			{
				// 8bit文字
				wc = c0;
			}
			else if (code >= 12 && code <= 13)
			{
				// 16bit文字
				c1 = *src++;
				wc = ((c0 & 0x1F) << 6) | (c1 & 0x3F);
			}
			else if (code == 14)
			{
				// 24bit文字
				c1 = *src++;
				c2 = *src++;
				wc = ((c0 & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
			}
			else
			{
				continue;
			}
			dst[i] = wc;
		}
		dst[i] = 0;
		return i;
	}

	std::wstring ToWide(const char* pText)
	{
#if _WIN32
		int Len = ::MultiByteToWideChar(CP_ACP, 0, pText, -1, NULL, 0);

		wchar_t* pOut = new wchar_t[Len + 1];
		::MultiByteToWideChar(CP_ACP, 0, pText, -1, pOut, Len);
		std::wstring Out(pOut);
		delete[] pOut;
		return Out;
#else
		int16_t result[4096];
		std::wstring temp;

		Utf8ToUtf16(result, 4096, (int8_t*) pText);

		if (sizeof(wchar_t) == 2)
		{
			temp = std::wstring((const wchar_t*) result);
		}
		else if (sizeof(wchar_t) == 4)
		{
			wchar_t buf[4096];
			for (int i = 0; i < 4096; i++)
			{
				buf[i] = (int32_t) result[i];
				if (result[i] == 0)
				{
					break;
				}
			}
			temp = std::wstring(buf);
		}

		return temp;
#endif
	}

	astring ToAString(const wchar_t* src)
	{
		if (sizeof(wchar_t) == 2)
		{
#ifdef _WIN32
			return astring(src);
#else
			return astring((uint16_t*) src);
#endif
		}
		if (sizeof(wchar_t) == 4)
		{
#ifndef _WIN32
			uint16_t temp[2048];
			int32_t length = 0;
			while (src[length] != 0 && length < 2047)
			{
				temp[length] = (uint16_t) src[length];
				length++;
			}
			temp[length] = 0;
			return astring(temp);
#endif
		}
		return astring();
	}

	astring ToAString(const char* src)
	{
		return ToAString(ToWide(src).c_str());
	}

	std::string ToUtf8String(const achar* src)
	{
		int8_t result[4096];
		std::string temp;

		Utf16ToUtf8(result, 4096, (int16_t*) src);
		temp = std::string((const char*) result);

		return temp;
	}

	astring ReplaceAll(const astring text, const achar* from, const achar* to)
	{
		astring result = text;
		astring endStr = astring(to);
		astring::size_type pos = 0;
		while (true)
		{
			pos = result.find(from, pos + 1);
			if (pos != astring::npos)
			{
				result = result.replace(pos, endStr.length(), endStr);
			}
			else
			{
				break;
			}
		}

		return result;
	}

	astring CombinePath(const achar* rootPath, const achar* path)
	{
		const int32_t dstLength = 260;
		achar dst[dstLength];

		int rootPathLength = 0;
		while (rootPath[rootPathLength] != 0)
		{
			rootPathLength++;
		}

		int pathLength = 0;
		while (path[pathLength] != 0)
		{
			pathLength++;
		}

		// 両方ともなし
		if (rootPathLength == 0 && pathLength == 0)
		{
			return astring();
		}

		// 片方なし
		if (rootPathLength == 0)
		{
			if (pathLength < dstLength)
			{
				memcpy(dst, path, sizeof(achar) * (pathLength + 1));
				return astring(dst);
			}
			else
			{
				return astring();
			}
		}

		if (pathLength == 0)
		{
			if (rootPathLength < dstLength)
			{
				memcpy(dst, rootPath, sizeof(wchar_t) * (rootPathLength + 1));
				return astring(dst);
			}
			else
			{
				return astring();
			}
		}

		// 両方あり

		// ディレクトリパスまで戻す。
		int PathPosition = rootPathLength;
		while (PathPosition > 0)
		{
			if (rootPath[PathPosition - 1] == L'/' || rootPath[PathPosition - 1] == L'\\')
			{
				break;
			}
			PathPosition--;
		}

		// コピーする
		memcpy(dst, rootPath, sizeof(achar) * PathPosition);
		dst[PathPosition] = 0;

		// 無理やり繋げる
		if (PathPosition + pathLength > dstLength)
		{
			return astring();
		}

		memcpy(&(dst[PathPosition]), path, sizeof(achar) * pathLength);
		PathPosition = PathPosition + pathLength;
		dst[PathPosition] = 0;

		// ../ ..\ の処理
		for (int i = 0; i < PathPosition - 2; i++)
		{
			if (dst[i] == L'.' && dst[i + 1] == L'.' && (dst[i + 2] == L'/' || dst[i + 2] == L'\\'))
			{
				int pos = 0;

				if (i > 1 && dst[i - 2] == L'.')
				{

				}
				else
				{
					for (pos = i - 2; pos > 0; pos--)
					{
						if (dst[pos - 1] == L'/' || dst[pos - 1] == L'\\')
						{
							break;
						}
					}

					for (int k = pos; k < PathPosition; k++)
					{
						dst[k] = dst[k + (i + 3) - pos];
					}
					PathPosition = PathPosition - (i + 3 - pos);
					i = pos - 1;
				}
			}
		}
		dst[PathPosition] = 0;
		return astring(dst);
	}
}