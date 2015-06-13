#pragma once
#include<cassert>
#include<cstring>
namespace asd
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