#include "ace.BaseFile_Imp.h"
#include <sstream>

namespace ace
{
	BaseFile_Imp::BaseFile_Imp(const Path& fileName) :
		BaseFile_Imp(fileName.ToAstring())
	{
	}

	BaseFile_Imp::BaseFile_Imp(const astring& path) :
		m_position(0),
		m_length(-1)
	{
		m_file.open(
#ifdef _WIN32
			path,
#else
			ToUtf8String(path.c_str()),
#endif
			std::basic_ios<uint8_t>::in | std::basic_ios<uint8_t>::binary);
	}

	BaseFile_Imp::~BaseFile_Imp()
	{
		m_file.close();
	}

	int64_t BaseFile_Imp::Size()
	{
		if (m_length < 0)
		{
			m_file.seekg(0, m_file.end);
#ifdef _WIN32
			m_length = m_file.tellg().seekpos();
#else
			m_length = m_file.tellg();
#endif
			m_file.seekg(0, m_file.beg);
		}
		return m_length;
	}

	void BaseFile_Imp::ReadBytes(std::vector<uint8_t>& buffer, const int64_t count)
	{
		const auto size = Size();

		if (!count)
		{
			buffer.resize(0);
			buffer.clear();
			return;
		}

		assert(0 <= count && count <= size);
		assert((m_position + count) <= size);

		buffer.resize(count);
		m_file.read(reinterpret_cast<char*>(&buffer[0]), count);

		m_position += count;
	}

	uint32_t BaseFile_Imp::ReadUInt32()
	{
		std::vector<uint8_t> buffer;
		ReadBytes(buffer, sizeof(uint32_t));

		return *reinterpret_cast<const uint32_t*>(buffer.data());
	}

	uint64_t BaseFile_Imp::ReadUInt64()
	{
		std::vector<uint8_t> buffer;
		ReadBytes(buffer, sizeof(uint64_t));

		return *reinterpret_cast<const uint64_t*>(buffer.data());
	}

	void BaseFile_Imp::ReadAllBytes(std::vector<uint8_t>& buffer)
	{
		const auto tmp = m_position;
		ReadBytes(buffer, Size());

		m_position = tmp;
	}

	void BaseFile_Imp::ReadAllLines(std::vector<astring>& lines)
	{
	}

	void BaseFile_Imp::ReadAllText(astring& text)
	{
	}

	void BaseFile_Imp::ReadAllText(astring& text, const std::locale& locale)
	{
	}

	void BaseFile_Imp::Seek(const int64_t offset, const SeekOrigin seekOrigin)
	{
		const auto size = Size();

		switch (seekOrigin)
		{
		case SeekOrigin::Begin:
			assert(0 <= offset && offset < size);
			m_file.seekg(offset, m_file.beg);
			break;
		case SeekOrigin::Current:
			assert(0 <= m_position + offset && m_position + offset < size);
			m_file.seekg(offset, m_file.cur);
			break;
		case SeekOrigin::End:
			assert(0 <= offset + size && offset <= 0);
			m_file.seekg(offset, m_file.end);
			break;
		default:
			assert(false);
			break;
		}

	}
}