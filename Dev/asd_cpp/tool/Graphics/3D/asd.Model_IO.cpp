
#include "asd.Model_IO.h"
#include <cmath>

namespace asd
{
	static const double DEF_NaN = std::numeric_limits<double>::quiet_NaN();

#if _WIN32
	static const double M_PI = 3.14159265358979;
#endif

	static float sqrt3(float value)
	{
		if (value == 0)
		{
			return 0;
		}
		else if (value > 0)
		{
			return (float) pow(value, 1.0f / 3.0f);
		}
		else
		{
			return -(float) pow(-value, 1.0f / 3.0f);
		}
	}

	/**
	@brief	文字列分割
	@note
	http://shnya.jp/blog/?p=195 のコードを改造
	*/
	static std::vector<astring> split(const astring &str, const astring &delim)
	{
		std::vector<astring> res;
		size_t current = 0, found, delimlen = delim.size();
		while ((found = str.find(delim, current)) != astring::npos)
		{
			res.push_back(astring(str, current, found - current));
			current = found + delimlen;
		}
		res.push_back(astring(str, current, str.size() - current));
		return res;
	}

	static void NormalizeValues(float k1[], float k1rh[], float k2lh[], float k2[])
	{
		float h1[2];
		float h2[2];

		// 傾きを求める
		h1[0] = k1[0] - k1rh[0];
		h1[1] = k1[1] - k1rh[1];

		h2[0] = k2[0] - k2lh[0];
		h2[1] = k2[1] - k2lh[1];

		// キーフレーム間の長さ
		auto len = k2[0] - k1[0];

		// キー1の右手の長さ
		auto lenR = abs(h1[0]);

		// キー2の左手の長さ
		auto lenL = abs(h2[0]);

		// 長さがない
		if (lenR == 0 && lenL == 0) return;

		// 手が重なっている場合、補正
		if ((lenR + lenL) > len)
		{
			auto f = len / (lenR + lenL);

			k1rh[0] = (k1[0] - f * h1[0]);
			k1rh[1] = (k1[1] - f * h1[1]);

			k2lh[0] = (k2[0] - f * h2[0]);
			k2lh[1] = (k2[1] - f * h2[1]);
		}
	}

	static void QuadraticFormula(float a, float b, float c, float& r1, float& r2)
	{
		r1 = DEF_NaN;
		r2 = DEF_NaN;

		if (a != 0.0)
		{
			float p = b * b - 4 * a * c;

			if (p > 0)
			{
				p = sqrt(p);
				r1 = (-b - p) / (2 * a);
				r2 = (-b + p) / (2 * a);
			}
			else if (p == 0)
			{
				r1 = -b / (2 * a);
			}
		}
		else if (b != 0.0)
		{
			r1 = -c / b;
		}
		else if (c == 0.0f)
		{
			r1 = 0.0f;
		}
	}

	static bool CalcT(float frame, float k1X, float k1rhX, float k2lhX, float k2X, float& r)
	{
		auto isValid = [](float v) -> bool
		{
			if (std::isnan(v)) return false;

			auto small_ = -0.00001f;
			auto big_ = 1.000001f;
			return (small_ <= v) && (v <= big_);
		};

		auto c3_ = k2X - k1X + 3.0f * (k1rhX - k2lhX);
		auto c2_ = 3.0f * (k1X - 2.0f * k1rhX + k2lhX);
		auto c1_ = 3.0f * (k1rhX - k1X);
		auto c0_ = k1X - frame;

		if (c0_ == 0.0)
		{
			r = 0.0f;
			return true;
		}

		if (c3_ != 0.0)
		{
			auto c2 = c2_ / c3_;
			auto c1 = c1_ / c3_;
			auto c0 = c0_ / c3_;

			auto p = c1 / 3.0f - c2 * c2 / (3.0f * 3.0f);
			auto q = (2.0f * c2 * c2 * c2 / (3.0f*3.0f*3.0f) - c2 / 3.0f * c1 + c0) / 2.0f;
			auto p3q2 = q * q + p * p * p;

			if (p3q2 > 0.0)
			{
				auto t = sqrt(p3q2);
				auto u = sqrt3(-q + t);
				auto v = sqrt3(-q - t);
				r = u + v - c2 / 3.0f;

				if (isValid(r)) return true;
				return false;
			}
			else if (p3q2 == 0.0)
			{
				auto u = sqrt3(-q);
				auto v = sqrt3(-q);
				r = u + v - c2 / 3.0f;
				if (isValid(r)) return true;

				u = -sqrt3(-q);
				v = -sqrt3(-q);
				r = u + v - c2 / 3.0f;
				if (isValid(r)) return true;

				return false;
			}
			else
			{
				// ビエタの解
				auto phi = acos(-q / sqrt(-(p * p * p)));
				auto pd3 = cos(phi / 3);
				auto rmp = sqrt(-p);

				r = 2.0f * rmp * cos(phi / 3) - c2 / 3.0f;
				if (isValid(r)) return true;

				auto q2 = sqrt(3 - 3 * pd3 * pd3);

				//r = -rmp * (pd3 + q2) - a;
				r = 2.0f * rmp * cos(phi / 3 + 2.0 * M_PI / 3.0) - c2 / 3.0f;
				if (isValid(r)) return true;

				//r = -rmp * (pd3 - q2) - a;
				r = 2.0f * rmp * cos(phi / 3 + 4.0 * M_PI / 3.0) - c2 / 3.0f;
				if (isValid(r)) return true;
				return false;
			}
		}
		else
		{
			// 2次方程式のケース
			float r1;
			float r2;
			QuadraticFormula(c2_, c1_, c0_, r1, r2);

			r = r1;
			if (isValid(r)) return true;

			r = r2;
			if (isValid(r)) return true;

			r = 0.0f;
			return false;
		}
	}

	static float CalcBezierValue(float k1Y, float k1rhY, float k2lhY, float k2Y, float t)
	{
		auto c0 = k1Y;
		auto c1 = 3.0f * (k1rhY - k1Y);
		auto c2 = 3.0f * (k1Y - 2.0f * k1rhY + k2lhY);
		auto c3 = k2Y - k1Y + 3.0f * (k1rhY - k2lhY);

		return c0 + t * c1 + t * t * c2 + t * t * t * c3;
	}

	void Model_IO::Int32ToUInt8Array(uint8_t dst[4], int32_t src)
	{
		memcpy(dst, &src, sizeof(int32_t));
	}

	bool Model_IO::Load(const std::vector<uint8_t>& data, const achar* path)
	{
		Meshes.clear();

		BinaryReader reader;
		reader.ReadIn(data.begin(), data.end());

		// ヘッダーチェック
		uint8_t header_true [] = "MDL";
		for (int32_t i = 0; i < 4; i++)
		{
			auto h = reader.Get<uint8_t>();
			if (header_true[i] != h) return false;
		}

		// バージョン
		int32_t version = reader.Get<int32_t>();

		// ボーン
		LoadDeformer(Deformer_, reader, path, version);

		// メッシュ
		LoadMeshes(Meshes, reader, path);

		// アニメーション
		int32_t sourceCount = reader.Get<int32_t>();
		AnimationSources.resize(sourceCount);
		for (int32_t i = 0; i < sourceCount; i++)
		{
			LoadAnimationSource(AnimationSources[i], reader, path);
		}

		int32_t clipCount = reader.Get<int32_t>();
		AnimationClips.resize(clipCount);
		for (int32_t i = 0; i < clipCount; i++)
		{
			LoadAnimationClip(AnimationClips[i], reader, path);
		}

		return true;
	}

	bool Model_IO::Save(BinaryWriter& writer, const achar* path)
	{
		// ヘッダー
		uint8_t header[] = "MDL";
		for (int32_t i = 0; i < 4; i++)
		{
			writer.Push(header[i]);
		}

		// バージョン
		writer.Push(0);

		// ボーン
		SaveDeformer(Deformer_, writer, path);

		// メッシュ
		SaveMeshes(Meshes, writer, path);

		// アニメーション
		writer.Push((int32_t)AnimationSources.size());
		
		for (int32_t i = 0; i < AnimationSources.size(); i++)
		{
			SaveAnimationSource(AnimationSources[i], writer, path);
		}

		writer.Push((int32_t) AnimationClips.size());
		for (int32_t i = 0; i <AnimationClips.size(); i++)
		{
			SaveAnimationClip(AnimationClips[i], writer, path);
		}

		return true;
	}

	void Model_IO::LoadMeshes(std::vector<Mesh>& meshes, BinaryReader& reader, const achar* path)
	{
		auto mcount = reader.Get<int32_t>();
		meshes.resize(mcount);
		
		for (auto i = 0; i < mcount; i++)
		{
			LoadMesh(meshes[i], reader, path);
		}
	}

	void Model_IO::LoadMesh(Mesh& mesh, BinaryReader& reader, const achar* path)
	{
		auto mcount = reader.Get<int32_t>();
		mesh.DividedMeshes.resize(mcount);

		for (auto i = 0; i < mcount; i++)
		{
			LoadDividedMesh(mesh.DividedMeshes[i], reader, path);
		}

		LoadMaterials(mesh.Materials, reader, path);
		
	}

	void Model_IO::LoadDividedMesh(DividedMesh& mesh, BinaryReader& reader, const achar* path)
	{
		int32_t vcount = reader.Get<int32_t>();
		mesh.Vertices.resize(vcount);

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
			auto indexesOriginal = reader.Get<int32_t>();

			mesh.Vertices[i].Position = pos;
			mesh.Vertices[i].Normal = normal;
			mesh.Vertices[i].Binormal = binormal;
			mesh.Vertices[i].UV1 = uv;
			mesh.Vertices[i].UV2 = subuv;
			mesh.Vertices[i].VColor = color;

			Int32ToUInt8Array(mesh.Vertices[i].BoneWeights, weights);
			Int32ToUInt8Array(mesh.Vertices[i].BoneIndexes, indexes);
			Int32ToUInt8Array(mesh.Vertices[i].BoneIndexesOriginal, indexesOriginal);
		}

		int32_t fcount = reader.Get<int32_t>();
		mesh.Faces.resize(fcount);

		for (int32_t i = 0; i < fcount; i++)
		{
			auto f1 = reader.Get<int32_t>();
			auto f2 = reader.Get<int32_t>();
			auto f3 = reader.Get<int32_t>();

			mesh.Faces[i].Indexes[0] = f1;
			mesh.Faces[i].Indexes[1] = f2;
			mesh.Faces[i].Indexes[2] = f3;
		}

		int32_t mcount = reader.Get<int32_t>();
		mesh.MaterialOffsets.resize(mcount);

		for (int32_t i = 0; i < mcount; i++)
		{
			auto index = reader.Get<int32_t>();
			auto facecount = reader.Get<int32_t>();

			mesh.MaterialOffsets[i].MaterialIndex = index;
			mesh.MaterialOffsets[i].FaceOffset = facecount;
		}

		int32_t bcount = reader.Get<int32_t>();
		mesh.BoneConnectors.resize(bcount);
		for (int32_t i = 0; i < bcount; i++)
		{
			auto index = reader.Get<int32_t>();
			auto offsetMatrix = reader.Get<Matrix44>();

			mesh.BoneConnectors[i].TargetIndex = index;
			mesh.BoneConnectors[i].OffsetMatrix = offsetMatrix;
		}
	}

	void Model_IO::LoadDeformer(Deformer& deformer, BinaryReader& reader, const achar* path, int32_t version)
	{
		int32_t bcount = reader.Get<int32_t>();
		deformer.Bones.resize(bcount);

		for (int32_t i = 0; i < bcount; i++)
		{
			auto name = reader.Get<astring>();
			auto parent = reader.Get<int32_t>();
			RotationOrder rotationOrder = reader.Get<RotationOrder>();
			
			deformer.Bones[i].Name = name;
			deformer.Bones[i].ParentBoneIndex = parent;
			deformer.Bones[i].RotationType = rotationOrder;

			if (version >= 2)
			{
				deformer.Bones[i].Translation[0] = reader.Get<float>();
				deformer.Bones[i].Translation[1] = reader.Get<float>();
				deformer.Bones[i].Translation[2] = reader.Get<float>();
			
				deformer.Bones[i].Rotation[0] = reader.Get<float>();
				deformer.Bones[i].Rotation[1] = reader.Get<float>();
				deformer.Bones[i].Rotation[2] = reader.Get<float>();
				deformer.Bones[i].Rotation[3] = reader.Get<float>();

				deformer.Bones[i].Scaling[0] = reader.Get<float>();
				deformer.Bones[i].Scaling[1] = reader.Get<float>();
				deformer.Bones[i].Scaling[2] = reader.Get<float>();

				float rotation[4];
				rotation[0] = deformer.Bones[i].Rotation[0] / 180.0f * PI;
				rotation[1] = deformer.Bones[i].Rotation[1] / 180.0f * PI;
				rotation[2] = deformer.Bones[i].Rotation[2] / 180.0f * PI;
				rotation[3] = deformer.Bones[i].Rotation[3] / 180.0f * PI;

				deformer.Bones[i].LocalMat = ModelUtils::CalcMatrix(
					deformer.Bones[i].Translation,
					rotation,
					deformer.Bones[i].Scaling,
					deformer.Bones[i].RotationType);
			}
			else
			{
				auto localMat = reader.Get<Matrix44>();
				deformer.Bones[i].LocalMat = localMat;

				deformer.Bones[i].Translation[0] = 0;
				deformer.Bones[i].Translation[1] = 0;
				deformer.Bones[i].Translation[2] = 0;

				deformer.Bones[i].Rotation[0] = 0;
				deformer.Bones[i].Rotation[1] = 0;
				deformer.Bones[i].Rotation[2] = 0;
				deformer.Bones[i].Rotation[3] = 0;

				deformer.Bones[i].Scaling[0] = 1.0f;
				deformer.Bones[i].Scaling[1] = 1.0f;
				deformer.Bones[i].Scaling[2] = 1.0f;
			}			
		}
	}

	void Model_IO::LoadMaterials(std::vector<Material>& materials, BinaryReader& reader, const achar* path)
	{
		int32_t materialCount = reader.Get<int32_t>();
		materials.resize(materialCount);

		for (int32_t i = 0; i < materialCount; i++)
		{
			LoadMaterial(materials[i], reader, path);
		}
	}

	void Model_IO::LoadMaterial(Material& material, BinaryReader& reader, const achar* path)
	{
		material.Type = reader.Get<int32_t>();
		if (material.Type == 1)
		{
			auto path = reader.Get<astring>();
		}
		else
		{
			material.OriginalColorTexture = reader.Get<astring>();
			material.OriginalNormalTexture = reader.Get<astring>();
			material.OriginalMetalnessTexture = reader.Get<astring>();

			if (material.OriginalColorTexture != astring()) material.ColorTexture = CombinePath(path, material.OriginalColorTexture.c_str());
			if (material.OriginalNormalTexture != astring()) material.NormalTexture = CombinePath(path, material.OriginalNormalTexture.c_str());
			if (material.OriginalMetalnessTexture != astring()) material.MetalnessTexture = CombinePath(path, material.OriginalMetalnessTexture.c_str());
		}
	}

	void Model_IO::LoadAnimationSource(AnimationSource& as, BinaryReader& reader, const achar* path)
	{
		auto sourceName = reader.Get<astring>();
		auto keyframesCount = reader.Get<int32_t>();

		as.Name = sourceName;
		as.KeyframeAnimations.resize(keyframesCount);

		for (int32_t i = 0; i < keyframesCount; i++)
		{
			LoadKeyframeAnimation(as.KeyframeAnimations[i], reader, path);
		}
	}

	void Model_IO::LoadKeyframeAnimation(KeyframeAnimation& ka, BinaryReader& reader, const achar* path)
	{
		auto name = reader.Get<astring>();
		ka.Name = name;

		auto keyCount = reader.Get<int32_t>();
		ka.Keyframes.resize(keyCount);

		for (int32_t i = 0; i < keyCount; i++)
		{
			ka.Keyframes[i].KeyValue = reader.Get<Vector2DF>();
			ka.Keyframes[i].LeftHandle = reader.Get<Vector2DF>();
			ka.Keyframes[i].RightHandle = reader.Get<Vector2DF>();
			ka.Keyframes[i].Interpolation = (InterpolationType) reader.Get<int32_t>();
		}
	}

	void Model_IO::LoadAnimationClip(AnimationClip& ac, BinaryReader& reader, const achar* path)
	{
		auto clipName = reader.Get<astring>();
		auto sourceIndex = reader.Get<int32_t>();

		ac.Name = clipName;
		ac.Index = sourceIndex;
	}

	void Model_IO::SaveDeformer(Deformer& deformer, BinaryWriter& writer, const achar* path)
	{
		writer.Push((int32_t)deformer.Bones.size());

		for (auto i = 0; i < deformer.Bones.size(); i++)
		{
			auto& bone = deformer.Bones[i];
			writer.Push(deformer.Bones[i].Name);
			writer.Push(deformer.Bones[i].ParentBoneIndex);
			writer.Push((int32_t)deformer.Bones[i].RotationType);
			writer.Push(deformer.Bones[i].LocalMat);
		}
	}

	void Model_IO::SaveMeshes(std::vector<Mesh>& meshes, BinaryWriter& writer, const achar* path)
	{
		writer.Push((int32_t) meshes.size());

		for (auto i = 0; i < meshes.size(); i++)
		{
			SaveMesh(meshes[i], writer, path);
		}
	}

	void Model_IO::SaveMesh(Mesh& mesh, BinaryWriter& writer, const achar* path)
	{
		writer.Push((int32_t)mesh.DividedMeshes.size());

		for (auto i = 0; i < mesh.DividedMeshes.size(); i++)
		{
			SaveDividedMesh(mesh.DividedMeshes[i], writer, path);
		}

		SaveMaterials(mesh.Materials, writer, path);
	}

	void Model_IO::SaveDividedMesh(DividedMesh& mesh, BinaryWriter& writer, const achar* path)
	{
		writer.Push((int32_t) mesh.Vertices.size());

		for (int32_t i = 0; i < mesh.Vertices.size(); i++)
		{
			auto& v = mesh.Vertices[i];
			writer.Push(v.Position);
			writer.Push(v.Normal);
			writer.Push(v.Binormal);
			writer.Push(v.UV1);
			writer.Push(v.UV2);
			writer.Push(v.VColor.R);
			writer.Push(v.VColor.G);
			writer.Push(v.VColor.B);
			writer.Push(v.VColor.A);

			writer.Push(v.BoneWeights[0]);
			writer.Push(v.BoneWeights[1]);
			writer.Push(v.BoneWeights[2]);
			writer.Push(v.BoneWeights[3]);

			writer.Push(v.BoneIndexes[0]);
			writer.Push(v.BoneIndexes[1]);
			writer.Push(v.BoneIndexes[2]);
			writer.Push(v.BoneIndexes[3]);

			writer.Push(v.BoneIndexesOriginal[0]);
			writer.Push(v.BoneIndexesOriginal[1]);
			writer.Push(v.BoneIndexesOriginal[2]);
			writer.Push(v.BoneIndexesOriginal[3]);
		}

		writer.Push((int32_t) mesh.Faces.size());

		for (auto i = 0; i < mesh.Faces.size(); i++)
		{
			auto& f = mesh.Faces[i];
			writer.Push(f.Indexes[0]);
			writer.Push(f.Indexes[1]);
			writer.Push(f.Indexes[2]);
		}

		writer.Push((int32_t) mesh.MaterialOffsets.size());

		for (int32_t i = 0; i < mesh.MaterialOffsets.size(); i++)
		{
			writer.Push(mesh.MaterialOffsets[i].MaterialIndex);
			writer.Push(mesh.MaterialOffsets[i].FaceOffset);
		}

		writer.Push((int32_t) mesh.BoneConnectors.size());

		for (int32_t i = 0; i < mesh.BoneConnectors.size(); i++)
		{
			writer.Push(mesh.BoneConnectors[i].TargetIndex);
			writer.Push(mesh.BoneConnectors[i].OffsetMatrix);
		}
	}

	void Model_IO::SaveMaterials(std::vector<Material>& materials, BinaryWriter& writer, const achar* path)
	{
		writer.Push((int32_t) materials.size());

		for (int32_t i = 0; i < materials.size(); i++)
		{
			SaveMaterial(materials[i], writer, path);
		}
	}

	void Model_IO::SaveMaterial(Material& material, BinaryWriter& writer, const achar* path)
	{
		if (material.Type == 1)
		{
			writer.Push((int32_t)1);
			astring path;
			writer.Push(path);
		}
		else
		{
			writer.Push((int32_t) 0);
			writer.Push(material.OriginalColorTexture.c_str());
			writer.Push(material.OriginalNormalTexture.c_str());
			writer.Push(material.OriginalMetalnessTexture.c_str());
		}
	}

	void Model_IO::SaveAnimationSource(AnimationSource& as, BinaryWriter& writer, const achar* path)
	{
		writer.Push(as.Name);
		writer.Push((int32_t) as.KeyframeAnimations.size());

		for (int32_t i = 0; i < as.KeyframeAnimations.size(); i++)
		{
			SaveKeyframeAnimation(as.KeyframeAnimations[i], writer, path);
		}
	}

	void Model_IO::SaveKeyframeAnimation(KeyframeAnimation& ka, BinaryWriter& writer, const achar* path)
	{
		writer.Push(ka.Name);
		writer.Push((int32_t) ka.Keyframes.size());

		for (int32_t i = 0; i < ka.Keyframes.size(); i++)
		{
			writer.Push(ka.Keyframes[i].KeyValue);
			writer.Push(ka.Keyframes[i].LeftHandle);
			writer.Push(ka.Keyframes[i].RightHandle);
			writer.Push((int32_t)ka.Keyframes[i].Interpolation);
		}
	}

	void Model_IO::SaveAnimationClip(AnimationClip& ac, BinaryWriter& writer, const achar* path)
	{
		writer.Push(ac.Name);
		writer.Push(ac.Index);
	}

	void ModelUtils::CalculateBoneMatrixes(std::vector<Matrix44>& dst, const std::vector<Model_IO::Bone>& bones, const std::vector<Matrix44>& localMatrixes)
	{
		// 計算
		for (auto i = 0; i < bones.size(); i++)
		{
			auto& b = bones[i];

			dst[i] = localMatrixes[i];
	
			if (b.ParentBoneIndex >= 0)
			{
				Matrix44::Mul(dst[i], dst[b.ParentBoneIndex], dst[i]);
			}
		}
	}

	Matrix44 ModelUtils::CalcRotationMatrix(float rotation[4], RotationOrder rotationType)
	{
		if (rotationType == RotationOrder::QUATERNION)
		{
			Matrix44 matR;
			matR.SetQuaternion(rotation[0], rotation[1], rotation[2], rotation[3]);
			return matR;
		}
		else if (rotationType == RotationOrder::AXIS)
		{
			Matrix44 matR;
			matR.SetRotationAxis(Vector3DF(rotation[0], rotation[2], -rotation[1]), rotation[3]);
			return matR;
		}
		else
		{
			Matrix44 mat, matRx, matRy, matRz;
			matRx.SetRotationX(rotation[0]);
			matRy.SetRotationY(rotation[1]);
			matRz.SetRotationZ(rotation[2]);

			if (rotationType == RotationOrder::XZY)
			{
				mat = Matrix44::Mul(mat, matRz, matRx);
				mat = Matrix44::Mul(mat, matRy, mat);
			}

			if (rotationType == RotationOrder::XYZ)
			{
				mat = Matrix44::Mul(mat, matRy, matRx);
				mat = Matrix44::Mul(mat, matRz, mat);
			}

			if (rotationType == RotationOrder::ZXY)
			{
				mat = Matrix44::Mul(mat, matRx, matRz);
				mat = Matrix44::Mul(mat, matRy, mat);
			}

			if (rotationType == RotationOrder::ZYX)
			{
				mat = Matrix44::Mul(mat, matRy, matRz);
				mat = Matrix44::Mul(mat, matRz, mat);
			}

			if (rotationType == RotationOrder::YXZ)
			{
				mat = Matrix44::Mul(mat, matRx, matRy);
				mat = Matrix44::Mul(mat, matRz, mat);
			}

			if (rotationType == RotationOrder::YZX)
			{
				mat = Matrix44::Mul(mat, matRz, matRy);
				mat = Matrix44::Mul(mat, matRx, mat);
			}

			return mat;
		}

		return Matrix44();
	}

	Matrix44 ModelUtils::CalcMatrix(float position[3], float rotation [4], float scale[3], RotationOrder rotationType)
	{
		auto matR = CalcRotationMatrix(rotation, rotationType);
		return CalcMatrix(position, matR, scale);
	}

	Matrix44 ModelUtils::CalcMatrix(float position[3], const Matrix44& matR, float scale[3])
	{
		Matrix44 mat, matS, matT;
		matS.SetScale(scale[0], scale[1], scale[2]);
		matT.SetTranslation(position[0], position[1], position[2]);

		mat = Matrix44::Mul(mat, matR, matS);
		mat = Matrix44::Mul(mat, matT, mat);

		return mat;
	}

	float ModelUtils::GetKeyframeValue(float time, const std::vector<FCurveKeyframe>& keyframes)
	{
		if (keyframes.size() == 0) return 0;

		if (keyframes[0].KeyValue.X > time) return keyframes[0].KeyValue.Y;

		if (keyframes[keyframes.size() - 1].KeyValue.X <= time) return keyframes[keyframes.size() - 1].KeyValue.Y;

		int32_t left = 0;
		int32_t right = keyframes.size() - 1;
		int32_t middle = (left + right) / 2;

		while (true)
		{
			if (keyframes[left].KeyValue.X <= time && time < keyframes[middle].KeyValue.X)
			{
				right = middle;
				middle = (left + right) / 2;
			}
			else
			{
				left = middle;
				middle = (left + right) / 2;
			}

			if (left == middle) break;
		}

		if (keyframes[left].KeyValue.X <= time && time < keyframes[left + 1].KeyValue.X)
		{
			if (keyframes[left].Interpolation == InterpolationType::Constant)
			{
				return keyframes[left].KeyValue.Y;
			}
			else if (keyframes[left].Interpolation == InterpolationType::Linear)
			{
				auto d = time - keyframes[left].KeyValue.X;
				auto dx = keyframes[left + 1].KeyValue.X - keyframes[left].KeyValue.X;
				auto dy = keyframes[left + 1].KeyValue.Y - keyframes[left].KeyValue.Y;

				return keyframes[left].KeyValue.Y + dy / dx * d;
			}
			else if (keyframes[left].Interpolation == InterpolationType::Cubic)
			{
				float k1[2];
				float k1rh[2];
				float k2lh[2];
				float k2[2];

				k1[0] = keyframes[left].KeyValue.X;
				k1[1] = keyframes[left].KeyValue.Y;

				k1rh[0] = keyframes[left].RightHandle.X;
				k1rh[1] = keyframes[left].RightHandle.Y;

				k2lh[0] = keyframes[left + 1].LeftHandle.X;
				k2lh[1] = keyframes[left + 1].LeftHandle.Y;

				k2[0] = keyframes[left + 1].KeyValue.X;
				k2[1] = keyframes[left + 1].KeyValue.Y;

				NormalizeValues(k1, k1rh, k2lh, k2);
				float t = 0;
				auto getT = CalcT(time, k1[0], k1rh[0], k2lh[0], k2[0], t);
				if (getT)
				{
					return CalcBezierValue(k1[1], k1rh[1], k2lh[1], k2[1], t);
				}
				else
				{
					return 0;
				}
			}
			return 0;
		}

		/*
		for (int32_t i = 0; i < keyframes.size() - 1; i++)
		{
			if (keyframes[i].KeyValue.X <= time && time < keyframes[i + 1].KeyValue.X)
			{
				if (keyframes[i].InterpolationType == eInterpolationType::INTERPOLATION_TYPE_CONSTANT)
				{
					return keyframes[i].KeyValue.Y;
				}
				else if (keyframes[i].InterpolationType == eInterpolationType::INTERPOLATION_TYPE_LINEAR)
				{
					auto d = time - keyframes[i].KeyValue.X;
					auto dx = keyframes[i + 1].KeyValue.X - keyframes[i].KeyValue.X;
					auto dy = keyframes[i + 1].KeyValue.Y - keyframes[i].KeyValue.Y;

					return keyframes[i].KeyValue.Y + dy / dx * d;
				}
				else if (keyframes[i].InterpolationType == eInterpolationType::INTERPOLATION_TYPE_CUBIC)
				{
					float k1[2];
					float k1rh[2];
					float k2lh[2];
					float k2[2];

					k1[0] = keyframes[i].KeyValue.X;
					k1[1] = keyframes[i].KeyValue.Y;

					k1rh[0] = keyframes[i].RightHandle.X;
					k1rh[1] = keyframes[i].RightHandle.Y;

					k2lh[0] = keyframes[i + 1].LeftHandle.X;
					k2lh[1] = keyframes[i + 1].LeftHandle.Y;

					k2[0] = keyframes[i + 1].KeyValue.X;
					k2[1] = keyframes[i + 1].KeyValue.Y;

					NormalizeValues(k1, k1rh, k2lh, k2);
					float t = 0;
					auto getT = CalcT(time, k1[0], k1rh[0], k2lh[0], k2[0], t);
					if (getT)
					{
						return CalcBezierValue(k1[1], k1rh[1], k2lh[1], k2[1], t);
					}
					else
					{
						return 0;
					}
				}
				return 0;
			}
		}
		*/

		return 0;
	}

	bool ModelUtils::GetAnimationTarget(astring& targetName, AnimationCurveTargetType& targetType, AnimationCurveTargetAxis& targetAxis, const astring& name)
	{
		auto strs = split(astring(name), ToAString("."));

		// ボーン向け設定の取得
		targetName = astring();
		targetType = AnimationCurveTargetType::NoneTarget;
		targetAxis = AnimationCurveTargetAxis::NoneTarget;

		if (strs.size() < 3) return false;

		for (size_t i = 0; i < strs.size() - 2; i++)
		{
			targetName += strs[i];

			if (i != strs.size() - 3)
			{
				targetName += ToAString(".");
			}
		}

		if (strs[strs.size() - 2] == ToAString("pos"))
		{
			targetType = AnimationCurveTargetType::Position;
		}

		if (strs[strs.size() - 2] == ToAString("rot"))
		{
			targetType = AnimationCurveTargetType::Rotation;
		}

		if (strs[strs.size() - 2] == ToAString("scl"))
		{
			targetType = AnimationCurveTargetType::Scale;
		}

		if (strs[strs.size() - 1] == ToAString("x"))
		{
			targetAxis = AnimationCurveTargetAxis::X;
		}

		if (strs[strs.size() - 1] == ToAString("y"))
		{
			targetAxis = AnimationCurveTargetAxis::Y;
		}

		if (strs[strs.size() - 1] == ToAString("z"))
		{
			targetAxis = AnimationCurveTargetAxis::Z;
		}

		if (strs[strs.size() - 1] == ToAString("w"))
		{
			targetAxis = AnimationCurveTargetAxis::W;
		}

		return true;
	}

	void ModelUtils::SetBoneValue(
		float position[3],
		float rotation[4],
		float scale[3],
		AnimationCurveTargetType targetType,
		AnimationCurveTargetAxis targetAxis,
		float value)
	{
		if (targetType == AnimationCurveTargetType::NoneTarget) return;
		if (targetAxis == AnimationCurveTargetAxis::NoneTarget) return;
		
		if (targetType == AnimationCurveTargetType::Position)
		{
			position[(int32_t)targetAxis] = value;
		}
		else if (targetType == AnimationCurveTargetType::Rotation)
		{
			rotation[(int32_t) targetAxis] = value / 180.0f * 3.141592f;
		}
		else if (targetType == AnimationCurveTargetType::Scale)
		{
			scale[(int32_t) targetAxis] = value;
		}
	}
}