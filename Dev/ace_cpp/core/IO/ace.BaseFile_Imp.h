#pragma once

#include "../ace.ReferenceObject.h"
#include "../ace.Core.Base.h"
#include "ace.Path.h"
#include <fstream>

namespace ace
{
	enum class SeekOrigin
	{
		Begin,
		Current,
		End
	};

	class BaseFile_Imp : public ReferenceObject
	{
	private:
		std::basic_ifstream<char> m_file;
		int64_t m_position;
		int64_t m_length;

	public:
		BaseFile_Imp(const Path& fileName);
		BaseFile_Imp(const astring& path);
		virtual ~BaseFile_Imp();

		int64_t Position() const { return m_position; }

		int64_t Size();
		void ReadBytes(std::vector<uint8_t>& buffer, const int64_t count);
		uint32_t ReadUInt32();
		uint64_t ReadUInt64();
		void ReadAllBytes(std::vector<uint8_t>& buffer);
		void ReadAllLines(std::vector<astring>& lines);
		void ReadAllText(astring& text);
		void ReadAllText(astring& text, const std::locale& locale);
		void Seek(const int64_t offset, const SeekOrigin = SeekOrigin::Begin);

		int GetRef() { return ReferenceObject::GetRef(); }
		int AddRef() { return ReferenceObject::AddRef(); }
		int Release() { return ReferenceObject::Release(); }
	};
}
