
#if _DEBUG
#pragma comment(lib,"debug/libfbxsdk-mt.lib")
#pragma comment(lib,"Debug/ace_common.lib")
#else
#pragma comment(lib,"release/libfbxsdk-mt.lib")
#pragma comment(lib,"Release/ace_common.lib")
#endif

#include <fbxsdk.h>
#include <Utility/ace.BinaryWriter.h>

#include "MDLExporter.h"
/**
 * Main function - loads the hard-coded fbx file,
 * and prints its contents in an xml format to stdout.
 */
int main(int argc, char** argv) {

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
