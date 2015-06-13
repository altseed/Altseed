#include "FontData.h"
#include <Utility/asd.BinaryWriter.h>

using namespace std;

namespace FontGenerator
{
	void FontData::WriteOut(ostream stream)
	{
		asd::BinaryWriter writer;

		writer.Push(sheetNumber);
		writer.Push(x);
		writer.Push(y);
		writer.Push(width);
		writer.Push(height);

		writer.WriteOut(stream);
	}
}