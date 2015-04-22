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
		std::vector<uint8_t> m_buffer;
		astring m_path;

	public:
		StaticFile_Imp(std::shared_ptr<BaseFile>& file);
		StaticFile_Imp(std::shared_ptr<BaseFile>& packedFile, PackFileInternalHeader& internalHeader);

		virtual ~StaticFile_Imp();
		virtual const std::vector<uint8_t>& GetBuffer() const { return m_buffer; }
		virtual const achar* GetFullPath() const { return m_path.c_str(); }
		virtual void* GetData() { return static_cast<void*>(m_buffer.data()); }
		virtual int32_t GetSize() { return static_cast<int32_t>(m_buffer.size()); }
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
	};
}