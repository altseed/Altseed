
#pragma once

#include <ace.common.Base.h>
#include "../Core/ace.Core.Base_Imp.h"

#include "../IO/ace.File.h"
#include "../IO/ace.StaticFile.h"

#include <time.h>
#include <functional>

#if !_WIN32
#include <sys/stat.h>
#endif

namespace ace
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
			{
				auto existing = Get(path);
				if (existing != nullptr)
				{
					SafeAddRef(existing);
					return existing;
				}
			}

			auto staticFile = file->CreateStaticFile(path);
			if (staticFile.get() == nullptr) return nullptr;

			auto ret = loadFunc((uint8_t*)staticFile->GetData(), staticFile->GetSize());

			if (ret == nullptr)
			{
				return nullptr;
			}

			auto info = std::make_shared<LoadingInformation>(); 
			info->ModifiedTime = GetModifiedTime(path);
			info->LoadedPath = path;
			info->ResourcePtr = ret;
			Register(path, ret, info);

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
			if (staticFile.get() == nullptr) return nullptr;

			auto ret = loadFunc(staticFile->ReadAllBytes());

			if (ret == nullptr)
			{
				return nullptr;
			}

			auto info = std::make_shared<LoadingInformation>();
			info->ModifiedTime = GetModifiedTime(path);
			info->LoadedPath = path;
			info->ResourcePtr = ret;
			Register(path, ret, info);

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
				auto fp = _wfopen(path, L"rb");
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
				auto fp = _wfopen(path, L"rb");
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
			if (stat(p.c_str(), &stat_buf) == 0)
			{
				return stat_buf.st_mtime;
			}

			return 0;
#endif
		}
	};
}