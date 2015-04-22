#pragma once

#include "../ace.ReferenceObject.h"
#include "../ace.Core.Base.h"
#include <fstream>

namespace ace
{
	enum class SeekOrigin
	{
		Begin,
		Current,
		End
	};

	/**
		@brief	直接ファイルを操作するクラス
	*/
	class BaseFile 
		: public ReferenceObject
	{
	private:
		std::basic_ifstream<char> m_file;
		int64_t m_position;
		int64_t m_length;
		astring m_filePath;

	public:
		BaseFile(const astring& path);
		BaseFile(const achar* path);

		virtual ~BaseFile();

		bool IsValid();

		int64_t GetPosition() const { return m_position; }
		const astring& GetFullPath() const { return m_filePath; }

		int64_t GetSize();
		void ReadBytes(std::vector<uint8_t>& buffer, const int64_t count);
		uint32_t ReadUInt32();
		uint64_t ReadUInt64();
		void ReadAllBytes(std::vector<uint8_t>& buffer);

		void Seek(const int64_t offset, const SeekOrigin = SeekOrigin::Begin);
	};
}
