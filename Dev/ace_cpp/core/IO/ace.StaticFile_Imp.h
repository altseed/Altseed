#pragma once

#include "../ace.Core.Base.h"
#include "ace.StaticFile.h"
#include "ace.BaseFile.h"
#include "ace.PackFile.h"

#include <memory>

namespace ace
{
	class StaticFile_Imp
		: public StaticFile
		, public ReferenceObject
	{
	protected:
		File_Imp*	file = nullptr;
		astring		cacheKey;

		std::vector<uint8_t> m_buffer;
		astring m_path;

		bool	isInPackage = false;

	public:
		StaticFile_Imp(File_Imp* file, const astring& cacheKey, std::shared_ptr<BaseFile>& baseFile);
		StaticFile_Imp(File_Imp* file, const astring& cacheKey, const std::shared_ptr<BaseFile>& packedFile, PackFileInternalHeader& internalHeader, std::shared_ptr<Decryptor> decryptor);

		virtual ~StaticFile_Imp();
		
		const std::vector<uint8_t>& GetBuffer() const { return m_buffer; }
		const achar* GetFullPath() const { return m_path.c_str(); }
		void* GetData() { return static_cast<void*>(m_buffer.data()); }
		int32_t GetSize() { return static_cast<int32_t>(m_buffer.size()); }

		bool GetIsInPackage() const override;

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
	};
}