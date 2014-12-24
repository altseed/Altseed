
#pragma once

#if defined(_WIN32)
#include <Windows.h>
#endif


#include <cstdint>
#include <cstring>

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

namespace ace
{


std::string GetCPUName()
{
	/**
	@brief	CPU名を取得する
	*/

#if defined(__i386__) || defined(_M_IX86)
	char procName[64] = "";
	
	std::uint32_t inst = 0x80000002;
	
	union
	{
		std::uint32_t i[4];
		std::uint8_t ch[16];
	} tmp;
	
	for(std::uint32_t i = 0; i < 3; i++)
	{
#if defined(_MSC_VER)
		_asm
		{
			mov EAX, inst
			cpuid
			mov tmp + 0, EAX
			mov tmp + 4, EBX
			mov tmp + 8, ECX
			mov tmp + 12, EDX
		}
#elif defined(__GNUC__) || defined(__clang__)
		__asm__(
			"cpuid"
			: "=a" (tmp.i[0]), "=b" (tmp.i[1]), "=c" (tmp.i[2]), "=d" (tmp.i[3])
			: "a" (inst)
			: "%eax", "%ebx", "%ecx", "%edx"
		);
#endif
		std::memcpy(procName + i * 16, &tmp, 16);
		inst++;
	}
	return std::string(std::find_if(procName, procName + 64, [](std::uint8_t const ch){return ch != ' ';}), procName + 64);
#else
	return std::string("Not implemented");
#endif
}


std::string GetMemoryInfo()
{
	/**
	@brief	メモリ情報を取得する
	*/

#if defined(_WIN32)
	std::stringstream ss;
	MEMORYSTATUSEX stat;
	ZeroMemory(&stat, sizeof(MEMORYSTATUSEX));
	stat.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&stat);
	const int MB = 1024 * 1024;
	ss << 
		"Using: " << stat.dwMemoryLoad << "%\n"
		"Total physical memory: " << stat.ullTotalPhys  / MB << "MB\n"
		"Free physical memory: " << stat.ullAvailPhys  / MB << "MB\n"
		"Total virtual memory: " << stat.ullTotalVirtual  / MB << "MB\n"
		"Free virtual memory: " << stat.ullAvailVirtual  / MB << "MB" <<
	std::endl;
	
	return ss.str();
#else
	return std::string("Not implemented");
#endif
}

std::string GetWindowsVersion()
{
	/**
	@brief	Windowsのバージョン情報を取得する
	@detail	バージョン情報等については
	http://msdn.microsoft.com/ja-jp/library/windows/desktop/ms724833(v=vs.85).aspx
	を参照
	*/

#if defined(_WIN32)
	std::stringstream ss;
	char const* const pt[] = {"Unkown", "Workstation", "Domain Controller", "Server"};
	OSVERSIONINFOEX info;
	ZeroMemory(&info, sizeof(OSVERSIONINFOEX));
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	GetVersionEx((LPOSVERSIONINFO)&info);
	
	ss << 
		"Version: " << info.dwMajorVersion << "." << info.dwMinorVersion << " " << info.szCSDVersion << "\n"
		"Build: " << info.dwBuildNumber << "\n"
		"ProductType: " << ((info.wProductType <= 0 || info.wProductType > 3)? pt[0]: pt[info.wProductType]) << "\n"
		"SuiteMask: 0x" << std::hex << info.wSuiteMask << std::dec <<
	std::endl;
	
	return ss.str();
#else
	return std::string("Not implemented");
#endif

}



}