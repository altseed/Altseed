#pragma once

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include <vector>
#include <string>
#include <set>
#include <map>
#include <list>
#include <algorithm>
#include <functional>

#include <memory>

#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	文字コード
	MSVC		sizeof(wchar_t)==2
	gcc(cygwin)	sizeof(wchar_t)==2
	gcc(linux)	sizeof(wchar_t)==4
*/
namespace ace
{
#ifdef _WIN32
typedef wchar_t achar;
typedef std::wstring astring;
#else 
typedef uint16_t achar;
typedef std::basic_string<uint16_t> astring;
#endif
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define ACE_STDCALL __stdcall
# else
#   define ACE_STDCALL
# endif 

# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define ACE_DLLEXPORT __declspec(dllexport)
# else
#   define ACE_DLLEXPORT
# endif 



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct Vector2DF;
struct Vector2DI;
struct Vector3DF;
struct Matrix43;
struct Matrix44;
struct RectF;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	最大値取得
*/
template <typename T,typename U>
T Max( T t, U u )
{
	if( t > (T)u )
	{
		return t;
	}
	return u;
}

/**
	@brief	最小値取得
*/
template <typename T,typename U>
T Min( T t, U u )
{
	if( t < (T)u )
	{
		return t;
	}
	return u;
}

/**
	@brief	範囲内値取得
*/
template <typename T,typename U,typename V>
T Clamp( T t, U max_, V min_ )
{
	if( t > (T)max_ )
	{
		t = (T)max_;
	}

	if( t < (T)min_ )
	{
		t = (T)min_;
	}

	return t;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
inline float NormalizeAngle(float angle)
{
    int32_t ofs = (*(int32_t*)&angle & 0x80000000) | 0x3F000000; 
    return (angle - ((int)(angle * 0.159154943f + *(float*)&ofs) * 6.283185307f)); 
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
inline void SinCos(float x, float& s, float& c)
{
	x = NormalizeAngle(x);
	float x2 = x * x;
	float x4 = x * x * x * x;
	float x6 = x * x * x * x * x * x;
	float x8 = x * x * x * x * x * x * x * x;
	float x10 = x * x * x * x * x * x * x * x * x * x;
	s = x * (1.0f - x2 / 6.0f + x4 / 120.0f - x6 / 5040.0f + x8 / 362880.0f - x10 / 39916800.0f);
	c = 1.0f - x2 / 2.0f + x4 / 24.0f - x6 / 720.0f + x8 / 40320.0f - x10 / 3628800.0f;
}


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
@brief	文字コードを変換する。(UTF16 -> UTF8)
@param	dst	[out]	出力配列の先頭ポインタ
@param	dst_size	[in]	出力配列の長さ
@param	src			[in]	入力配列の先頭ポインタ
@return	文字数
*/
static int32_t Utf16ToUtf8(int8_t* dst, int32_t dst_size, const int16_t* src)
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

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief	文字コードを変換する。(UTF8 -> UTF16)
	@param	dst			[out]	出力配列の先頭ポインタ
	@param	dst_size	[in]	出力配列の長さ
	@param	src			[in]	入力配列の先頭ポインタ
	@return	文字数
*/
static int32_t Utf8ToUtf16(int16_t* dst, int32_t dst_size, const int8_t* src)
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
		code = (uint8_t)c0 >> 4;
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

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

static std::wstring ToWide(const char* pText)
{
#if _WIN32
	int Len = ::MultiByteToWideChar(CP_ACP, 0, pText, -1, NULL, 0);

	wchar_t* pOut = new wchar_t[Len + 1];
	::MultiByteToWideChar(CP_ACP, 0, pText, -1, pOut, Len);
	std::wstring Out(pOut);
	delete [] pOut;
	return Out;
#else
	int16_t result[4096];
	std::wstring temp;

	Utf8ToUtf16(result, 4096, (int8_t*)pText);

	if (sizeof(wchar_t)==2)
	{
		temp = std::wstring((const wchar_t*)result);
	}
	else if (sizeof(wchar_t)==4)
	{
		wchar_t buf[4096];
		for (int i = 0; i < 4096; i++)
		{
			buf[i] = (int32_t)result[i];
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

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

static astring ToAString(const wchar_t* src)
{
	if (sizeof(wchar_t)== 2)
	{
#ifdef _WIN32
		return astring(src);
#else
		return astring((uint16_t*)src);
#endif
	}
	if (sizeof(wchar_t)== 4)
	{
#ifndef _WIN32
		uint16_t temp[2048];
		int32_t length = 0;
		while (src[length] != 0 && length < 2047)
		{
			temp[length] = (uint16_t)src[length];
			length++;
		}
		temp[length] = 0;
		return astring(temp);
#endif
	}
	return astring();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

static astring ToAString(const char* src)
{
	return ToAString(ToWide(src).c_str());
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

static std::string ToUtf8String(const achar* src)
{
	int8_t result[4096];
	std::string temp;

	Utf16ToUtf8(result, 4096, (int16_t*)src);
	temp = std::string((const char*)result);

	return temp;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

static astring ReplaceAll(const astring text, const achar* from, const achar* to)
{
	astring result = text;
	astring endStr = astring(to);
	astring::size_type pos = 0;
	while (true)
	{
		pos = result.find(from, pos+1);
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

#if !_WIN32 && !SWIG
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static astring ReplaceAll(const astring text, const wchar_t* from, const wchar_t* to)
{
	return ReplaceAll(text, ToAString(from).c_str(), ToAString(to).c_str());
}
#endif


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#ifdef _WIN32
inline void Sleep( int32_t ms )
{
	::Sleep( ms );
}
#else
inline void Sleep( int32_t ms )
{
	usleep( 1000 * ms );
}
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
template <class T>
void SafeAddRef(T& t)
{
	if (t != NULL)
	{
		t->AddRef();
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
template <class T>
void SafeRelease(T& t)
{
	if (t != NULL)
	{
		t->Release();
		t = NULL;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
template <class T>
void SafeSubstitute(T& target, T& value)
{
	SafeAddRef(value);
	SafeRelease(target);
	target = value;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
template <typename T>
inline void SafeDelete(T*& p)
{
	if (p != NULL)
	{
		delete (p);
		(p) = NULL;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
template <typename T>
inline void SafeDeleteArray(T*& p)
{
	if (p != NULL)
	{
		delete[] (p);
		(p) = NULL;
	}
}


const float PI = 3.14159265358979f;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static float DegreeToRadian(float degree)
{
	return degree / 180.0f * PI;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static float RadianToDegree(float radian)
{
	return radian / PI * 180.0f;
}

}

