#include "ace.BaseFile_Imp.h"
#include <sstream>

namespace ace
{
	BaseFile_Imp::BaseFile_Imp(const Path& fileName) :
		m_position(0),
		m_length(-1)
	{
		m_file.open(
			fileName.ToAstring(),
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
			m_length = m_file.tellg().seekpos();
			m_file.seekg(0, m_file.beg);
		}
		return m_length;
	}

	void BaseFile_Imp::ReadBytes(std::vector<uint8_t>& buffer, const int64_t count)
	{
		const auto size = Size();
		assert(0 <= count && count < size);
		assert((m_position + count) <= size);

		buffer.resize(count);
		m_file.read(&buffer[0], count);

		m_position += count;
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