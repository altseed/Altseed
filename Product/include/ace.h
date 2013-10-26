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




//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
		@brief	全ての参照カウンタの値の合計を取得する。
		@return	参照カウンタ合計値
	*/
	int32_t GetGlobalReferenceCount();

	/**
		@brief	全ての参照カウンタの値を取得し、0ならDLLを解放する。
	*/
	void CheckDLL();

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

#include<deque>
#include<cstdlib>
#include<algorithm>
#include<cstdint>
#include<climits>
#include<cassert>
namespace ace
{

class BinaryReader
{
protected:
	std::deque<int8_t> data;
public:
	template<typename itrT> void ReadIn(itrT beg, itrT end)
	{
		//std::copy<itrT, std::deque<int8_t>::iterator>(beg, end, data.begin());
		for (itrT itr = beg; itr != end; itr++)
		{
			data.push_back(*itr);
		}

	}

	template<typename T> T Get(){ assert(!"The type cannot be serializable."); return T();};
	bool IsEmpty()const{ return data.empty(); }

};
template<> inline int32_t BinaryReader::Get()
{
	int8_t cs[4];
	for (int i = 0; i < 4; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return static_cast<int32_t>(0);
		}
		cs[i] = data.front();
		data.pop_front();
	}

	return *(static_cast<int32_t*>(static_cast<void*>(cs)));
}

template<> inline int16_t BinaryReader::Get()
{
	int8_t cs[2];
	for (int i = 0; i < 2; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return static_cast<int16_t>(0);
		}
		cs[i] = data.front();
		data.pop_front();
	}

	return *(static_cast<int16_t*>(static_cast<void*>(cs)));
}

template<> inline int8_t BinaryReader::Get()
{
	int8_t c;
	assert(!data.empty());
	if (data.empty()){
		return static_cast<int8_t>(0);
	}
	c = data.front();
	data.pop_front();

	return c;
}

template<> inline std::string BinaryReader::Get()
{
	int8_t lenCs[4];
	std::string str = std::string();
	for (int i = 0; i < 4; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return str;
		}
		lenCs[i] = data.front();
		data.pop_front();
	}

	int32_t const len = *(static_cast<int32_t*>(static_cast<void*>(lenCs)));

	int8_t chr;

	for (int32_t i = 0; i < len; i++)
	{

		assert(!data.empty());
		if (data.empty()){
			return str;
		}
		chr = data.front();
		data.pop_front();

		str.push_back(*(static_cast<char*>(static_cast<void*>(&chr))));

	}
	return str;
}
template<> inline ace::astring BinaryReader::Get()
{
	int8_t lenCs[4];
	ace::astring astr = ace::astring();
	for (int i = 0; i < 4; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return astr;
		}
		lenCs[i] = data.front();
		data.pop_front();
	}

	int32_t const len =  *(static_cast<int32_t*>(static_cast<void*>(lenCs)));

	int8_t charCs[2];

	for (int32_t i = 0; i < len; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			assert(!data.empty());
			if (data.empty()){
				return astr;
			}
			charCs[j] = data.front();
			data.pop_front();
		}

		astr.push_back(*(static_cast<ace::achar*>(static_cast<void*>(charCs))));

	}
	return astr;
}

template<> inline ace::achar* BinaryReader::Get()
{
	int8_t lenCs[4];
	for (int i = 0; i < 4; i++)
	{
		assert(!data.empty());
		if (data.empty()){
			return nullptr;
		}
		lenCs[i] = data.front();
		data.pop_front();
	}

	int32_t const len = *(static_cast<int32_t*>(static_cast<void*>(lenCs)));

	ace::achar *achs = static_cast<ace::achar*>(calloc(len + 1, sizeof(ace::achar)));
	int8_t charCs[2];

	for (int32_t i = 0; i < len; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			assert(!data.empty());
			if (data.empty()){
				return achs;
			}
			charCs[j] = data.front();
			data.pop_front();
		}

		achs[i] = *(static_cast<ace::achar*>(static_cast<void*>(charCs)));

	}
	return achs;
}


} 
#include<vector>
#include<cstdint>
#include<climits>
#include<fstream>
#include<string>

namespace ace
{

/**
@brief	数値、文字列をバイト列にシリアライズし、ファイルに書き出すクラス
@detail	対応する型はint32_t, int16_t, int8_t, std::string, astring, achar*
@warning	エンディアンは現状非互換、正当性の検査等はないので、
データを読み出す際はBinaryReaderで*書き出した順に*読み出すこと
*/
class BinaryWriter
{
protected:
	std::vector<int8_t> m_data; ///< シリアライズされたバイト列
public:
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	扱うデータが大きいことが予想される場合にあらかじめ容量を確保する
	@param	size	確保するサイズ
	*/

	void Reserve(size_t size)
	{
		m_data.reserve(size);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	シリアライズされたバイト列をconst参照で返す。
	@warning	本クラスのインスタンスより寿命を長く取る際はコピーなどして懸垂参照を避けること
	*/

	std::vector<int8_t> const& Get() const
	{
		return m_data;
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	int32_tをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする整数
	*/
	void Push(int32_t content){
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&content));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	int16_tをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする整数
	*/
	
	void Push(int16_t content){
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&content));
		for (int i = 0; i < 2; i++)
		{
			m_data.push_back(pc[i]);
		}
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	int8_tをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする整数
	*/
	void Push(int8_t content){

		m_data.push_back(content);

	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	std::stringをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする文字列
	*/
	void Push(std::string const& content)
	{
		// TODO: string::lengthの返値が64bitでもたれていた場合に危険
		int32_t l = (int32_t)content.length();

		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&l));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
		for (int32_t i = 0; i < l; i++)
		{
			m_data.push_back(static_cast<int8_t>(content.at(i)));
		}

	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	astringをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする文字列
	*/
	void Push(ace::astring const& content)
	{
		auto l = content.length();

		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&l));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
		for (uint32_t i = 0; i < l; i++)
		{
			//copy
			achar a = content.at(i);
			int8_t* pcs = static_cast<int8_t*>(static_cast<void*>(&a));
			m_data.push_back(static_cast<int8_t>(pcs[0]));
			m_data.push_back(static_cast<int8_t>(pcs[1]));
		}

	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	achar*をシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする文字列の先頭ポインタ
	@param	size	シリアライズする文字列のサイズ
	*/
	void Push(ace::achar const* content, int32_t size)
	{
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&size));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
		for (int32_t i = 0; i < size; i++)
		{
			//copy
			achar a = content[i];
			int8_t* pcs = static_cast<int8_t*>(static_cast<void*>(&a));
			m_data.push_back(static_cast<int8_t>(pcs[0]));
			m_data.push_back(static_cast<int8_t>(pcs[1]));
		}

	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	保持しているバイト列をストリームに書き出す。
	@param	os	書き出すストリーム
	@return 成功すればtrue, 失敗すればfalse
	*/

	bool WriteOut(std::ostream& os) const
	{
		if (!os){ return false; }
		for (auto itr = m_data.cbegin(); itr != m_data.cend(); itr++)
		{
			os << *itr;
		}
		return true;
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	保持しているバイト列をファイルに書き出す。
	@param	filename	書き出すファイル名
	@param	isAppend	追記するか否か
	@return 成功すればtrue, 失敗すればfalse
	*/
	bool WriteOut(std::string const& filename, bool isAppend = false) const
	{
		std::ofstream os(
			filename,
			std::ios_base::binary | std::ios_base::out | (isAppend ? std::ios_base::app : std::ios_base::out)
		);
		if (!os) {
			return false;
		}
		if (!WriteOut(os)) { return false; }
		os.close();
		return true;
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	デストラクタ
	*/
	virtual ~BinaryWriter(){}
};
}
#include<string>
#if defined(_MSC_VER)

#elif defined(__clang__)
#include<cxxabi.h>
#include<cstdlib>
#include<cassert>
#if !__has_include(<cxxabi.h>)
#error "cxxabi.h is not found"
#endif
#elif defined(__GNUC__)
#include<cxxabi.h>
#include<cstdlib>
#include<cassert>
#endif

namespace ace
{
/**
	@brief	型から型名を人間の読める形の文字列として得る
	@detail	対応コンパイラ: msvc, gcc, clang.
*/
template<typename T> std::string GetTypeName()
{
#ifdef _MSC_VER
#ifndef _CPPRTTI
#error "/GR option is needed to get type names"
#endif
	return std::string(typeid(T).name());
#else
	int stat;
	std::string ret;
	char *pc = abi::__cxa_demangle(typeid(T).name(), 0, 0, &stat);
	switch (stat)
	{
	case 0:
		ret = std::string(pc);
		free(pc);
		return ret;
		break;
	case -1:
		assert(!"failed to memory allocation");
		return ret;
		break;
	case -2:
		assert(!"the type name couldn't be demangled");
		return ret;
		break;
	case -3:
		assert(!"there's an illegal argument");
		return ret;
		break;
	default:
		return ret;
		break;
	}
#endif
}

}
#include<cstddef>
#include<cassert>
namespace ace
{

/**
	@brief	確保されたメモリ領域の先頭ポインタと長さから、デバッグ時境界検査つき配列を提供する
	@details	確保されたメモリの所有権は移動しないので、使用する側でメモリを開放する必要がある
*/

template<typename T> class PointerArray
{
public:
	T* m_data; //!< 配列の先頭ポインタ
protected:
	size_t m_length; //!< 配列の長さ
public:
	typedef T Type; //!< 対象となっている型
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
		@brief	配列の長さを得る
	*/
	size_t GetLength() const{return m_length;}
	
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
		@brief	コンストラクタ
		@detail	T*はlength確保した配列と見なす。所有権は移動しない
		@param	idx	[in]	
	*/
	//! 
	PointerArray(T* ptr, size_t length): m_data(ptr), m_length(length){}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
		@brief	idx番目の要素への参照を得る
		@detail	デバッグ時には範囲外チェックがつく
		@param	idx	[in]	要素のインデックス
	*/
	T& At(size_t idx)
	{
		assert(idx >= 0 && idx < m_length);
		return m_data[idx];
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
		@brief	idx番目の要素への参照を得る
		@detail	デバッグ時には範囲外チェックがつく
		@param	idx	[in]	要素のインデックス
	*/
	T& operator[](size_t idx)
	{
		return At(idx);
	}
};
}
#include<cstdint>

#ifdef _WIN32
/* win */
#include<Windows.h>
#else
/* *nix */
#include <sys/time.h>
#endif

namespace ace
{
/**
	@brief	パフォーマンスカウンタの現在の値をナノ秒単位で返す
*/

inline int64_t GetTime()
{
#ifdef _WIN32
	int64_t count, freq;
	if (QueryPerformanceCounter((LARGE_INTEGER*)&count))
	{
		if (QueryPerformanceFrequency((LARGE_INTEGER*)&freq))
		{
			return count * 1000000 / freq;
		}
	}
	return 0;
#else
	struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000000 + (int64_t)tv.tv_usec;
#endif
}
}
#include<cassert>
#include<cstring>
namespace ace
{

/**
	@brief	型は異なるが同じ構造を持つ2変数の間でコピーする
	@details	T, Uが同じサイズかを検査し、そのサイズ分fromからtoにコピーする
	@warning	とても危険な関数なため、使用の際には細心の注意を要する。
	@param	from	[in]	コピー元のポインタ
	@param	to		[out]	コピー先のポインタ
*/
template<typename T, typename U> void TypeErasureCopy(T const* from, U* to)
{
	
#if __cplusplus >= 201103L
	static_assert(alignof(T) == alignof(U), "arguments 'from' and 'to' must be the same alignment.");
	static_assert(sizeof(T) == sizeof(U), "arguments 'from' and 'to' must be the same size.");
#else
	assert(sizeof(T) == sizeof(U));
#endif 
	memcpy(to, from, sizeof(T));
}
}


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief	2次元ベクトル
*/
struct Vector2DF
{
public:
	/**
		@brief	X
	*/
	float	X;

	/**
		@brief	Y
	*/
	float	Y;

	/**
		@brief	コンストラクタ
	*/
	Vector2DF();

	/**
		@brief	コンストラクタ
	*/
	Vector2DF( float x, float y );

	Vector2DF operator+(const Vector2DF& right);

	Vector2DF operator-(const Vector2DF& right);

	Vector2DF operator*(float right);

	Vector2DF& operator+=(const Vector2DF& right);

	Vector2DF& operator-=(const Vector2DF& right);

	Vector2DF& operator*=(float right);
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief	2次元ベクトル
*/
struct Vector2DI
{
public:
	/**
		@brief	X
	*/
	int32_t	X;

	/**
		@brief	Y
	*/
	int32_t	Y;

	/**
		@brief	コンストラクタ
	*/
	Vector2DI();

	/**
		@brief	コンストラクタ
	*/
	Vector2DI( int32_t x, int32_t y );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	3次元ベクトル
*/
struct Vector3DF
{
public:
	/**
		@brief	X
	*/
	float	X;

	/**
		@brief	Y
	*/
	float	Y;

	/**
		@brief	Z
	*/
	float	Z;

	/**
		@brief	コンストラクタ
	*/
	Vector3DF();

	/**
		@brief	コンストラクタ
	*/
	Vector3DF( float x, float y, float z );

	Vector3DF operator + ( const Vector3DF& o ) const;

	Vector3DF operator - ( const Vector3DF& o ) const;

	Vector3DF operator * ( const float& o ) const;

	Vector3DF operator / ( const float& o ) const;

	Vector3DF& operator += ( const Vector3DF& o );

	Vector3DF& operator -= ( const Vector3DF& o );

	Vector3DF& operator *= ( const float& o );

	Vector3DF& operator /= ( const float& o );

	/**
		@brief	加算
	*/
	static void Add( Vector3DF* pOut, const Vector3DF* pIn1, const Vector3DF* pIn2 );

	/**
		@brief	減算
	*/
	static Vector3DF& Sub( Vector3DF& o, const Vector3DF& in1, const Vector3DF& in2 );

	/**
		@brief	長さ
	*/
	static float Length( const Vector3DF& in );

	/**
		@brief	長さの二乗
	*/
	static float LengthSq( const Vector3DF& in );

	/**
		@brief	内積
	*/
	static float Dot( const Vector3DF& in1, const Vector3DF& in2 );

	/**
		@brief	単位ベクトル
	*/
	static void Normal( Vector3DF& o, const Vector3DF& in );

	/**
		@brief	外積
		@note
		右手系の場合、右手の親指がin1、人差し指がin2としたとき、中指の方向を返す。<BR>
		左手系の場合、左手の親指がin1、人差し指がin2としたとき、中指の方向を返す。<BR>
	*/
	static Vector3DF& Cross( Vector3DF& o, const Vector3DF& in1, const Vector3DF& in2 );

	static Vector3DF& Transform( Vector3DF& o, const Vector3DF& in, const Matrix43& mat );

	static Vector3DF& Transform( Vector3DF& o, const Vector3DF& in, const Matrix44& mat );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


namespace ace
{
	/**
		@brief	3×3行列を表す構造体。
		@note
		[0,0][0,1]
		[1,0][1,1]
		*/
	struct Matrix33
	{
	public:
		/**
			@brief	オブジェクトを生成し、単位行列で初期化する。
			*/
		Matrix33();

		/**
			@brief	行列の値
			*/
		float Values[3][3];

		/**
			@brief	単位行列を設定する。
			*/
		Matrix33& SetIdentity();

		/**
			@brief	平行移動の行列を設定する。
			*/
		Matrix33& SetTranslation(float x, float y);

		/**
			@brief	拡大・縮小行列を設定する。
			*/
		Matrix33& SetScaling(float x, float y);

		/**
			@brief	回転行列を設定する。
			*/
		Matrix33& SetRotation(float angle);

		/**
			@brief	3×3行列の掛け算を計算する。
			*/
		Matrix33 operator*(const Matrix33& right);

		Vector3DF operator*(const Vector3DF& right);
	};
}


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief	行列
	@note
	右手系(回転:反時計回り)<BR>
	左手系(回転:時計回り)<BR>
	V[x,y,z,1] * M の形<BR>
	[0,0][0,1][0,2]
	[1,0][1,1][1,2]
	[2,0][2,1][2,2]
	[3,0][3,1][3,2]
*/
struct Matrix43
{
private:

public:
	/**
		@brief	行列の値
	*/
	float	Values[4][3];
	
	/**
		@brief	単位行列化
	*/
	void Indentity();

	/**
		@brief	拡大行列化
	*/
	void Scaling( float x, float y, float z );

	/**
		@brief	X軸回転行列
	*/
	void RotationX( float angle );

	/**
		@brief	Y軸回転行列
	*/
	void RotationY( float angle );

	/**
		@brief	Z軸回転行列
	*/
	void RotationZ( float angle );
	
	/**
		@brief	XYZ軸回転行列
	*/
	void RotationXYZ( float rx, float ry, float rz );
	
	/**
		@brief	ZXY軸回転行列
	*/
	void RotationZXY( float rz, float rx, float ry );

	/**
		@brief	任意軸反時計回転行列
	*/
	void RotationAxis( const Vector3DF& axis, float angle );

	/**
		@brief	任意軸反時計回転行列
	*/
	void RotationAxis( const Vector3DF& axis, float s, float c );

	/**
		@brief	移動行列化
	*/
	void Translation( float x, float y, float z );

	/**
		@brief	行列の拡大、回転、移動の分解
	*/
	void GetSRT( Vector3DF& s, Matrix43& r, Vector3DF& t ) const; 

	/**
		@brief	行列の拡大、回転、移動を設定する。
	*/
	void SetSRT( const Vector3DF& s, const Matrix43& r, const Vector3DF& t );

	/**
		@brief	乗算
	*/
	static void Multiple( Matrix43& out, const Matrix43& in1, const Matrix43& in2 );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief	行列
	@note
	右手系<BR>
	左手系<BR>
	V[x,y,z,1] * M の形<BR>
	[0,0][0,1][0,2][0,3]
	[1,0][1,1][1,2][1,3]
	[2,0][2,1][2,2][2,3]
	[3,0][3,1][3,2][3,3]
*/
struct Matrix44
{
private:

public:

	/**
		@brief	コンストラクタ
	*/
	Matrix44();

	/**
		@brief	行列の値
	*/
	float	Values[4][4];

	/**
		@brief	単位行列化
	*/
	Matrix44& Indentity();

	/**
		@brief	カメラ行列化(右手系)
	*/
	Matrix44& LookAtRH( const Vector3DF& eye, const Vector3DF& at, const Vector3DF& up );

	/**
		@brief	カメラ行列化(左手系)
	*/
	Matrix44& LookAtLH( const Vector3DF& eye, const Vector3DF& at, const Vector3DF& up );

	/**
		@brief	射影行列化(右手系)
	*/
	Matrix44& PerspectiveFovRH( float ovY, float aspect, float zn, float zf );

	/**
		@brief	OpenGL用射影行列化(右手系)
	*/
	Matrix44& PerspectiveFovRH_OpenGL( float ovY, float aspect, float zn, float zf );

	/**
		@brief	射影行列化(左手系)
	*/
	Matrix44& PerspectiveFovLH( float ovY, float aspect, float zn, float zf );

	/**
		@brief	正射影行列化(右手系)
	*/
	Matrix44& OrthographicRH( float width, float height, float zn, float zf );

	/**
		@brief	正射影行列化(左手系)
	*/
	Matrix44& OrthographicLH( float width, float height, float zn, float zf );

	/**
		@brief	拡大行列化
	*/
	void Scaling( float x, float y, float z );

	/**
		@brief	X軸回転行列(右手)
	*/
	void RotationX( float angle );

	/**
		@brief	Y軸回転行列(右手)
	*/
	void RotationY( float angle );

	/**
		@brief	Z軸回転行列(右手)
	*/
	void RotationZ( float angle );

	/**
		@brief	移動行列
	*/
	void Translation( float x, float y, float z );

	/**
		@brief	任意軸反時計回転行列
	*/
	void RotationAxis( const Vector3DF& axis, float angle );

	/**
		@brief	クオータニオンから行列に変換
	*/
	void Quaternion( float x, float y, float z, float w );

	/**
		@brief	乗算
	*/
	static Matrix44& Mul( Matrix44& o, const Matrix44& in1, const Matrix44& in2 );

	/**
		@brief	逆行列
	*/
	static Matrix44& Inverse( Matrix44& o, const Matrix44& in );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	四角形
*/
struct RectF
{
private:

public:
	float	X;

	float	Y;

	float	Width;

	float	Height;

	RectF();

	RectF( float x, float y, float width, float height );

	Vector2DF Position() const;

	Vector2DF Size() const;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	色
	*/
	struct Color
	{
	public:

		/**
		@brief	赤
		*/
		uint8_t	R;

		/**
		@brief	緑
		*/
		uint8_t	G;

		/**
		@brief	青
		*/
		uint8_t	B;

		/**
		@brief	透明度
		*/
		uint8_t	A;

		/**
		@brief	コンストラクタ
		*/
		Color();

		/**
		@brief	コンストラクタ
		@param	r	赤
		@param	g	緑
		@param	b	青
		@param	a	透明度
		*/
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Core;
	class Graphics;
	class Window;
	class Keyboard;
	class Mouse;
	class Log;
	class Profiler;
	class ProfilerViewer;
	class Renderer2D;

	class Joystick;
	class JoystickContainer;

	class Texture2D;
	class RenderTexture2D;
	class Shader2D;
	class Material2D;

	class ObjectSystemFactory;
	class ICoreObject2D;
	class CoreObject2D;
	class CoreTextureObject2D;
	class CoreLayer;
	class CoreLayer2D;
	class CoreScene;
	class CorePostEffect;

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	enum eTextureFormat
	{
		TEXTURE_FORMAT_RGBA8888 = 0,
	};

	enum eTextureClassType
	{
		TEXTURE_CLASS_TEXTURE2D = 0,
		TEXTURE_CLASS_RENDERTEXTURE = 1,
	};

	enum eAlphaBlend
	{
		/// <summary>
		/// 不透明
		/// </summary>
		ALPHA_BLEND_OPACITY = 0,
		/// <summary>
		/// 透明
		/// </summary>
		ALPHA_BLEND_BLEND = 1,
		/// <summary>
		/// 加算
		/// </summary>
		ALPHA_BLEND_ADD = 2,
		/// <summary>
		/// 減算
		/// </summary>
		ALPHA_BLEND_SUB = 3,
		/// <summary>
		/// 乗算
		/// </summary>
		ALPHA_BLEND_MUL = 4,

		ALPHA_BLEND_DWORD = 0x7fffffff,
	};

	enum eTextureFilterType
	{
		TEXTURE_FILTER_NEAREST = 0,
		TEXTURE_FILTER_LINEAR = 1,
		TEXTURE_FILTER_DWORD = 0x7fffffff,
	};

	enum eTextureWrapType
	{
		TEXTURE_WRAP_REPEAT = 0,
		TEXTURE_WRAP_CLAMP = 1,

		TEXTURE_WRAP_DWORD = 0x7fffffff,
	};

	enum eCullingType
	{
		CULLING_FRONT = 0,
		CULLING_BACK = 1,
		CULLING_DOUBLE = 2,

		CULLING_DWORD = 0x7fffffff,
	};

	enum eGraphicsType
	{
		GRAPHICS_TYPE_DX11,
		GRAPHICS_TYPE_GL,
	};

	/**
		@brief	シェーダーに対して外部から設定可能なプロパティの変数の種類
	*/
	enum ShaderVariableType
	{
		SHADER_VARIABLE_TYPE_FLOAT,
		SHADER_VARIABLE_TYPE_VECTOR2DF,
		SHADER_VARIABLE_TYPE_VECTOR3DF,
		SHADER_VARIABLE_TYPE_TEXTURE2D,
	};

	/**
		@brief	シェーダーに対して外部から設定可能なプロパティの情報
	*/
	class ShaderVariableProperty
	{
	public:
		/**
			@brief	名称
		*/
		astring	Name;

		/**
			@brief	種類
		*/
		ShaderVariableType Type;

		/**
			@brief	定数バッファ上での先頭からのオフセット
		*/
		int32_t	Offset;
	};

	/**
	@brief	参照カウンタのインターフェース
	*/
	class IReference
	{
	public:
		/**
			@brief	参照カウンタを加算する。
			@return	加算後の参照カウンタ
		*/
		virtual int AddRef() = 0;

		/**
		@brief	参照カウンタを取得する。
		@return	参照カウンタ
		*/
		virtual int GetRef() = 0;

		/**
		@brief	参照カウンタを減算する。0になった時、インスタンスを削除する。
		@return	減算後の参照カウンタ
		*/
		virtual int Release() = 0;
	};


	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <memory>


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	参照カウンタオブジェクト
	*/
	class ReferenceObject
		: public IReference
	{
	private:
		int32_t m_reference;

	public:
		ReferenceObject();

		virtual ~ReferenceObject();

		virtual int AddRef();

		virtual int GetRef();

		virtual int Release();
	};

#if !SWIG
	/**
	@brief	全ての参照カウンタの値の合計を取得する。
	@return	参照カウンタ合計値
	@note
	実体はEngineに存在するのでそちらを参照する。
	*/
	int32_t GetGlobalReferenceCount();

	/**
	@brief	全ての参照カウンタの値を取得し、0ならDLLを解放する。
	@note
	実体はEngineに存在するのでそちらを参照する。
	*/
	void CheckDLL();

	template <typename T>
	struct ReferenceDeleter
	{
		void operator ()(T* p)
		{
			auto p_ = ((IReference*) p);
			SafeRelease(p_);
		}
	};

	template <typename T>
	struct ReferenceDeleterWithReleaseDLL
	{
		void operator ()(T* p)
		{
			auto p_ = ((IReference*) p);
			SafeRelease(p_);

			auto ref = GetGlobalReferenceCount();
			if (ref == 0)
			{
				CheckDLL();
			}
		}
	};

	template <typename T>
	static std::shared_ptr<T> CreateSharedPtr(T* p)
	{		
		return std::shared_ptr<T>(p, ReferenceDeleter<T>());
	}

	template <typename T>
	static std::shared_ptr<T> CreateSharedPtrWithReleaseDLL(T* p)
	{
		return std::shared_ptr<T>(p, ReferenceDeleterWithReleaseDLL<T>());
	}

#endif

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};

namespace ace{

	enum eMouseButtonState
	{
		MOUSE_PUSH,MOUSE_PULL,MOUSE_HOLD,MOUSE_FREE
	};

	class IMouseButtonState
	{
	public:
		virtual eMouseButtonState GetButtonState() const = 0;
	};
};


namespace ace{


	class IMouseWheelableButtonState:public IMouseButtonState
	{
	public:

		virtual eMouseButtonState GetButtonState() const = 0;

		virtual const double GetRotation() const = 0;
	};
};


namespace ace{

	class SideButton:public IMouseButtonState
	{
	private:
		eMouseButtonState m_eMouseButtonState;
	public:
		eMouseButtonState GetButtonState() const;
		SideButton(){}
		SideButton(eMouseButtonState eMouseButtonState,bool doubleClicked);
	};
};


namespace ace{

	class MiddleButton : public IMouseWheelableButtonState
	{
	private:
		eMouseButtonState m_eMouseButtonState;
		double m_rotation;
	public:
		eMouseButtonState GetButtonState() const;
		const double GetRotation() const;
		MiddleButton(eMouseButtonState eMouseButtonState,double rotation);
		MiddleButton(){}
	};
};

#include <memory>
namespace ace{
	class Mouse
	{
	public:
		virtual const SideButton* GetLeftButton() const = 0;


		virtual const SideButton* GetRightButton() const = 0;


		virtual const MiddleButton* GetMiddleButton() const = 0;


		virtual const Vector2DF GetPosition() const = 0;

	};
};

namespace ace{

	enum eKeys
	{
		ACE_KEY_UNKNOWN = 0x00,
		ACE_KEY_SPACE = 0x01,
		ACE_KEY_APOSTROPHE = 0x02,
		ACE_KEY_COMMA = 0x03,
		ACE_KEY_MINUS = 0x04,
		ACE_KEY_PERIOD = 0x05,
		ACE_KEY_SLASH = 0x06,
		ACE_KEY_0 = 0x07,
		ACE_KEY_1 = 0x08,
		ACE_KEY_2 = 0x09,
		ACE_KEY_3 = 0x0a,
		ACE_KEY_4 = 0x0b,
		ACE_KEY_5 = 0x0c,
		ACE_KEY_6 = 0x0d,
		ACE_KEY_7 = 0x0e,
		ACE_KEY_8 = 0x0f,
		ACE_KEY_9 = 0x10,
		ACE_KEY_SEMICOLON = 0x11,
		ACE_KEY_EQUAL = 0x12,
		ACE_KEY_A = 0x13,
		ACE_KEY_B = 0x14,
		ACE_KEY_C = 0x15,
		ACE_KEY_D = 0x16,
		ACE_KEY_E = 0x17,
		ACE_KEY_F = 0x18,
		ACE_KEY_G = 0x19,
		ACE_KEY_H = 0x1a,
		ACE_KEY_I = 0x1b,
		ACE_KEY_J = 0x1c,
		ACE_KEY_K = 0x1d,
		ACE_KEY_L = 0x1e,
		ACE_KEY_M = 0x1f,
		ACE_KEY_N = 0x20,
		ACE_KEY_O = 0x21,
		ACE_KEY_P = 0x22,
		ACE_KEY_Q = 0x23,
		ACE_KEY_R = 0x24,
		ACE_KEY_S = 0x25,
		ACE_KEY_T = 0x26,
		ACE_KEY_U = 0x27,
		ACE_KEY_V = 0x28,
		ACE_KEY_W = 0x29,
		ACE_KEY_X = 0x2a,
		ACE_KEY_Y = 0x2b,
		ACE_KEY_Z = 0x2c,
		ACE_KEY_LEFT_BRACKET = 0x2d,
		ACE_KEY_BACKSLASH = 0x2e,
		ACE_KEY_RIGHT_BRACKET = 0x2f,
		ACE_KEY_GRAVE_ACCENT = 0x30,
		ACE_KEY_WORLD_1 = 0x31,
		ACE_KEY_WORLD_2 = 0x32,
		ACE_KEY_ESCAPE = 0x33,
		ACE_KEY_ENTER = 0x34,
		ACE_KEY_TAB = 0x35,
		ACE_KEY_BACKSPACE = 0x36,
		ACE_KEY_INSERT = 0x37,
		ACE_KEY_DELETE = 0x38,
		ACE_KEY_RIGHT = 0x39,
		ACE_KEY_LEFT = 0x3a,
		ACE_KEY_DOWN = 0x3b,
		ACE_KEY_PAGE_UP = 0x3c,
		ACE_KEY_PAGE_DOWN = 0x3d,
		ACE_KEY_HOME = 0x3e,
		ACE_KEY_END = 0x3f,
		ACE_KEY_CAPS_LOCK = 0x40,
		ACE_KEY_SCROLL_LOCK = 0x41,
		ACE_KEY_NUM_LOCK = 0x42,
		ACE_KEY_PRINT_SCREEN = 0x43,
		ACE_KEY_PAUSE = 0x44,
		ACE_KEY_F1 = 0x45,
		ACE_KEY_F2 = 0x46,
		ACE_KEY_F3 = 0x47,
		ACE_KEY_F4 = 0x48,
		ACE_KEY_F5 = 0x49,
		ACE_KEY_F6 = 0x4a,
		ACE_KEY_F7 = 0x4b,
		ACE_KEY_F8 = 0x4c,
		ACE_KEY_F9 = 0x4d,
		ACE_KEY_F10 = 0x4e,
		ACE_KEY_F11 = 0x4f,
		ACE_KEY_F12 = 0x50,
		ACE_KEY_F13 = 0x51,
		ACE_KEY_F14 = 0x52,
		ACE_KEY_F15 = 0x53,
		ACE_KEY_F16 = 0x54,
		ACE_KEY_F17 = 0x55,
		ACE_KEY_F18 = 0x56,
		ACE_KEY_F19 = 0x57,
		ACE_KEY_F20 = 0x58,
		ACE_KEY_F21 = 0x59,
		ACE_KEY_F22 = 0x5a,
		ACE_KEY_F23 = 0x5b,
		ACE_KEY_F24 = 0x5c,
		ACE_KEY_F25 = 0x5d,
		ACE_KEY_KP_0 = 0x5e,
		ACE_KEY_KP_1 = 0x5f,
		ACE_KEY_KP_2 = 0x60,
		ACE_KEY_KP_3 = 0x61,
		ACE_KEY_KP_4 = 0x62,
		ACE_KEY_KP_5 = 0x63,
		ACE_KEY_KP_6 = 0x64,
		ACE_KEY_KP_7 = 0x65,
		ACE_KEY_KP_8 = 0x66,
		ACE_KEY_KP_9 = 0x67,
		ACE_KEY_KP_DECIMAL = 0x68,
		ACE_KEY_KP_DIVIDE = 0x69,
		ACE_KEY_KP_MULTIPLY = 0x6a,
		ACE_KEY_KP_SUBTRACT = 0x6b,
		ACE_KEY_KP_ADD = 0x6c,
		ACE_KEY_KP_ENTER = 0x6d,
		ACE_KEY_KP_EQUAL = 0x6e,
		ACE_KEY_LEFT_SHIFT = 0x6f,
		ACE_KEY_LEFT_CONTROL = 0x70,
		ACE_KEY_LEFT_ALT = 0x71,
		ACE_KEY_LEFT_SUPER = 0x72,
		ACE_KEY_RIGHT_SHIFT = 0x73,
		ACE_KEY_RIGHT_CONTROL = 0x74,
		ACE_KEY_RIGHT_ALT = 0x75,
		ACE_KEY_RIGHT_SUPER = 0x76,
		ACE_KEY_MENU = 0x77,
		ACE_KEY_LAST = 0x78,
		MAX = 0xfff
	};

	enum eKeyboardButtonState
	{
		KEYBOARD_PUSH, KEYBOARD_PULL, KEYBOARD_HOLD, KEYBOARD_FREE
	};

	class Keyboard
	{
	public:
		virtual const eKeyboardButtonState GetKeyState(eKeys key) = 0;
	};

};

namespace ace{

	enum eJoystickButtonState
	{
		ACE_JOYSTICK_BUTTON_PUSH,
		ACE_JOYSTICK_BUTTON_PULL,
		ACE_JOYSTICK_BUTTON_FREE,
		ACE_JOYSTICK_BUTTON_HOLD
	};

	enum eJoystickButtons
	{
		BUTTON_1
	};

	enum eJoystickAxises
	{
		AXIS_1
	};

	class Joystick
	{
	public:
		virtual const ace::achar* GetJoystickName() = 0;


		virtual const int GetButtonsCount() = 0;


		virtual const int GetAxesCount() = 0;


		virtual const eJoystickButtonState GetButtonState(int at) = 0;


		virtual const float GetAxisState(int at) = 0;
	};
};


namespace ace{
	class JoystickContainer
	{
	public:
		virtual const bool IsPresentAt(int at) = 0;


		virtual Joystick* GetJoystickAt(int at) = 0;
	};
};
#include <string>
#include <memory>

namespace ace
{
	class Log
	{
	public:
		enum eLogLevel
		{
			LOG_ERROR = 1 << 1,
			LOG_CRITICAL = 1 << 2,
			LOG_WARNING = 1 << 3,
			LOG_INFORMATION = 1 << 4,
			LOG_ALL = 2147483647	// INT_MAXと等しい
		};

		virtual ~Log()
		{
		}

		/**
			@brief	ログ ファイルへ文字列を書き込む。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void Write(const achar* text, int level = LOG_ALL) = 0;

		/**
			@brief	ログ ファイルへ文字列を書き込み、改行する。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLine(const achar* text, int level = LOG_ALL) = 0;

		/**
			@brief	ログ ファイルへ強調された文字列を書き込む。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteStrongly(const achar* text, int level = LOG_ALL) = 0;

		/**
			@brief	ログ ファイルへ強調された文字列を書き込み、改行する。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLineStrongly(const achar* text, int level = LOG_ALL) = 0;

		/**
			@brief	ログ ファイルへ見出しとして文字列を書き込む。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHeading(const achar* text, int level = LOG_ALL) = 0;

#if !_WIN32 && !SWIG
		/**
		@brief	ログ ファイルへ文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void Write(const wchar_t* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLine(const wchar_t* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteStrongly(const wchar_t* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLineStrongly(const wchar_t* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ見出しとして文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHeading(const wchar_t* text, int level = LOG_ALL) = 0;
#endif

#ifndef SWIG
	public:
		/**
		@brief	ログ ファイルへ文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void Write(const char* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLine(const char* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteStrongly(const char* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLineStrongly(const char* text, int level = LOG_ALL) = 0;

		/**
		@brief	ログ ファイルへ見出しとして文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHeading(const char* text, int level = LOG_ALL) = 0;
#endif

		/**
			@brief	ログ ファイルへ水平線を書き込む。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHorizontalRule(int level = LOG_ALL) = 0;

		/**
			@brief	ログ ファイルへ表形式(<table> 要素)での書き込みを開始する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void BeginTable(int level = LOG_ALL) = 0;

		/**
			@brief	ログ ファイルへの表形式(<table> 要素)での書き込みを終了する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void EndTable(int level = LOG_ALL) = 0;

		/**
			@brief	表形式での出力において、新しい行に移動する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void ChangeRow(int level = LOG_ALL) = 0;

		/**
			@brief	表形式での出力において、新しい列に移動する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void ChangeColumn(int level = LOG_ALL) = 0;

		/**
			@brief	ログの出力が可能であるかどうかを表す真偽値を取得する。
			@param	level	[in]	ログの出力が可能か?
		*/
		virtual bool IsValid() = 0;

		/**
			@brief	ログの出力レベルを設定する。ログ出力メソッドに指定した出力レベルがこの値より小さければログが出力される。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void SetOutputLevel(int level) = 0;
	};
}
#include <list>

namespace ace
{
	/**
		@brief	プログラムの実行状況(計算時間や、使っているプロセッサ)を記録するクラス。
	*/
	class Profiler : public ReferenceObject
	{
	protected:
		Profiler(){}
		virtual ~Profiler(){}

	public:
		/**
			@brief	計算の開始時間と、使用するプロセッサ番号を記録する。
			@param	id		プロファイリングを識別するID。Start関数は、IDの一致するEnd関数と対応する。
		*/
		virtual void Start(int id) = 0;

		/**
			@brief	計算の終了時間を記録し、プロファイリング結果を蓄積する。
			@param	id		プロファイリングを識別するID。End関数は、IDの一致するStart関数と対応する。
		*/
		virtual void End(int id) = 0;
	};
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Texture2D
		: public IReference
	{
	protected:
		eTextureClassType	m_type;
		
	public:
		Texture2D(){}
		virtual ~Texture2D(){}

		/**
			@brief	テクスチャのサイズを取得する。
			@return	サイズ
		*/
		virtual Vector2DI GetSize() const = 0;

		/**
			@brief	テクスチャのクラスの種類を取得する。
			@return	種類
		*/
		virtual eTextureClassType GetType() { return TEXTURE_CLASS_TEXTURE2D; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class RenderTexture2D
		: public Texture2D
	{
	public:
		RenderTexture2D(){}
		virtual ~RenderTexture2D(){}

		/**
		@brief	テクスチャのサイズを取得する。
		@return	サイズ
		*/
		virtual Vector2DI GetSize() const = 0;

		/**
		@brief	テクスチャのクラスの種類を取得する。
		@return	種類
		*/
		virtual eTextureClassType GetType() { return TEXTURE_CLASS_RENDERTEXTURE; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	シェーダー(2D)
	*/
	class Shader2D
		: public IReference
	{
	protected:
		Shader2D(){}
		virtual ~Shader2D(){}
	public:
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Material2D
		: public IReference
	{
	protected:
		virtual Texture2D* GetTexture2D__(const achar* name) = 0;
		virtual void SetTexture2D__(const achar* name, Texture2D* value) = 0;

		Material2D(){}
		virtual ~Material2D(){}
	public:
		virtual float GetFloat(const achar* name) = 0;
		virtual void SetFloat(const achar* name, float value) = 0;

		virtual Vector2DF GetVector2DF(const achar* name) = 0;
		virtual void SetVector2DF(const achar* name, Vector2DF value) = 0;

		virtual Vector3DF GetVector3DF(const achar* name) = 0;
		virtual void SetVector3DF(const achar* name, Vector3DF value) = 0;

#if! SWIG
		std::shared_ptr<Texture2D> GetTexture2D(const achar* name)
		{
			return CreateSharedPtrWithReleaseDLL(GetTexture2D__(name));
		}

		void SetTexture2D(const achar* name, std::shared_ptr<Texture2D> value)
		{
			SetTexture2D__(name, value.get());
		}

		void SetTexture2D(const achar* name, std::shared_ptr<RenderTexture2D> value)
		{
			SetTexture2D__(name, value.get());
		}
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class Graphics
	: public ReferenceObject
{
protected:
	virtual Texture2D* CreateTexture2D_(const achar* path) = 0;
	virtual RenderTexture2D* CreateRenderTexture2D_(int32_t width, int32_t height, eTextureFormat format) = 0;
	virtual Shader2D* CreateShader2D_( const achar* shaderText, ShaderVariableProperty* variableProperties, int32_t variablePropertiesCount) = 0;
	virtual Material2D* CreateMaterial2D_(Shader2D* shader) = 0;

public:
	Graphics(){}
	virtual ~Graphics(){}

#ifndef SWIG

	/**
	@brief	テクスチャを生成する。
	@param	path	パス
	@return	テクスチャ
	*/
	std::shared_ptr<Texture2D> CreateTexture2D(const achar* path) { return CreateSharedPtrWithReleaseDLL(CreateTexture2D_(path)); }

	/**
	@brief	描画先として指定可能なテクスチャを生成する。
	@param	width	横幅
	@param	height	縦幅
	@return	テクスチャ
	*/
	std::shared_ptr<RenderTexture2D> CreateRenderTexture(int32_t width, int32_t height, eTextureFormat format){ return CreateSharedPtrWithReleaseDLL(CreateRenderTexture2D_(width, height, format)); }

	/**
	@brief	シェーダー(2D)を生成する。
	@param	shaderText						シェーダーのコード
	@param	variableProperties				シェーダーで使用可能な外部入力可能な変数
	@return	シェーダー(2D)
	*/
	std::shared_ptr<Shader2D> CreateShader2D(
		const achar* shaderText,
		std::vector <ShaderVariableProperty>& variableProperties)
	{
		return CreateSharedPtrWithReleaseDLL(
			CreateShader2D_(
			shaderText,
			variableProperties.size() > 0 ? &(variableProperties[0]) : nullptr,
			variableProperties.size()));
	}

	/**
	@brief	マテリアル(2D)を生成する。
	@param	shader	シェーダー
	@return	マテリアル(2D)
	*/
	std::shared_ptr<Material2D> CreateMaterial2D(std::shared_ptr<Shader2D> shader)
	{
		auto material = CreateMaterial2D_(shader.get());
		return CreateSharedPtrWithReleaseDLL(material);
	}
#endif

	/**
	@brief	描画ランタイムの種類を取得する。
	@return	種類
	*/
	virtual eGraphicsType GetGraphicsType() const = 0;

};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Core
		: public ReferenceObject
	{
	private:

	public:

		/**
		@brief	初期化を行う。
		@param	title	タイトル
		@param	width	横幅
		@param	height	縦幅
		@param	isFullScreen	フルスクリーンで起動するかどうか?
		@return	成否
		*/
		virtual bool Initialize(const achar* title, int32_t width, int32_t height, bool isFullScreen) = 0;

		/**
		@brief	初期化を行う。
		@param	handle1	ハンドル
		@param	handle2	ハンドル
		@param	width	横幅
		@param	height	縦幅
		@return	成否
		*/
		virtual bool InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height) = 0;

		/**
		@brief	イベントを実行し、進行可否を判断する。
		@return	進行可能か?
		*/
		virtual bool DoEvents() = 0;

		/**
		@brief	更新処理を行う。
		*/
		virtual void Update() = 0;

		/**
		@brief	終了処理を行う。
		*/
		virtual void Terminate() = 0;

		/**
		@brief	更新されたリソースのリロードを行う。
		*/
		virtual void Reload() = 0;

		/**
			@brief	描画機能の準備をする。
		*/
		virtual void BeginDrawing() = 0;

		/**
			@brief	描画機能の使用を完了する。
		*/
		virtual void EndDrawing() = 0;

		/**
			@brief	現在のシーンを描画します。
		*/
		virtual void Draw() = 0;

		/**
			@brief	描画対象となるシーンを変更します。
		*/
		virtual void ChangeScene(CoreScene* scene) = 0;


		/**
		@brief キーボードの入力を扱うKeyboardクラスを取得する。
		*/
		virtual Keyboard* GetKeyboard() = 0;

		/**
		@brief マウスの入力を扱うMouseクラスを取得する。
		*/
		virtual Mouse* GetMouse() = 0;

		/**
		@brief 接続されているジョイスティックの入力を扱うJoystickContainerクラスを取得する。
		*/
		virtual JoystickContainer* GetJoystickContainer() = 0;

		/**
			@brief	ログ出力を扱うLogクラスを取得する。
		*/
		virtual Log* GetLogger() = 0;

		/**
			@brief	プロファイリング機能を提供するProfilerクラスを取得する。
		*/
		virtual Profiler* GetProfiler() = 0;

		/**
			@brief	描画を扱うグラフィックスクラスを取得する。
		*/
		virtual Graphics* GetGraphics() = 0;


		virtual ObjectSystemFactory* GetObjectSystemFactory() = 0;
	};
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};


namespace ace
{
	enum eChildMode
	{
		CHILD_MODE_POSITION = 0,
		CHILD_MODE_ALL = 1,
		CHILD_MODE_NOTHING = 2,
	};
}


#include <vector>
#include <memory>

namespace ace
{
	class Engine;

	/**
		@brief	ポストエフェクトを適用するクラス
	*/
	class PostEffect
	{
		friend class Engine;
		friend class Layer2D;

	private:
		std::shared_ptr<CorePostEffect>	m_corePostEffect;

		CorePostEffect* GetCoreObject() const;

	public:
		PostEffect();
		virtual ~PostEffect();

		/**
		@brief	オーバーライドして、毎フレーム描画される処理を記述できる。
		*/
		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) {}

		/**
		@brief	マテリアルを用いてテクスチャに画像を描画する。
		@param	target		描画先
		@param	material	マテリアル
		*/
		void DrawOnTexture2DWithMaterial(std::shared_ptr<RenderTexture2D> target, std::shared_ptr<Material2D> material);
	};
}


#include<memory>
#include <vector>


namespace ace
{

	/**
	@brief	グレースケール化するポストエフェクトクラス
	*/

	class PostEffectGrayScale : public PostEffect
	{
	private:

		std::shared_ptr<ace::Shader2D>		m_shader;
		std::shared_ptr<ace::Material2D>	m_material2d;
		PostEffectGrayScale() = default;
	public:
		PostEffectGrayScale(Graphics *g);

		virtual ~PostEffectGrayScale() = default;

		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;
	};

}

#include<memory>
#include <vector>


namespace ace
{

	/**
	@brief	セピア調化するポストエフェクトクラス
	*/

	class PostEffectSepia : public PostEffect
	{
	private:

		std::shared_ptr<ace::Shader2D>		m_shader;
		std::shared_ptr<ace::Material2D>	m_material2d;
		PostEffectSepia() = default;
	public:
		PostEffectSepia(Graphics *g);

		virtual ~PostEffectSepia() = default;

		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;
	};

}

namespace ace
{
	class Component
	{
		friend class Object2DBase;

	private:
		void Update();

	protected:
		virtual void OnUpdate() = 0;

	public:
		virtual ~Component()
		{
		}
	};
}
#include <memory>
#include <list>

namespace ace
{
	class Layer2D;

	class Object2DBase
	{
		friend class Layer2D;

	public:
		typedef std::shared_ptr<Object2DBase> Object2DBasePtr;
		typedef std::shared_ptr<Component> ComponentPtr;

	private:
		Layer2D* m_owner;
		std::list<Object2DBasePtr> m_children;
		std::map<astring, ComponentPtr> m_components;

		void Start();
		void Update();
		void SetLayer(Layer2D* layer);
		virtual ICoreObject2D* GetCoreObject() const = 0;

	protected:
		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDrawAdditionally() = 0;

	public:
		Object2DBase();
		virtual ~Object2DBase();

		/**
			@brief	このオブジェクトを保持しているレイヤーを取得する。
		*/
		Layer2D* GetLayer() const;

		Vector2DF GetPosition() const;
		void SetPosition(Vector2DF position);
		Vector2DF GetGlobalPosition();

		float GetAngle() const;
		void SetAngle(float value);

		Vector2DF GetScale() const;
		void SetScale(Vector2DF value);

		void AddChild(const Object2DBasePtr& child, eChildMode mode);
		void RemoveChild(const Object2DBasePtr& child);
		const std::list<Object2DBasePtr>& GetChildren() const;

		void AddComponent(const ComponentPtr& component, astring key);
		ComponentPtr& GetComponent(astring key);
		void RemoveComponent(astring key);
	};
}
#include <memory>

namespace ace
{
	class Object2D : Object2DBase
	{
	private:
		std::shared_ptr<CoreObject2D> m_coreObject;

		ICoreObject2D* GetCoreObject() const;

	protected:
		/**
		@brief	オーバーライドして、このオブジェクトの初期化処理を記述できる。
		*/
		virtual void OnStart();

		/**
		@brief	オーバーライドして、毎フレーム実行される更新処理を記述できる。
		*/
		virtual void OnUpdate();

		/**
		@brief	オーバーライドして、追加の描画処理を記述できる。
		*/
		virtual void OnDrawingAdditionally();

	public:
		Object2D();
		virtual ~Object2D();
	};
}
#include <memory>

namespace ace
{
	class TextureObject2D : public Object2DBase
	{
	private:
		std::shared_ptr<CoreTextureObject2D> m_coreObject;
		std::shared_ptr<Texture2D> m_texturePtr;

		ICoreObject2D* GetCoreObject() const;

	protected:
		/**
		@brief	オーバーライドして、このオブジェクトの初期化処理を記述できる。
		*/
		virtual void OnStart();

		/**
		@brief	オーバーライドして、毎フレーム実行される更新処理を記述できる。
		*/
		virtual void OnUpdate();

		/**
		@brief	オーバーライドして、追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally();

	public:
		TextureObject2D();
		virtual ~TextureObject2D();


		std::shared_ptr<Texture2D> GetTexture() const;
		void SetTexture(std::shared_ptr<Texture2D> texture);

		Vector2DF GetCenterPosition() const;
		void SetCenterPosition(Vector2DF position);

		Color GetColor() const;
		void SetColor(Color color);

		bool GetTurnLR() const;
		void SetTurnLR(bool turnLR);

		bool GetTurnUL() const;
		void SetTurnUL(bool turnUL);

		int GetDrawingPriority() const;
		void SetDrawingPriority(int priority);

		eAlphaBlend GetAlphaBlendMode() const;
		void SetAlphaBlendMode(eAlphaBlend alphaBlend);
	};
}
#include <memory>

namespace ace
{
	class Scene;

	class Layer
	{
		friend class Scene;

	private:
		virtual std::shared_ptr<CoreLayer> GetCoreLayer() const = 0;
		virtual void SetScene(Scene* scene) = 0;

		virtual void Update() = 0;
		virtual void DrawAdditionally() = 0;

		virtual void BeginDrawing() = 0;
		virtual void EndDrawing() = 0;

	public:
		virtual ~Layer()
		{
		}

		virtual int GetDrawingPriority() const = 0;
		virtual void SetDrawingPriority(int value) = 0;

		virtual Scene* GetScene() const = 0;
	};
}
#include <memory>
#include <list>

namespace ace
{
	class Layer2D : public Layer
	{
		friend class Scene;

	private:
		typedef Object2DBase Object;

		std::shared_ptr<CoreLayer2D>				m_coreLayer;
		std::list<std::shared_ptr<Object>>			m_objects;
		std::vector<std::shared_ptr<PostEffect>>	m_postEffects;
		Scene* m_scene;
		
		std::shared_ptr<CoreLayer> GetCoreLayer() const;
		void SetScene(Scene* scene);
		void Update();
		void DrawAdditionally();

		void BeginDrawing();
		void EndDrawing();

	protected:
		virtual void OnUpdating();
		virtual void OnUpdated();
		virtual void OnDrawAdditionally();

	public:
		Layer2D();
		virtual ~Layer2D();

		void AddObject(const std::shared_ptr<Object>& object);
		void RemoveObject(const std::shared_ptr<Object>& object);

		/**
			@brief	ポストエフェクトを追加する。
			@param	postEffect	ポストエフェクト
		*/
		void AddPostEffect(const std::shared_ptr<PostEffect>& postEffect);

		/**
			@brief	ポストエフェクトを全て消去する。
		*/
		void ClearPostEffects();

		int GetDrawingPriority() const;
		void SetDrawingPriority(int value);

		Scene* GetScene() const;
	};
}
#include <vector>
#include <memory>

namespace ace
{
	class Engine;

	class Scene
	{
		friend class Engine;

	private:
		std::shared_ptr<CoreScene> m_coreScene;
		std::list<std::shared_ptr<Layer>> m_layers;

		void Update();
		void DrawAdditionally();
		void BeginDrawing();
		void EndDrawing();

	protected:
		virtual void OnUpdating();
		virtual void OnUpdated();

	public:
		Scene();
		virtual ~Scene();

		void AddLayer(const std::shared_ptr<Layer>& layer);
		void RemoveLayer(const std::shared_ptr<Layer>& layer);
	};
}



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Engine
	{
		friend Engine* GetEngine();

	private:
		Core*	m_core;
		std::shared_ptr<Scene>	m_currentScene;
		
		Engine(Core* core);
		~Engine();

	public:
		/**
			@brief	初期化を行う。
			@param	title	タイトル
			@param	width	横幅
			@param	height	縦幅
			@param	isFullScreen	フルスクリーンで起動するかどうか?
			@return	成否
		*/
		bool Initialize(const achar* title, int32_t width, int32_t height, bool isFullScreen);

		/**
			@brief	初期化を行う。
			@param	handle1	ハンドル
			@param	handle2	ハンドル
			@param	width	横幅
			@param	height	縦幅
			@return	成否
		*/
		bool InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height);

		/**
			@brief	イベントを実行し、進行可否を判断する。
			@return	進行可能か?
		*/
		bool DoEvents();

		/**
			@brief	更新処理を行う。
		*/
		void Update();

		/**
			@brief	終了処理を行う。
		*/
		void Terminate();

		/**
			@brief	描画する対象となるシーンを変更する。
		*/
		void ChangeScene(std::shared_ptr<Scene>& scene);

#if _WIN32

#else
		/**
			@brief	初期化を行う。
			@param	title	タイトル
			@param	width	横幅
			@param	height	縦幅
			@param	isFullScreen	フルスクリーンで起動するかどうか?
			@return	成否
		*/
		bool Initialize(const wchar_t* title, int32_t width, int32_t height, bool isFullScreen)
		{
			return Initialize( ToAString(title).c_str(), width, height, isFullScreen );
		}
#endif
	};

	/**
		@brief	エンジンを取得する。
		@return	エンジン
	*/
	Engine* GetEngine();

	/**
		@brief キーボードクラスを取得する。
		@return キーボード
	*/
	Keyboard* GetKeyboard();

	/**
		@brief マウスクラスを取得する。
		@return マウス
	*/
	Mouse* GetMouse();

	/**
		@brief	ログクラスを取得する。
		@return	ログクラス
	*/
	Log* GetLogger();

	/**
	@brief	プロファイラクラスを取得する。
	@return	プロファイラクラス
	*/
	Profiler* GetProfiler();

	/**
	@brief	ジョイスティックコンテナクラスを取得する。
	@return	ジョイスティックコンテナクラス
	*/
	JoystickContainer* GetJoystickContainer();

	/**
		@brief	Graphicsクラスを取得する。
	*/
	Graphics* GetGraphics();

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

