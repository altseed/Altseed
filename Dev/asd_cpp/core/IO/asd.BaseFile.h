#pragma once

#include "../asd.ReferenceObject.h"
#include "../asd.Core.Base.h"

#include "asd.Decryptor.h"

#include <fstream>

namespace asd
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
		void ReadBytes(std::vector<uint8_t>& buffer, const int64_t count, Decryptor* decryptor = nullptr, int64_t globalPos = 0);
		uint32_t ReadUInt32(Decryptor* decryptor = nullptr, int64_t globalPos = 0);
		uint64_t ReadUInt64(Decryptor* decryptor = nullptr, int64_t globalPos = 0);
		void ReadAllBytes(std::vector<uint8_t>& buffer, Decryptor* decryptor = nullptr, int64_t globalPos = 0);

		void Seek(const int64_t offset, const SeekOrigin = SeekOrigin::Begin);
	};
}