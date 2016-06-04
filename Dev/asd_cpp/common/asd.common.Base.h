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
namespace asd
{
#ifdef _WIN32

#ifdef _CHAR16T
	typedef wchar_t achar;
	typedef std::wstring astring;
#else
	typedef char16_t achar;
	typedef std::basic_string<char16_t> astring;
#endif
#else 
typedef char16_t achar;
typedef std::basic_string<char16_t> astring;
#endif
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define ASD_STDCALL __stdcall
# else
#   define ASD_STDCALL
# endif 

# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define ASD_DLLEXPORT __declspec(dllexport)
# else
#   define ASD_DLLEXPORT
# endif 



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct Vector2DF;
struct Vector2DI;
struct Vector3DF;
struct Vector4DF;
struct Matrix43;
struct Matrix44;
struct RectF;
struct RectI;

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
@param	dst	[out]	出力vector
@param	src			[in]	入力配列の先頭ポインタ
@return	文字数
*/
int32_t Utf16ToUtf8(std::vector<int8_t> &dst, const int16_t* src);

/**
	@brief	文字コードを変換する。(UTF8 -> UTF16)
	@param	dst			[out]	出力vector
	@param	src			[in]	入力配列の先頭ポインタ
	@return	文字数
*/
int32_t Utf8ToUtf16(std::vector<int16_t> &dst, const int8_t* src);

std::wstring ToWide(const char* pText);

astring ToAString(const char16_t* src);

astring ToAString(const wchar_t* src);

astring ToAString(const char* src);

std::string ToUtf8String(const achar* src);

astring ReplaceAll(const astring text, const achar* from, const achar* to);

astring CombinePath(const achar* rootPath, const achar* path);

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
#if !SWIG
void ShowMessageBox(const achar* title, const achar* text);

#if _WIN32
#define SPRINTF sprintf_s
#else
#define SPRINTF snprintf
#endif

#define ACE_ASSERT(condition, message) { \
if (!(condition)) { \
	char lbuf[100]; \
	SPRINTF(lbuf, 100, "%d", __LINE__); \
	auto m = ::asd::ToAString(message); \
	auto f = ::asd::ToAString(__FILE__); \
	auto l = ::asd::ToAString(lbuf); \
	auto state = f + ::asd::ToAString("(") + l + ::asd::ToAString(")"); \
	auto m_ = state + ::asd::ToAString("\n") + m; \
	::asd::ShowMessageBox(::asd::ToAString("Assert").c_str(), m_.c_str()); \
	(*((volatile int*)0x0) = 0x0);  } \
}

#define ACE_ASSERT_A(condition, message) { \
if (!(condition)) { \
	char lbuf[100]; \
	SPRINTF(lbuf, 100, "%d", __LINE__); \
	auto m = message; \
	auto f = ::asd::ToAString(__FILE__); \
	auto l = ::asd::ToAString(lbuf); \
	auto state = f + ::asd::ToAString("(") + l + ::asd::ToAString(")"); \
	auto m_ = state + ::asd::ToAString("\n") + m; \
	::asd::ShowMessageBox(::asd::ToAString("Assert").c_str(), m_.c_str()); \
	(*((volatile int*)0x0) = 0x0);  } \
}

#endif

/**
	@brief	static_castとdynamic_castの値が違う場合に警告するassert
*/
#define ASSERT_STATIC_CAST(type,name) assert( static_cast<type>(name) == dynamic_cast<type>(name) )

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

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

