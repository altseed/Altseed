
#include "ace.MassModel_IO.h"
#include "ace.Model_IO.h"

namespace ace
{
	bool MassModel_IO::Convert(Model_IO& model)
	{
		Reset();

		if (model.MeshGroups.size() >= 2) return false;
		if (model.MeshGroups.size() == 0) return false;

		if (model.MeshGroups[0].Mesh_.BoneOffsets.size() >= 2) return false;
		if (model.MeshGroups[0].Mesh_.BoneOffsets.size() == 0) return false;

		auto& mg = model.MeshGroups[0];

		for (auto& v : mg.Mesh_.Vertices)
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

		for (auto& f : mg.Mesh_.Faces)
		{
			Face f_;
			f_.Index1 = f.Index1;
			f_.Index2 = f.Index2;
			f_.Index3 = f.Index3;
			Faces.push_back(f_);
		}

		for (auto& mo : mg.Mesh_.MaterialOffsets)
		{
			MaterialOffset mo_;
			mo_.FaceOffset = mo.FaceOffset;
			mo_.MaterialIndex = mo.MaterialIndex;
			MaterialOffsets.push_back(mo_);
		}

		for (auto& m : mg.Materials)
		{
			Material m_;
			m_.ColorTexture = m.ColorTexture;
			m_.NormalTexture = m.NormalTexture;
			m_.SpecularTexture = m.SpecularTexture;
			Materials.push_back(m_);
		}

		for (auto& ac : model.AnimationClips)
		{
			AnimationClip ac_;
			ac_.Index = ac.Index;
			ac_.Name = ac.Name;
			AnimationClips.push_back(ac_);
		}

		AnimationTexture.AnimationCount = model.AnimationSources.size();
		AnimationTexture.FrameSkip = 1;

		for (auto& as : model.AnimationSources)
		{
			float frame = 0;

			for (auto& ks : as.KeyframeAnimations)
			{
				if (ks.Keyframes.size() == 0) continue;
				auto lastTime = (*(ks.Keyframes.end() - 1)).KeyValue.X;
				frame = Max(lastTime, frame);
			}
			AnimationTexture.FrameCount.push_back(frame);
		}

		int32_t frameMax = 0;
		for (auto& f : AnimationTexture.FrameCount)
		{
			frameMax = Max(frameMax, f);
		}

		AnimationTexture.TextureWidth = frameMax * 4;
		AnimationTexture.TextureHeight = AnimationTexture.AnimationCount * 32;

		AnimationTexture.Buffer.resize(AnimationTexture.TextureWidth * AnimationTexture.TextureHeight);

		struct BoneValue
		{
			float Position[3];
			float Rotation[4];
			float Scale[3];
		};

		std::vector<Matrix44> localMatrixes;
		std::vector<BoneValue> boneValues;
		std::map<astring, int32_t> nameToBoneIndex;

		localMatrixes.resize(mg.Deformer_.Bones.size());
		boneValues.resize(mg.Deformer_.Bones.size());

		for (auto i = 0; i < mg.Deformer_.Bones.size(); i++)
		{
			auto& b = mg.Deformer_.Bones[i];
			nameToBoneIndex[b.Name] = i;
		}

		for (auto i = 0; i < AnimationTexture.AnimationCount; i++)
		{
			auto& as = model.AnimationSources[i];

			for (auto t = 0; t < AnimationTexture.FrameCount[i]; t++)
			{
				for (auto& kf : as.KeyframeAnimations)
				{
					astring targetName;
					eAnimationCurveTargetType targetType;
					eAnimationCurveTargetAxis targetAxis;

					ModelUtils::GetAnimationTarget(targetName, targetType, targetAxis, kf.Name);

					if(nameToBoneIndex.find(targetName) == nameToBoneIndex.end()) continue;
					auto index = nameToBoneIndex[targetName];
					auto value = ModelUtils::GetKeyframeValue(t, kf.Keyframes);
					ModelUtils::SetBoneValue(
						boneValues[index].Position,
						boneValues[index].Rotation,
						boneValues[index].Scale,
						targetType,
						targetAxis,
						value);

					localMatrixes[index] = ModelUtils::CalcMatrix(
						boneValues[index].Position,
						boneValues[index].Rotation,
						boneValues[index].Scale,
						mg.Deformer_.Bones[index].RotationType);
				}

				ModelUtils::CalculateBoneMatrixes(
					localMatrixes,
					mg.Deformer_.Bones,
					localMatrixes);
				
				for (auto j = 0; j < mg.Deformer_.Bones.size(); j++)
				{
					int32_t x = t * 4;
					int32_t y = i * 32 + j;

					for (auto k = 0; k < 4; k++)
					{
						AnimationTexture.Buffer[x + k + y * AnimationTexture.TextureWidth].X = localMatrixes[j].Values[k][0];
						AnimationTexture.Buffer[x + k + y * AnimationTexture.TextureWidth].X = localMatrixes[j].Values[k][1];
						AnimationTexture.Buffer[x + k + y * AnimationTexture.TextureWidth].X = localMatrixes[j].Values[k][2];
						AnimationTexture.Buffer[x + k + y * AnimationTexture.TextureWidth].X = localMatrixes[j].Values[k][3];
					}
				}
			}
		}

		return true;
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