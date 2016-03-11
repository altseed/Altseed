﻿#pragma once
#include<cstddef>
#include<cassert>
namespace asd
{

/**
	@brief	確保されたメモリ領域の先頭ポインタと長さから、デバッグ時境界検査つき配列を提供する
	@details	確保されたメモリの所有権は移動しないので、使用する側でメモリを解放する必要がある
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