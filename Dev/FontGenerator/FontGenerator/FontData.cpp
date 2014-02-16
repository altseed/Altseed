#include "FontData.h"
#include <Utility/ace.BinaryWriter.h>

using namespace std;

namespace FontGenerator
{
	void FontData::WriteOut(ostream stream)
	{
		ace::BinaryWriter writer;

		writer.Push(sheetNumber);
		writer.Push(x);
		writer.Push(y);
		writer.Push(width);
		writer.Push(height);

		writer.WriteOut(stream);
	}
}