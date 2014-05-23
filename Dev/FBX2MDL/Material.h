
#pragma once

struct Material
{
	//メッシュグループに属した後のインデックス
	int groupIndex;

	//テクスチャタイプ(内部もしくは外部)
	int Type;

	//テクスチャパス
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