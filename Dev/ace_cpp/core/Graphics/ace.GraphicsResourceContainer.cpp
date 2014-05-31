
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.GraphicsResourceContainer.h"
#include "Resource/ace.Texture2D_Imp.h"
#include "3D/ace.Model_Imp.h"
#include "Resource/ace.Font_Imp.h"

#if !_WIN32
#include <sys/stat.h>
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	GraphicsResourceContainer::GraphicsResourceContainer()
	{
	
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	GraphicsResourceContainer::~GraphicsResourceContainer()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	time_t GraphicsResourceContainer::GetModifiedTime(const achar* path)
	{
#if _WIN32
		HANDLE file = nullptr;
		FILETIME time;
		SYSTEMTIME st;

		file = CreateFileW(path,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			0);

		if (file == INVALID_HANDLE_VALUE)
		{
			CloseHandle(file);
			return 0;
		}
		
		GetFileTime(
			file,
			nullptr,
			nullptr,
			&time);

		CloseHandle(file);

		LONGLONG ll;
		ll = ((LONGLONG) time.dwHighDateTime << 32) + time.dwLowDateTime;
		return (time_t) ((ll - 116444736000000000) / 10000000);
#else
		auto p = ToUtf8String(path);
		struct stat stat_buf;
		if(stat(p.c_str(), &stat_buf) == 0 )
		{
			return stat_buf.st_mtime;
		}

		return 0;
#endif
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void GraphicsResourceContainer::Reload()
	{
		for (auto info : Texture2Ds.m_reloadInfo)
		{
			if (info.second == nullptr) continue;

			auto info_ = info.second.get();

			auto time = GetModifiedTime(info_->Path.c_str());

			if (info_->ModifiedTime == time) continue;
			info_->ModifiedTime = time;

			// リロード処理
#if _WIN32
			auto fp = _wfopen(info_->Path.c_str(), L"rb");
			if (fp == nullptr) continue;
#else
			auto fp = fopen(ToUtf8String(info_->Path.c_str()).c_str(), "rb");
			if (fp == nullptr) continue;
#endif

			fseek(fp, 0, SEEK_END);
			auto size = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			auto data = new uint8_t[size];
			fread(data, 1, size, fp);
			fclose(fp);
			
			info.first->Reload(data, size);
			SafeDeleteArray(data);
		}

		for (auto info : Models.m_reloadInfo)
		{
			if (info.second == nullptr) continue;

			auto info_ = info.second.get();

			auto time = GetModifiedTime(info_->Path.c_str());

			if (info_->ModifiedTime == time) continue;
			info_->ModifiedTime = time;

			// リロード処理
#if _WIN32
			auto fp = _wfopen(info_->Path.c_str(), L"rb");
			if (fp == nullptr) continue;
#else
			auto fp = fopen(ToUtf8String(info_->Path.c_str()).c_str(), "rb");
			if (fp == nullptr) continue;
#endif

			std::vector<uint8_t> data;
			fseek(fp, 0, SEEK_END);
			auto size = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			data.resize(size);
			fread(&data[0], 1, size, fp);
			fclose(fp);

			info.first->Reload(data, info_->Path.c_str());
		}

		for (auto info : Fonts.m_reloadInfo)
		{
			if (info.second == nullptr) continue;

			auto info_ = info.second.get();

			auto time = GetModifiedTime(info_->Path.c_str());

			if (info_->ModifiedTime == time) continue;
			info_->ModifiedTime = time;

			// リロード処理
			info.first->Reload(info_->Path.c_str());
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
