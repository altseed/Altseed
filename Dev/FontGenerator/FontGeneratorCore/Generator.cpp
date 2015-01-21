
#include <fstream>
#include "Generator.h"
#include <Graphics/Font/ace.AffHeader.h>
#include <Graphics/Font/ace.AffIndexTable.h>
#include <Graphics/Font/ace.GlyphData.h>
#include <Utility/ace.BinaryWriter.h>
#include <Utility/ace.BinaryReader.h>
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;
using namespace ace;

namespace FontGenerator
{
	typedef BOOL(__stdcall *funcNkfConvertSafe)(LPSTR, DWORD, LPDWORD, LPCSTR, DWORD);
	typedef int(__stdcall *funcSetNkfOption)(LPCSTR);

	static void Convert(const char* mode, char* outBuffer, int outBufferSize, const char* inStr, int inStrlength)
	{
		HINSTANCE dll = LoadLibrary(L"nkf32.dll");

		funcSetNkfOption setNkfOption = (funcSetNkfOption)GetProcAddress(dll, "SetNkfOption");
		setNkfOption(mode);

		DWORD returnedBytes = 0;

		funcNkfConvertSafe nkfConvertSafe = (funcNkfConvertSafe)GetProcAddress(dll, "NkfConvertSafe");
		nkfConvertSafe(outBuffer, outBufferSize, &returnedBytes, inStr, inStrlength);
	}

	static vector<achar> ToUtf16(const char* str, int length)
	{
		char buffer[65536];
		int bufferSize = sizeof(buffer);
		memset(buffer, 0, bufferSize);

		Convert("-w16", buffer, bufferSize, str, length);

		vector<achar> result;
		for (size_t i = 0; i <= bufferSize - 2; i += 2)
		{
			char c[2] = { buffer[i + 1], buffer[i] };
			achar* w = reinterpret_cast <achar*>(c);
			if (w[0] != 0 && w[0] != 65279)
			{
				result.push_back(w[0]);
			}
		}

		return result;
	}

	static vector<char> GetBytes(astring filePath)
	{
		vector<char> bytes;
		ifstream fin(filePath.c_str(), ios::binary | ios::in);

		ACE_ASSERT(!fin.fail(), "ファイルは開けませんでした");

		while (!fin.eof())
		{
			char byte;
			fin.read(&byte, 1);
			if (!fin.eof())
			{
				bytes.push_back(byte);
			}
		}

		fin.close();

		return bytes;
	}


	Generator::Generator()
		: m_sheetName(ToAString(L"font"))
		, m_sheetSize(DEFAULT_SIZE)
	{
		m_setting = SettingForRendering();
		m_setting.SetBold(nullptr);
		m_setting.SetBorder(nullptr);
		m_setting.SetFontSize(16);
		m_setting.SetFontColor(Color(0, 0, 0, 255));
	}

	void Generator::GenerateFontFile(
		astring fontPath,
		astring textPath)
	{
		auto result = RenderPng(fontPath, textPath);

		BinaryWriter writer;
		PushAff(writer, result);

		ofstream file(m_sheetName + L".aff", ios::out | ios::binary);
		writer.WriteOut(file);
	}

	ResultOfGeneratingPng Generator::RenderPng(astring fontPath, astring textPath)
	{
		auto charactors = GetCharactors(ToAString(textPath.c_str()));

		PngGenerator png;
		png.SetSetting(m_setting);
		png.SetSheetName(m_sheetName);
		png.SetSheetSize(m_sheetSize);

		auto result = png.Generate(ToAString(fontPath.c_str()), charactors);
		return result;
	}

	void Generator::PushAff(BinaryWriter& writer, ResultOfGeneratingPng& result)
	{
		AffHeader header(m_sheetName);
		header.SetFontSize(m_setting.GetFontSize());
		header.SetFontCount(result.fonts.size());
		header.SetSheetCount(result.sheetCount);

		AffIndexTable indexTable;
		for (int i = 0; i < result.fonts.size(); ++i)
		{
			indexTable.AppendFontIndex(result.fonts[i].GetCharactor(), (int16_t)i);
		}

		header.Push(writer);
		indexTable.Push(writer);
		for (auto& f : result.fonts)
		{
			f.Push(writer);
		}
	}

	vector<achar> Generator::GetCharactors(astring textPath)
	{
		std::vector<achar> charactors;
		auto bytes = GetBytes(textPath);
		auto chars = ToUtf16(bytes.data(), bytes.size());

		std::set<achar> charactorSet;
		for (auto& c : chars)
		{
			charactorSet.insert(c);
		}

		std::vector<achar> charactorVector;
		for (auto& c : charactorSet)
		{
			charactorVector.push_back(c);
		}

		return charactorVector;
	}

	astring Generator::GetSheetName() const
	{
		return m_sheetName;
	}

	void Generator::SetSheetName(astring value)
	{
		m_sheetName = value;
	}

	SettingForRendering Generator::GetSetting() const
	{
		return m_setting;
	}

	void Generator::SetSetting(SettingForRendering value)
	{
		m_setting = value;
	}

	int Generator::GetSheetSize() const
	{
		return m_sheetSize;
	}

	void Generator::SetSheetSize(int value)
	{
		m_sheetSize = value;
	}

}