
#include "asd.MassModel_IO.h"
#include "asd.Model_IO.h"

namespace asd
{
	struct WeightIndex
	{
		uint8_t	Weight;
		uint8_t	Index;
	};

	bool MassModel_IO::Convert(Model_IO& model)
	{
		Reset();

		if (model.Meshes.size() != 1) return false;
		if (model.Meshes[0].DividedMeshes.size() != 1) return false;
		
		auto& materials = model.Meshes[0].Materials;
		auto& mesh = model.Meshes[0].DividedMeshes[0];
		auto& deformer = model.Deformer_;

		std::array<WeightIndex, 4> wis;

		for (auto& v : mesh.Vertices)
		{
			Vertex v_;

			uint8_t* weights = (uint8_t*) v.BoneWeights;
			uint8_t* indexes = (uint8_t*) v.BoneIndexes;

			for (int32_t i = 0; i < 4; i++)
			{
				wis[i].Weight = weights[i];
				wis[i].Index = indexes[i];
			}

			std::sort(wis.begin(), wis.end(), 
				[](const WeightIndex&left, const WeightIndex& right){
				return left.Weight > right.Weight;
			});

			for (int32_t i = 0; i < 4; i++)
			{
				weights[i] = wis[i].Weight;
				indexes[i] = wis[i].Index;
			}

			float restWeights = weights[2] + weights[3];
			weights[0] += (restWeights / 2.0f);
			weights[1] += (restWeights / 2.0f);
			weights[2] = 0;
			weights[3] = 0;

			v_.Position = v.Position;
			v_.Normal = v.Normal;
			v_.Binormal = v.Binormal;
			v_.BoneIndexes = *((int32_t*)v.BoneIndexes);
			v_.BoneWeights = *((int32_t*) v.BoneWeights);
			v_.UV1 = v.UV1;
			v_.UV2 = v.UV2;
			v_.VColor = v.VColor;
			Vertices.push_back(v_);
		}

		for (auto& f : mesh.Faces)
		{
			Face f_;
			f_.Index1 = f.Indexes[0];
			f_.Index2 = f.Indexes[1];
			f_.Index3 = f.Indexes[2];
			Faces.push_back(f_);
		}

		if (materials.size() > 0)
		{
			Material_.ColorTexture = materials[0].ColorTexture;
			Material_.NormalTexture = materials[0].NormalTexture;
			Material_.MetalnessTexture = materials[0].MetalnessTexture;

			Material_.OriginalColorTexture = materials[0].OriginalColorTexture;
			Material_.OriginalNormalTexture = materials[0].OriginalNormalTexture;
			Material_.OriginalMetalnessTexture = materials[0].OriginalMetalnessTexture;

			Material_.Type = materials[0].Type;
		}

		if (model.AnimationClips.size() > 0)
		{
			// アニメーションあり
			for (auto& ac : model.AnimationClips)
			{
				AnimationClip ac_;
				ac_.Index = ac.Index;
				ac_.Name = ac.Name;
				AnimationClips.push_back(ac_);
			}

			AnimationTexture_.AnimationCount = model.AnimationSources.size();

			for (auto& as : model.AnimationSources)
			{
				float frame = 0;

				for (auto& ks : as.KeyframeAnimations)
				{
					if (ks.Keyframes.size() == 0) continue;
					auto lastTime = (*(ks.Keyframes.end() - 1)).KeyValue.X;
					frame = Max(lastTime, frame);
				}
				AnimationTexture_.FrameCount.push_back(frame);
			}

			int32_t frameMax = 0;
			for (auto& f : AnimationTexture_.FrameCount)
			{
				frameMax = Max(frameMax, f);
			}

			AnimationTexture_.TextureWidth = frameMax;
			AnimationTexture_.TextureHeight = AnimationTexture_.AnimationCount * 32 * 4;

			AnimationTexture_.Buffer.resize(AnimationTexture_.TextureWidth * AnimationTexture_.TextureHeight);

			struct BoneValue
			{
				float Position[3];
				float Rotation[4];
				float Scale[3];
			};

			std::vector<Matrix44> localMatrixes;
			std::vector<Matrix44> boneMatrixes;
			std::vector<BoneValue> boneValues;
			std::map<astring, int32_t> nameToBoneIndex;

			localMatrixes.resize(deformer.Bones.size());
			boneValues.resize(deformer.Bones.size());

			auto& boneConnectors = mesh.BoneConnectors;
			if (boneConnectors.size() > 0)
			{
				boneMatrixes.resize(boneConnectors.size());
			}
			else
			{
				boneMatrixes.resize(1);
			}

			for (auto i = 0; i < deformer.Bones.size(); i++)
			{
				auto& b = deformer.Bones[i];
				nameToBoneIndex[b.Name] = i;
			}

			for (auto i = 0; i < AnimationTexture_.AnimationCount; i++)
			{
				auto& as = model.AnimationSources[i];

				for (auto t = 0; t < AnimationTexture_.FrameCount[i]; t++)
				{
					for (auto& kf : as.KeyframeAnimations)
					{
						astring targetName;
						AnimationCurveTargetType targetType;
						AnimationCurveTargetAxis targetAxis;

						ModelUtils::GetAnimationTarget(targetName, targetType, targetAxis, kf.Name);

						if (nameToBoneIndex.find(targetName) == nameToBoneIndex.end()) continue;
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
							deformer.Bones[index].RotationType);
					}

					ModelUtils::CalculateBoneMatrixes(
						localMatrixes,
						deformer.Bones,
						localMatrixes);

					// 行列計算
					if (boneConnectors.size() > 0)
					{
						// ボーンあり
						for (int32_t i = 0; i < Min(32, boneConnectors.size()); i++)
						{
							boneMatrixes[i].SetIdentity();
							Matrix44::Mul(boneMatrixes[i], localMatrixes[boneConnectors[i].TargetIndex], boneConnectors[i].OffsetMatrix);
						}
					}
					else
					{
						// ボーンなし
						boneMatrixes[0].SetIdentity();
					}

					for (auto j = 0; j < boneMatrixes.size(); j++)
					{
						for (auto k = 0; k < 4; k++)
						{
							int32_t x = t;
							int32_t y = i * 32 * 4 + j * 4 + k;

							AnimationTexture_.Buffer[x + y * AnimationTexture_.TextureWidth].X = boneMatrixes[j].Values[k][0];
							AnimationTexture_.Buffer[x + y * AnimationTexture_.TextureWidth].Y = boneMatrixes[j].Values[k][1];
							AnimationTexture_.Buffer[x + y * AnimationTexture_.TextureWidth].Z = boneMatrixes[j].Values[k][2];
							AnimationTexture_.Buffer[x + y * AnimationTexture_.TextureWidth].W = boneMatrixes[j].Values[k][3];
						}
					}
				}
			}
		}
		else
		{
			// アニメーションなし
			AnimationTexture_.AnimationCount = 0;
			AnimationTexture_.TextureWidth = 1;
			AnimationTexture_.TextureHeight = 1 * 32 * 4;

			AnimationTexture_.Buffer.resize(AnimationTexture_.TextureWidth * AnimationTexture_.TextureHeight);

			struct BoneValue
			{
				float Position[3];
				float Rotation[4];
				float Scale[3];
			};

			std::vector<Matrix44> localMatrixes;
			std::vector<Matrix44> boneMatrixes;
			std::vector<BoneValue> boneValues;
			std::map<astring, int32_t> nameToBoneIndex;

			localMatrixes.resize(deformer.Bones.size());
			boneValues.resize(deformer.Bones.size());

			auto& boneConnectors = mesh.BoneConnectors;
			if (boneConnectors.size() > 0)
			{
				boneMatrixes.resize(boneConnectors.size());
			}
			else
			{
				boneMatrixes.resize(1);
			}

			for (auto i = 0; i < deformer.Bones.size(); i++)
			{
				auto& b = deformer.Bones[i];
				nameToBoneIndex[b.Name] = i;
			}

			for (auto i = 0; i < deformer.Bones.size(); i++)
			{
				auto& b = deformer.Bones[i];
				localMatrixes[i] = b.LocalMat;			
			}

			ModelUtils::CalculateBoneMatrixes(
				localMatrixes,
				deformer.Bones,
				localMatrixes);

			// 行列計算
			if (boneConnectors.size() > 0)
			{
				// ボーンあり
				for (int32_t i = 0; i < Min(32, boneConnectors.size()); i++)
				{
					boneMatrixes[i].SetIdentity();
					Matrix44::Mul(boneMatrixes[i], localMatrixes[boneConnectors[i].TargetIndex], boneConnectors[i].OffsetMatrix);
				}
			}
			else
			{
				// ボーンなし
				boneMatrixes[0].SetIdentity();
			}

			for (auto j = 0; j < boneMatrixes.size(); j++)
			{
				for (auto k = 0; k < 4; k++)
				{
					int32_t x = 0;
					int32_t y = 0 * 32 * 4 + j * 4 + k;

					AnimationTexture_.Buffer[x + y * AnimationTexture_.TextureWidth].X = boneMatrixes[j].Values[k][0];
					AnimationTexture_.Buffer[x + y * AnimationTexture_.TextureWidth].Y = boneMatrixes[j].Values[k][1];
					AnimationTexture_.Buffer[x + y * AnimationTexture_.TextureWidth].Z = boneMatrixes[j].Values[k][2];
					AnimationTexture_.Buffer[x + y * AnimationTexture_.TextureWidth].W = boneMatrixes[j].Values[k][3];
				}
			}
			
			AnimationTexture_.AnimationCount = 0;
			AnimationTexture_.TextureWidth = 0;
			AnimationTexture_.TextureHeight = 0;

			AnimationTexture_.Buffer.clear();

			// 頂点に適用
			for (size_t i = 0; i < Vertices.size(); i++)
			{
				auto& v = Vertices[i];
				
				uint8_t* weights = (uint8_t*) (&v.BoneWeights);
				uint8_t* indexes = (uint8_t*) (&v.BoneIndexes);

				Matrix44 mat;

				for (auto c = 0; c < 4; c++)
				{
					for (auto r = 0; r < 4; r++)
					{
						mat.Values[r][c] =
							boneMatrixes[indexes[0]].Values[r][c] * (weights[0] / 255.0f) +
							boneMatrixes[indexes[1]].Values[r][c] * (weights[1] / 255.0f);
					}
				}

				auto pn = v.Position + v.Normal;
				auto pb = v.Position + v.Binormal;

				v.Position = mat.Transform3D(v.Position);
				pn = mat.Transform3D(pn);
				pb = mat.Transform3D(pb);
				v.Normal = (pn - v.Position).GetNormal();
				v.Binormal = (pb - v.Position).GetNormal();
			}
			
		}
		
		return true;
	}

	void MassModel_IO::Reset()
	{
		Vertices.clear();
		Faces.clear();
		AnimationClips.clear();
		AnimationTexture_.FrameCount.clear();
		AnimationTexture_.Buffer.clear();
	}

	bool MassModel_IO::Load(std::vector<uint8_t>& data, const achar* path)
	{
		BinaryReader reader;
		reader.ReadIn(data.begin(), data.end());

		// ヘッダーチェック
		uint8_t header_true [] = "MM ";
		for (int32_t i = 0; i < 4; i++)
		{
			auto h = reader.Get<uint8_t>();
			if (header_true[i] != h) return false;
		}

		// バージョン
		int32_t version = reader.Get<int32_t>();

		// 頂点
		int32_t vcount = reader.Get<int32_t>();
		Vertices.resize(vcount);

		for (int32_t i = 0; i < vcount; i++)
		{
			auto pos = reader.Get<Vector3DF>();
			auto normal = reader.Get<Vector3DF>();
			auto binormal = reader.Get<Vector3DF>();
			auto uv = reader.Get<Vector2DF>();
			auto subuv = reader.Get<Vector2DF>();
			auto color = reader.Get<Color>();
			auto weights = reader.Get<int32_t>();
			auto indexes = reader.Get<int32_t>();

			Vertices[i].Position = pos;
			Vertices[i].Normal = normal;
			Vertices[i].Binormal = binormal;
			Vertices[i].UV1 = uv;
			Vertices[i].UV2 = subuv;
			Vertices[i].VColor = color;

			Vertices[i].BoneWeights = weights;
			Vertices[i].BoneIndexes = indexes;
		}

		// 面
		int32_t fcount = reader.Get<int32_t>();
		Faces.resize(fcount);

		for (int32_t i = 0; i < fcount; i++)
		{
			auto f1 = reader.Get<int32_t>();
			auto f2 = reader.Get<int32_t>();
			auto f3 = reader.Get<int32_t>();

			Faces[i].Index1 = f1;
			Faces[i].Index2 = f2;
			Faces[i].Index3 = f3;
		}

		// 材質
		Material_.Type = reader.Get<int32_t>();
		if (Material_.Type == 1)
		{
			auto path = reader.Get<astring>();
		}
		else
		{
			Material_.OriginalColorTexture = reader.Get<astring>();
			Material_.OriginalNormalTexture = reader.Get<astring>();
			Material_.OriginalMetalnessTexture = reader.Get<astring>();

			if (Material_.OriginalColorTexture != astring()) Material_.ColorTexture = CombinePath(path, Material_.OriginalColorTexture.c_str());
			if (Material_.OriginalNormalTexture != astring()) Material_.NormalTexture = CombinePath(path, Material_.OriginalNormalTexture.c_str());
			if (Material_.OriginalMetalnessTexture != astring()) Material_.MetalnessTexture = CombinePath(path, Material_.OriginalMetalnessTexture.c_str());
		}

		// アニメーション
		int32_t acount = reader.Get<int32_t>();
		AnimationClips.resize(acount);
		for (size_t i = 0; i < AnimationClips.size(); i++)
		{
			AnimationClips[i].Name = reader.Get<astring>();
			AnimationClips[i].Index = reader.Get<int32_t>();
		}

		AnimationTexture_.AnimationCount = reader.Get<int32_t>();

		int32_t afcount = reader.Get<int32_t>();
		AnimationTexture_.FrameCount.resize(afcount);
		for (size_t i = 0; i < AnimationTexture_.FrameCount.size(); i++)
		{
			AnimationTexture_.FrameCount[i] = reader.Get<int32_t>();
		}

		AnimationTexture_.TextureWidth = reader.Get<int32_t>();
		AnimationTexture_.TextureHeight = reader.Get<int32_t>();
		AnimationTexture_.Buffer.resize(AnimationTexture_.TextureWidth * AnimationTexture_.TextureHeight);
		for (auto i = 0; i < AnimationTexture_.TextureWidth * AnimationTexture_.TextureHeight; i++)
		{
			AnimationTexture_.Buffer[i].X = reader.Get<float>();
			AnimationTexture_.Buffer[i].Y = reader.Get<float>();
			AnimationTexture_.Buffer[i].Z = reader.Get<float>();
			AnimationTexture_.Buffer[i].W = reader.Get<float>();
		}
	
		return true;
	}

	bool MassModel_IO::Save(BinaryWriter& writer, const achar* path)
	{
		// ヘッダー
		uint8_t header [] = "MM ";
		for (int32_t i = 0; i < 4; i++)
		{
			writer.Push(header[i]);
		}

		// バージョン
		writer.Push(0);

		// 頂点
		writer.Push((int32_t) Vertices.size());

		for (size_t i = 0; i < Vertices.size(); i++)
		{
			Vertex& v = Vertices.at(i);
			writer.Push(v.Position);
			writer.Push(v.Normal);
			writer.Push(v.Binormal);
			writer.Push(v.UV1);
			writer.Push(v.UV2);
			writer.Push(v.VColor.R);
			writer.Push(v.VColor.G);
			writer.Push(v.VColor.B);
			writer.Push(v.VColor.A);

			writer.Push(v.BoneWeights);

			writer.Push(v.BoneIndexes);
		}

		// 面
		writer.Push((int32_t) Faces.size());
		for (size_t i = 0; i < Faces.size(); i++)
		{
			Face& f = Faces.at(i);
			writer.Push(f.Index1);
			writer.Push(f.Index2);
			writer.Push(f.Index3);
		}

		// 材質
		if (Material_.Type == 1)
		{
			writer.Push((int32_t) 1);
			astring path;
			writer.Push(path);
		}
		else
		{
			writer.Push((int32_t) 0);
			writer.Push(Material_.OriginalColorTexture.c_str());
			writer.Push(Material_.OriginalNormalTexture.c_str());
			writer.Push(Material_.OriginalMetalnessTexture.c_str());
		}

		// アニメーション
		writer.Push((int32_t) AnimationClips.size());
		for (size_t i = 0; i < AnimationClips.size(); i++)
		{
			writer.Push(AnimationClips[i].Name);
			writer.Push(AnimationClips[i].Index);
		}

		// テクスチャ
		writer.Push((int32_t) AnimationTexture_.AnimationCount);

		writer.Push((int32_t) AnimationTexture_.FrameCount.size());
		for (auto& fc : AnimationTexture_.FrameCount)
		{
			writer.Push(fc);
		}

		writer.Push((int32_t) AnimationTexture_.TextureWidth);
		writer.Push((int32_t) AnimationTexture_.TextureHeight);

		for (auto i = 0; i < AnimationTexture_.TextureWidth * AnimationTexture_.TextureHeight; i++)
		{
			writer.Push(AnimationTexture_.Buffer[i].X);
			writer.Push(AnimationTexture_.Buffer[i].Y);
			writer.Push(AnimationTexture_.Buffer[i].Z);
			writer.Push(AnimationTexture_.Buffer[i].W);
		}
	
		return true;
	}
}