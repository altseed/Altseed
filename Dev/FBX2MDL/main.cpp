#include <fbxsdk.h>
#include "../ace_cpp/common/Utility/ace.BinaryWriter.h"
#include "MDLExporter.h"
/**
 * Main function - loads the hard-coded fbx file,
 * and prints its contents in an xml format to stdout.
 */
int main(int argc, char** argv) {

    // Change the following filename to a suitable filename value.
    const char* lFilename = argv[1];
    
	MDLExporter *exporter= new MDLExporter(lFilename);

	exporter->Convert();
	
	delete exporter;
    return 0;
}