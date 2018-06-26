
#pragma once

#include <asd.common.Base.h>
#include "../asd.Core.Base_Imp.h"

#include "../IO/asd.File.h"
#include "../IO/asd.StaticFile.h"

#include <time.h>
#include <functional>

#if defined(_WIN32)

#elif ( defined(_CONSOLE_GAME) )

#else
#include <sys/stat.h>
#endif

namespace asd
{
	template <class RESOURCE>
	class ResourceContainer
	{
	public:
		class LoadingInformation
		{
		private:
		public:
			time_t ModifiedTime;
			astring LoadedPath;
			astring CachedPath;
			RESOURCE* ResourcePtr;
		};

	private:
		std::map<astring, RESOURCE*>	keyToResource;
		std::map<RESOURCE*, astring>	resourceToKeys;

		std::map <RESOURCE*, std::shared_ptr <LoadingInformation> >	loadInfo;

		File*	file;
	public:

		ResourceContainer(File* file)
			: file(file)
		{

		}

		const std::map<astring, RESOURCE*>& GetAllResources()
		{
			return keyToResource;
		}

		RESOURCE* Get(const achar* key)
		{
			if (keyToResource.count(key) > 0)
			{
				return keyToResource[key];
			}

			return nullptr;
		}

		RESOURCE* TryLoad(const achar* path, const std::function<RESOURCE*(uint8_t*,int32_t)>& loadFunc)
		{
			auto start = clock();

			{
				auto existing = Get(path);
				if (existing != nullptr)
				{
					SafeAddRef(existing);
					return existing;
				}
			}

			auto staticFileStart = clock();
			auto staticFile = file->CreateStaticFile(path);
			auto staticFileEnd = clock();
			printf("ResourceContainer.TryLoad-CreateStaticFile: %d\n", staticFileEnd - staticFileStart);
			if (staticFile == nullptr) return nullptr;

			auto loadStart = clock();
			auto ret = loadFunc((uint8_t*)staticFile->GetData(), staticFile->GetSize());
			auto loadEnd = clock();
			printf("ResourceContainer.TryLoad-loadFunc: %d\n", loadEnd - loadStart);

			if (ret == nullptr)
			{
				SafeRelease(staticFile);
				return nullptr;
			}

			auto info = std::make_shared<LoadingInformation>();

			if (!staticFile->GetIsInPackage())
			{
				auto path_ = staticFile->GetFullPath();
				info->ModifiedTime = GetModifiedTime(path_);
				info->LoadedPath = path_;
			}
			SafeRelease(staticFile);

			info->ResourcePtr = ret;

			Register(path, ret, info);

			auto end = clock();
			printf("ResourceContainer.TryLoad: %d\n", end - start);
			return ret;
		}

		RESOURCE* TryLoadWithVector(const achar* path, const std::function<RESOURCE*(const std::vector<uint8_t>&)>& loadFunc)
		{
			{
				auto existing = Get(path);
				if (existing != nullptr)
				{
					SafeAddRef(existing);
					return existing;
				}
			}

			auto staticFile = file->CreateStaticFile(path);
			if (staticFile == nullptr) return nullptr;

			auto ret = loadFunc(staticFile->GetBuffer());

			if (ret == nullptr)
			{
				SafeRelease(staticFile);
				return nullptr;
			}

			auto info = std::make_shared<LoadingInformation>();

			if (!staticFile->GetIsInPackage())
			{
				auto path_ = staticFile->GetFullPath();
				info->ModifiedTime = GetModifiedTime(path_);
				info->LoadedPath = path_;
			}

			info->ResourcePtr = ret;
			Register(path, ret, info);

			SafeRelease(staticFile);
			return ret;
		}

		void Register(const achar* key, RESOURCE* target, std::shared_ptr<LoadingInformation> info = nullptr)
		{
			keyToResource[key] = target;
			resourceToKeys[target] = key;

			if (info != nullptr)
			{
				loadInfo[target] = info;
			}
		}

		void Unregister(RESOURCE* target)
		{
			keyToResource.erase(resourceToKeys[target]);
			resourceToKeys.erase(target);
			loadInfo.erase(target);
		}

		void Reload(const std::function<void(std::shared_ptr <LoadingInformation>, uint8_t*, int32_t)>& reloadFunc)
		{
			for (auto info : loadInfo)
			{
				if (info.second == nullptr) continue;

				auto info_ = info.second;

				auto time = GetModifiedTime(info_->LoadedPath.c_str());

				if (info_->ModifiedTime == time) continue;
				info_->ModifiedTime = time;
				
				auto path = info_->LoadedPath.c_str();
#if _WIN32
				auto fp = _wfopen((const wchar_t*)path, L"rb");
				if (fp == nullptr) return;
#else
				auto fp = fopen(ToUtf8String(path).c_str(), "rb");
				if (fp == nullptr) return;
#endif
				fseek(fp, 0, SEEK_END);
				auto size = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				auto data = new uint8_t[size];
				fread(data, 1, size, fp);
				fclose(fp);

				reloadFunc(info_, data, size);

				SafeDeleteArray(data);
			}
		}

		void ReloadWithVector(const std::function<void(std::shared_ptr <LoadingInformation>, const std::vector<uint8_t>&)>& reloadFunc)
		{
			for (auto info : loadInfo)
			{
				if (info.second == nullptr) continue;

				auto info_ = info.second;

				auto time = GetModifiedTime(info_->LoadedPath.c_str());

				if (info_->ModifiedTime == time) continue;
				info_->ModifiedTime = time;

				auto path = info_->LoadedPath.c_str();
#if _WIN32
				auto fp = _wfopen((const wchar_t*)path, L"rb");
				if (fp == nullptr) return;
#else
				auto fp = fopen(ToUtf8String(path).c_str(), "rb");
				if (fp == nullptr) return;
#endif
				fseek(fp, 0, SEEK_END);
				auto size = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				std::vector<uint8_t> data;
				data.resize(size);
				fread(data.data(), 1, size, fp);
				fclose(fp);

				reloadFunc(info_, data);
			}
		}

		/**
		@brief	ファイル更新時間を取得する。
		@param	path	パス
		*/
		time_t GetModifiedTime(const achar* path)
		{
#if _WIN32
			HANDLE file = nullptr;
			FILETIME time;
			//SYSTEMTIME st;

			file = CreateFileW(
				(const wchar_t*)path,
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

#elif ( defined(_CONSOLE_GAME) )
			return 0;
#else
			auto p = ToUtf8String(path);
			struct stat stat_buf;
			if (stat(p.c_str(), &stat_buf) == 0)
			{
				return stat_buf.st_mtime;
			}

			return 0;
#endif
		}
	};
}