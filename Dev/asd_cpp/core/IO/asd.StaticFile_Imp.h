#pragma once

#include "../asd.Core.Base.h"
#include "asd.StaticFile.h"
#include "asd.BaseFile.h"
#include "asd.PackFile.h"
#include "../Utils/IAsyncResource.h"
#include "../Utils/asd.Synchronizer.h"

#include <memory>

namespace asd
{
	class StaticFile_Imp
		: public StaticFile
		, public ReferenceObject
		, public IAsyncResource
	{
	protected:
		File_Imp*	file = nullptr;
		astring		cacheKey;

		std::vector<uint8_t> m_buffer;
		astring m_path;

		bool	isInPackage = false;
		LoadState loadState;

	public:
		StaticFile_Imp(File_Imp* file,
			const astring& cacheKey,
			std::shared_ptr<BaseFile>& baseFile);
		StaticFile_Imp(File_Imp* file,
			const astring& cacheKey,
			const std::shared_ptr<BaseFile>& packedFile,
			PackFileInternalHeader& internalHeader,
			std::shared_ptr<Decryptor> decryptor);
		/**
		@brief	ファイルを非同期に読み込む。
		*/
		StaticFile_Imp(File_Imp* file,
			Synchronizer::Ptr sync,
			const astring& cacheKey,
			std::shared_ptr<BaseFile>& baseFile);
		/**
		@brief	ファイルをパッケージから非同期に読み込む。
		*/
		StaticFile_Imp(File_Imp* file,
			Synchronizer::Ptr sync,
			const astring& cacheKey,
			const std::shared_ptr<BaseFile>& packedFile,
			PackFileInternalHeader& internalHeader,
			std::shared_ptr<Decryptor> decryptor);

		virtual ~StaticFile_Imp();
		
		const std::vector<uint8_t>& GetBuffer() const { return m_buffer; }
		const char16_t* GetFullPath() const { return m_path.c_str(); }
		void* GetData() { return static_cast<void*>(m_buffer.data()); }
		int32_t GetSize() { return static_cast<int32_t>(m_buffer.size()); }

		bool GetIsInPackage() const override;
		LoadState GetLoadStateInternal() const override { return loadState; }
		LoadState GetLoadState() const override { return loadState; }

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
	};
}