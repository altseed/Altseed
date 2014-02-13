
#include "ace.MassModel_IO.h"
#include "ace.Model_IO.h"

namespace ace
{
	bool MassModel_IO::Convert(Model_IO& model)
	{
		Reset();

		if (model.MeshGroups.size() >= 2) return false;
		if (model.MeshGroups.size() == 0) return false;

		if (model.MeshGroups[0].Mesh.BoneOffsets.size() >= 2) return false;
		if (model.MeshGroups[0].Mesh.BoneOffsets.size() == 0) return false;

		auto& mg = model.MeshGroups[0];

		for (auto& v : mg.Mesh.Vertices)
		{
			Vertex v_;
			v_.Position = v.Position;
			v_.Normal = v.Normal;
			v_.Binormal = v.Binormal;
			v_.BoneIndexes = v.BoneIndexes;
			v_.BoneWeights = v.BoneWeights;
			v_.UV1 = v.UV1;
			v_.UV2 = v.UV2;
			v_.VColor = v.VColor;
			Vertices.push_back(v_);
		}

		return false;
	}

	void MassModel_IO::Reset()
	{
		Vertices.clear();
		Faces.clear();
		MaterialOffsets.clear();
		Materials.clear();
		AnimationClips.clear();
		AnimationTexture.FrameCount.clear();
		AnimationTexture.Buffer.clear();
	}
}