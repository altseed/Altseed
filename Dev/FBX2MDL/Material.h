#pragma once
#include <string>

struct Material
{
	int groupIndex;
	int Type;
	std::string texture[3];

	bool operator==(Material material)
	{
		return Type==material.Type && texture[0]==material.texture[0] && texture[1]==material.texture[1] && texture[2] == material.texture[2];
	}
};