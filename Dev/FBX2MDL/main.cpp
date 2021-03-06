
#include "FBX2MDL.Base.h"
#include "FBX2MDL.FBXImporter.h"
#include "FBX2MDL.FBXExporter.h"
#include "FBX2MDL.FBXOptimizer.h"

#include <Graphics/3D/asd.Model_IO.h>

#if _DEBUG
#pragma comment(lib,"debug/libfbxsdk-mt.lib")
#pragma comment(lib,"x86/Debug/asd_common.lib")
#pragma comment(lib,"x86/Debug/asd_tool.lib")
#else
#pragma comment(lib,"release/libfbxsdk-mt.lib")
#pragma comment(lib,"x86/Release/asd_common.lib")
#pragma comment(lib,"x86/Release/asd_tool.lib")
#endif

#if _WIN32
#include <Windows.h>
std::wstring ToWide(const char* pText);
void GetDirectoryName(char* dst, char* src);
#endif

//#define TEST 1

#if TEST
int main(int argc, char** argv)
{
#if _WIN32
	char current_path[MAX_PATH + 1];
	GetDirectoryName(current_path, argv[0]);
	SetCurrentDirectoryA(current_path);
#endif

	FBX2MDL::FBXImporter importer;
	FBX2MDL::FBXExporter exporter;
	FBX2MDL::FBXOptimizer optimizer;

	FbxManager* sdkManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(sdkManager, IOSROOT);
	sdkManager->SetIOSettings(ios);

	fbxsdk::FbxImporter* fbxImporter = fbxsdk::FbxImporter::Create(sdkManager, "");
	if (!fbxImporter->Initialize("test.fbx", -1, sdkManager->GetIOSettings()))
	{
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", fbxImporter->GetStatus().GetErrorString());
		system("PAUSE");
		exit(-1);
	}

	FbxScene* fbxScene = FbxScene::Create(sdkManager, "myScene");
	fbxImporter->Import(fbxScene);
	
	auto scene = importer.LoadScene(fbxScene, sdkManager);

	optimizer.Optimize(scene);

	auto writer = exporter.Export(scene);

	fbxScene->Destroy();
	fbxImporter->Destroy();
	sdkManager->Destroy();

	writer->WriteOut("test.mdl");

	
	auto buf = writer->Get();
	auto buf_ = std::vector<uint8_t>();

	for (size_t i = 0; i < buf.size(); i++)
	{
		auto b = ((uint8_t*) buf.data())[i];
		buf_.push_back(b);
	}

	asd::Model_IO model_io;
	model_io.Load(buf_, asd::ToAString("./").c_str());
	

	/*
	MDLExporter *exporter = new MDLExporter("Data/Model/AnimationTest.fbx", "Data/Model/AnimationTest.mdl");
	exporter->Convert();
	delete exporter;
	*/

	return 0;
}

#else
int main(int argc, char** argv)
{
	if(argc!=3)
	{
		printf("引数が足りません。\n");
		return -1;
	}

	const char* importPath = argv[1];
	const char* exportPath = argv[2];

	FBX2MDL::FBXImporter importer;
	FBX2MDL::FBXExporter exporter;
	FBX2MDL::FBXOptimizer optimizer;

	FbxManager* sdkManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(sdkManager, IOSROOT);
	sdkManager->SetIOSettings(ios);

	fbxsdk::FbxImporter* fbxImporter = fbxsdk::FbxImporter::Create(sdkManager, "");
	if (!fbxImporter->Initialize(importPath, -1, sdkManager->GetIOSettings()))
	{
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", fbxImporter->GetStatus().GetErrorString());
		system("PAUSE");
		exit(-1);
	}

	FbxScene* fbxScene = FbxScene::Create(sdkManager, "myScene");
	fbxImporter->Import(fbxScene);

	auto scene = importer.LoadScene(fbxScene, sdkManager);

	optimizer.Optimize(scene);

	auto writer = exporter.Export(scene);

	fbxScene->Destroy();
	fbxImporter->Destroy();
	sdkManager->Destroy();

	writer->WriteOut(exportPath);

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