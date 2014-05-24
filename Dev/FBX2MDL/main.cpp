
#include "FBX2MDL.Base.h"
#include "MDLExporter.h"

#if _DEBUG
#pragma comment(lib,"debug/libfbxsdk-mt.lib")
#pragma comment(lib,"Debug/ace_common.lib")
#else
#pragma comment(lib,"release/libfbxsdk-mt.lib")
#pragma comment(lib,"Release/ace_common.lib")
#endif

#if _WIN32
#include <Windows.h>
std::wstring ToWide(const char* pText);
void GetDirectoryName(char* dst, char* src);
#endif

#define TEST 1

#if TEST
int main(int argc, char** argv)
{
#if _WIN32
	char current_path[MAX_PATH + 1];
	GetDirectoryName(current_path, argv[0]);
	SetCurrentDirectoryA(current_path);
#endif

	MDLExporter *exporter = new MDLExporter("Data/Model/Box.fbx", "Data/Model/Box.mdl");
	exporter->Convert();
	delete exporter;
}

#else
int main(int argc, char** argv)
{
	if(argc!=3)
	{
		printf("コマンドの使い方が間違ってます。\n");
		return -1;
	}

    const char* lFilename = argv[1];
	const char* lExportName = argv[2];
    
	MDLExporter *exporter= new MDLExporter(lFilename,lExportName);

	exporter->Convert();
	
	delete exporter;

    return 0;
}
#endif


#if _WIN32
static std::wstring ToWide(const char* pText)
{
	int Len = ::MultiByteToWideChar(CP_ACP, 0, pText, -1, NULL, 0);

	wchar_t* pOut = new wchar_t[Len + 1];
	::MultiByteToWideChar(CP_ACP, 0, pText, -1, pOut, Len);
	std::wstring Out(pOut);
	delete [] pOut;

	return Out;
}

void GetDirectoryName(char* dst, char* src)
{
	auto Src = std::string(src);
	int pos = 0;
	int last = 0;
	while (Src.c_str()[pos] != 0)
	{
		dst[pos] = Src.c_str()[pos];

		if (Src.c_str()[pos] == L'\\' || Src.c_str()[pos] == L'/')
		{
			last = pos;
		}

		pos++;
	}

	dst[pos] = 0;
	dst[last] = 0;
}
#endif