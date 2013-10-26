#pragma once

#include<cstring>
#include<cassert>

namespace
{
namespace detail
{
	template <class T> struct remove_pointer;

	template <class T> struct remove_pointer<T*>
	{
		typedef T type;
	};
}
}

namespace ace
{
	
	/**
	@brief	配列をコピーする。memcpyと同じインタフェースを持つため、基本的に置き換えできる。
	@details	基本的には二つの配列が同じ型である必要があるが、同じサイズ・アラインメントを持つ型ならばコピーを許容する。
				ポインタではなく配列として渡された場合には長さを取得し、バッファオーバーラン検査を行う。
				バッファオーバランの危険があるときにはデバッグ実行時に異常終了する
	@warning	渡された値がポインタの場合、配列の長さが取得できないのでバッファオーバーラン検査ができない。
	@param	to		[out]	コピー先のポインタ
	@param	from	[in]	コピー元のポインタ
	@param	num		[in]	コピーする **バイト数**
	*/


	//---------------------------------------------------------------------------------------------------------
	//
	//---------------------------------------------------------------------------------------------------------
	template<class T, class U, std::size_t toN, std::size_t fromN> void* CopyArray(T(&to)[toN], const U(&from)[fromN], std::size_t const byteNum)
	{
#if (__cplusplus >= 201103L) 
		static_assert(alignof(T) == alignof(U), "arguments 'from' and 'to' must be the same alignment.");
		static_assert(sizeof(T) == sizeof(U), "arguments 'from' and 'to' must be the same size.");

#elif (_MSC_VER >= 1800)
		static_assert(__alignof(T) == __alignof(U), "arguments 'from' and 'to' must be the same alignment.");
		static_assert(sizeof(T) == sizeof(U), "arguments 'from' and 'to' must be the same size.");

#else
		assert(sizeof(T) == sizeof(U));

#endif 
		assert(((sizeof(T) * toN >= byteNum) && sizeof(U) * fromN >= byteNum) || !"The length of array is too short to copy. check the arg!");
		return std::memcpy(static_cast<void*>(to), static_cast<const void*>(from), byteNum);
	}

	//---------------------------------------------------------------------------------------------------------
	//
	//---------------------------------------------------------------------------------------------------------

	template<class T, class U, std::size_t N> void* CopyArray(T(&to)[N], const U from, std::size_t const byteNum)
	{
		typename detail::remove_pointer<U>::type Up;
#if (__cplusplus >= 201103L) 
		static_assert(alignof(T) == alignof(Up), "arguments 'from' and 'to' must be the same alignment.");
		static_assert(sizeof(T) == sizeof(Up), "arguments 'from' and 'to' must be the same size.");

#elif (_MSC_VER >= 1800)
		static_assert(__alignof(T) == __alignof(Up), "arguments 'from' and 'to' must be the same alignment.");
		static_assert(sizeof(T) == sizeof(Up), "arguments 'from' and 'to' must be the same size.");

#else
		assert(sizeof(T) == sizeof(Up));
		
#endif
		assert(sizeof(T) * N >= byteNum || !"The length of array is too short to copy. check the arg!");
		return std::memcpy(static_cast<void*>(to), static_cast<const void*>(from), byteNum);
	}


	//---------------------------------------------------------------------------------------------------------
	//
	//---------------------------------------------------------------------------------------------------------

	template<class T, class U, std::size_t N> void* CopyArray(T to, const U(&from)[N], std::size_t const byteNum)
	{
		typename detail::remove_pointer<T>::type Tp;
#if (__cplusplus >= 201103L) 
		static_assert(alignof(Tp) == alignof(U), "arguments 'from' and 'to' must be the same alignment.");
		static_assert(sizeof(Tp) == sizeof(U), "arguments 'from' and 'to' must be the same size.");

#elif (_MSC_VER >= 1800)
		static_assert(__alignof(Tp) == __alignof(U), "arguments 'from' and 'to' must be the same alignment.");
		static_assert(sizeof(Tp) == sizeof(U), "arguments 'from' and 'to' must be the same size.");
#else
		assert(sizeof(Tp) == sizeof(U));
		
#endif
		assert(sizeof(U) * N >= byteNum || !"The length of array is too short to copy. check the arg!");
		return std::memcpy(static_cast<void*>(to), static_cast<const void*>(from), byteNum);
	}

	//---------------------------------------------------------------------------------------------------------
	//
	//---------------------------------------------------------------------------------------------------------

	template<class T, class U> void* CopyArray(T to, const U from, std::size_t const byteNum)
	{
		typename detail::remove_pointer<T>::type Tp;
		typename detail::remove_pointer<U>::type Up;
#if (__cplusplus >= 201103L) 
		static_assert(alignof(Tp) == alignof(Up), "arguments 'from' and 'to' must be the same alignment.");
		static_assert(sizeof(Tp) == sizeof(Up), "arguments 'from' and 'to' must be the same size.");

#elif (_MSC_VER >= 1800)
		static_assert(__alignof(Tp) == __alignof(Up), "arguments 'from' and 'to' must be the same alignment.");
		static_assert(sizeof(Tp) == sizeof(Up), "arguments 'from' and 'to' must be the same size.");

#else
		assert(sizeof(Tp) == sizeof(Up));

#endif
		return std::memcpy(static_cast<void*>(to), static_cast<const void*>(from), byteNum);
	}
}