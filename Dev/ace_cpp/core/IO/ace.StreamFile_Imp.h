#pragma once

#include "../ace.Core.Base.h"
#include "ace.StreamFile.h"
#include "ace.BaseFile_Imp.h"
#include <memory>

namespace ace
{
	class StreamFile_Imp : public StreamFile, public ReferenceObject
	{
	private:
		std::shared_ptr<BaseFile_Imp> m_file;

	public:
		StreamFile_Imp(std::shared_ptr<BaseFile_Imp>& file);
		virtual ~StreamFile_Imp() { };
		virtual void ReadAllByte(std::vector<uint8_t>& buffer);
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
	};
}
