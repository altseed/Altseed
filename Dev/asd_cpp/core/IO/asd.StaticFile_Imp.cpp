
#include "asd.File_Imp.h"
#include "asd.StaticFile_Imp.h"
#include <time.h>
#include <iostream>

namespace asd
{
	StaticFile_Imp::StaticFile_Imp(File_Imp* file,
		const astring& cacheKey,
		std::shared_ptr<BaseFile>& baseFile)
		: file(file)
		, cacheKey(cacheKey)
	{
		baseFile->ReadAllBytes(m_buffer);
		loadState = LoadState::Loaded;

		m_path = baseFile->GetFullPath();

		SafeAddRef(file);

		isInPackage = false;
	}

	StaticFile_Imp::StaticFile_Imp(File_Imp* file, const astring& cacheKey, const std::shared_ptr<BaseFile>& packedFile, PackFileInternalHeader& internalHeader, std::shared_ptr<Decryptor> decryptor)
		: file(file)
		, cacheKey(cacheKey)
	{
		packedFile->Seek(internalHeader.GetOffset());
		packedFile->ReadBytes(m_buffer, internalHeader.GetSize(), decryptor.get(), internalHeader.GetOffset());
		loadState = LoadState::Loaded;

		m_path = packedFile->GetFullPath() + ToAString("?") + internalHeader.GetFileName();

		SafeAddRef(file);

		isInPackage = true;
	}

	StaticFile_Imp::StaticFile_Imp(
		File_Imp * file,
		Synchronizer::Ptr sync,
		const astring & cacheKey,
		std::shared_ptr<BaseFile>& baseFile)
		: file(file)
		, cacheKey(cacheKey)
	{
		loadThread = std::thread([this, baseFile]()
		{
			baseFile->ReadAllBytes(m_buffer);
			loadState = LoadState::Loaded;
		});
		sync->Start(this, [](){});
		loadState = LoadState::Loading;

		m_path = baseFile->GetFullPath();

		SafeAddRef(file);

		isInPackage = false;
	}

	StaticFile_Imp::StaticFile_Imp(File_Imp * file,
		Synchronizer::Ptr sync,
		const astring & cacheKey,
		const std::shared_ptr<BaseFile>& packedFile,
		PackFileInternalHeader & internalHeader,
		std::shared_ptr<Decryptor> decryptor)
		: file(file)
		, cacheKey(cacheKey)
	{
		loadThread = std::thread([this, packedFile, &internalHeader, decryptor]()
		{
			std::lock_guard<std::mutex> lock(this->file->packedFileMutex);
			packedFile->Seek(internalHeader.GetOffset());
			packedFile->ReadBytes(m_buffer, internalHeader.GetSize(), decryptor.get(), internalHeader.GetOffset());
			loadState = LoadState::Loaded;
		});
		loadState = LoadState::Loading;

		m_path = packedFile->GetFullPath() + ToAString("?") + internalHeader.GetFileName();

		SafeAddRef(file);

		isInPackage = true;
	}

	StaticFile_Imp::~StaticFile_Imp()
	{
		if (loadThread.joinable())
		{
			loadThread.detach();
		}
		file->UnregisterStaticFile(cacheKey);
		SafeRelease(file);
	}

	bool StaticFile_Imp::GetIsInPackage() const
	{
		return isInPackage;
	}
}
