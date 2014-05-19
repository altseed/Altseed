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

	Material& operator=( const Material &material)
	{
		groupIndex=material.groupIndex;
		Type=material.Type;
		for(int i=0;i<3;++i)
		{
			texture[i]=material.texture[i];
		}

		return *this;
	}
};