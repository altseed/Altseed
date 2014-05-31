
#pragma once

#include <fbxsdk.h>

#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Matrix44.h>
#include <Graphics/ace.Color.h>

#include <Utility/ace.BinaryWriter.h>

namespace FBX2MDL
{
	class FBXLoader;
	class Mesh;

	static ace::Matrix44 ToAce(FbxAMatrix& mat)
	{
		ace::Matrix44 mat_;

		for (auto c = 0; c < 4; c++)
		{
			for (auto r = 0; r < 4; r++)
			{
				mat_.Values[c][r] = mat.Get(c, r);
			}
		}

		mat_.SetTransposed();

		return mat_;
	}
}