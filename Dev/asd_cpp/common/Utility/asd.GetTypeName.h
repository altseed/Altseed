#pragma once
#include<string>
#if defined(_MSC_VER)

#elif defined(__clang__)
#include<cxxabi.h>
#include<cstdlib>
#include<cassert>
#include<typeinfo>
#if !__has_include(<cxxabi.h>)
#error "cxxabi.h is not found"
#endif
#elif defined(__GNUC__)
#include<cxxabi.h>
#include<cstdlib>
#include<cassert>
#include<typeinfo>
#endif

namespace asd
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