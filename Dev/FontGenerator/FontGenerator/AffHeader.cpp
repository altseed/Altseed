#include "AffHeader.h"
#include "utility.h"

using namespace std;

namespace FontGenerator
{
	AffHeader::AffHeader(int fontSize, wstring sheetName)
		: version(AFF_VERSION)
		, fontSize(fontSize)
		, fontWidth(fontSize)
		, fontHeight(fontSize)
		, sheetCount(1)
		, fontCount(1)
	{
		guid[0] = 'A';
		guid[1] = 'F';
		guid[2] = 'F';
		guid[3] = '/0';
		this->sheetName.fill(L'\0');

		for (int i = 0; i < SHEET_NAME_LENGTH && i < sheetName.size(); ++i)
		{
			this->sheetName[i] = sheetName[i];
		}
	}

	vector<char> AffHeader::GetBytes()
	{
		vector<char> result;
		result.insert(result.end(), guid.begin(), guid.end());

		AddRange(result, GetBytesOf(version));
		AddRange(result, GetBytesOf(fontSize));
		AddRange(result, GetBytesOf(fontWidth));
		AddRange(result, GetBytesOf(fontHeight));
		AddRange(result, GetBytesOf(sheetCount));

		for (auto& wc : sheetName)
		{
			AddRange(result, GetBytesOf(wc));
		}

		return result;
	}
}