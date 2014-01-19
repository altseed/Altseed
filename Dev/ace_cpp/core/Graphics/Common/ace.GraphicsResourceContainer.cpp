
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.GraphicsResourceContainer.h"
#include "Resource/ace.Texture2D_Imp.h"

#if !_WIN32
#include <sys/stat.h>
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

	GraphicsResourceFile::GraphicsResourceFile(
		GraphicsResourceContainer* container, 
		const achar* path, 
		GraphicsResourceFileObserver* observer)
	{
		m_path = path;
		m_modifiedTime = GraphicsResourceContainer::GetModifiedTime(path);
		m_container = container;

		m_observers.insert(observer);

		m_container->AddResourceFile(this);
	}

	GraphicsResourceFile::~GraphicsResourceFile()
	{
	}

	void GraphicsResourceFile::Reload(bool fourced)
	{
		auto time = GraphicsResourceContainer::GetModifiedTime(m_path.c_str());

		if (m_modifiedTime != time)
		{
			for (auto& o : m_observers)
			{
				o->Update(this);
			}

			m_modifiedTime = time;
		}
	}

	void GraphicsResourceFile::Attach(GraphicsResourceFileObserver* observer)
	{
		if (m_observers.count(observer) == 0)
		{
			m_observers.insert(observer);
		}
	}

	void GraphicsResourceFile::Detach(GraphicsResourceFileObserver* observer)
	{
		if (m_observers.count(observer) != 0)
		{
			m_observers.erase(observer);
		}

		if (m_observers.size() == 0)
		{
			m_container->RemoveResourceFile(this);
			delete this;
		}
	}

	GraphicsTexture2DFileObserver::GraphicsTexture2DFileObserver(Texture2D_Imp* texture)
	{
		m_texture = texture;
	}

	GraphicsTexture2DFileObserver::~GraphicsTexture2DFileObserver()
	{

	}

	void GraphicsTexture2DFileObserver::Update(GraphicsResourceFile* file)
	{
#if _WIN32
		auto fp = _wfopen(file->GetPath(), L"rb");
		if (fp == nullptr) return;
#else
		auto fp = fopen(ToUtf8String(file->GetPath()).c_str(), "rb");
		if (fp == nullptr) return;
#endif

		fseek(fp, 0, SEEK_END);
		auto size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		auto data = new uint8_t[size];
		fread(data, 1, size, fp);
		fclose(fp);

		m_texture->Reload(data, size);
		SafeDeleteArray(data);
	}

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
	Texture2D_Imp* GraphicsResourceContainer::GetTexture2D(const achar* key)
	{
		if (m_keyToTexture2Ds.count(key) > 0)
		{
			return m_keyToTexture2Ds[key];
		}

		return nullptr;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void GraphicsResourceContainer::RegistTexture2D(const achar* key, std::shared_ptr <Texture2DReloadInformation> info, Texture2D_Imp* texture)
	{
		m_keyToTexture2Ds[key] = texture;
		m_texture2DToKeys[texture] = key;
		m_texture2DReloadInfo[texture] = info;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void GraphicsResourceContainer::UnregistTexture2D(Texture2D_Imp* texture)
	{
		m_keyToTexture2Ds.erase(m_texture2DToKeys[texture]);
		m_texture2DToKeys.erase(texture);
		m_texture2DReloadInfo.erase(texture);
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
		for (auto info : m_texture2DReloadInfo)
		{
			if (info.second == nullptr) continue;

			auto info_ = info.second.get();

			auto time = GetModifiedTime(info_->Path.c_str());

			if (info_->ModifiedTime == time) continue;

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
	}

	void GraphicsResourceContainer::AddResourceFile(GraphicsResourceFile* file)
	{
		m_resourceFiles.insert(file);
	}

	void GraphicsResourceContainer::RemoveResourceFile(GraphicsResourceFile* file)
	{
		m_resourceFiles.erase(file);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
