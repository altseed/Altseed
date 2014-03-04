
#include "ace.Model_IO.h"

namespace ace
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
			return (v >= small_) && (v <= big_);
		};

		auto c3_ = k2X - k1X + 3.0f * (k1rhX - k2lhX);
		auto c2_ = 3.0f * (k1X - 2.0f * k1rhX + k2lhX);
		auto c1_ = 3.0f * (k1rhX - k1X);
		auto c0_ = k1X - frame;

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


	bool Model_IO::Load(std::vector<uint8_t>&	data, const achar* path)
	{
		MeshGroups.clear();

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

		// メッシュグループ
		int32_t meshGroupCount = reader.Get<int32_t>();
		MeshGroups.resize(meshGroupCount);
		for (int32_t i = 0; i < meshGroupCount; i++)
		{
			LoadMeshGroup(&(MeshGroups[i]), reader, path);
		}

		// アニメーション
		int32_t sourceCount = reader.Get<int32_t>();
		AnimationSources.resize(sourceCount);
		for (int32_t i = 0; i < sourceCount; i++)
		{
			LoadAnimationSource(&(AnimationSources[i]), reader, path);
		}

		int32_t clipCount = reader.Get<int32_t>();
		AnimationClips.resize(clipCount);
		for (int32_t i = 0; i < clipCount; i++)
		{
			LoadAnimationClip(&(AnimationClips[i]), reader, path);
		}

		return true;
	}

	void Model_IO::LoadMeshGroup(MeshGroup* meshGroup, BinaryReader& reader, const achar* path)
	{
		LoadMesh(&(meshGroup->Mesh), reader, path);
		LoadDeformer(&(meshGroup->Deformer), reader, path);
		LoadMaterials(&(meshGroup->Materials), reader, path);
	}

	void Model_IO::LoadMesh(Mesh* mesh, BinaryReader& reader, const achar* path)
	{
		int32_t vcount = reader.Get<int32_t>();
		mesh->Vertices.resize(vcount);

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

			mesh->Vertices[i].Position = pos;
			mesh->Vertices[i].Normal = normal;
			mesh->Vertices[i].Binormal = binormal;
			mesh->Vertices[i].UV1 = uv;
			mesh->Vertices[i].UV2 = subuv;
			mesh->Vertices[i].VColor = color;
			mesh->Vertices[i].BoneWeights = weights;
			mesh->Vertices[i].BoneIndexes = indexes;
			mesh->Vertices[i].BoneIndexesOriginal = indexesOriginal;
		}

		int32_t fcount = reader.Get<int32_t>();
		mesh->Faces.resize(fcount);

		for (int32_t i = 0; i < fcount; i++)
		{
			auto f1 = reader.Get<int32_t>();
			auto f2 = reader.Get<int32_t>();
			auto f3 = reader.Get<int32_t>();

			mesh->Faces[i].Index1 = f1;
			mesh->Faces[i].Index2 = f2;
			mesh->Faces[i].Index3 = f3;
		}

		int32_t mcount = reader.Get<int32_t>();
		mesh->MaterialOffsets.resize(mcount);

		for (int32_t i = 0; i < mcount; i++)
		{
			auto index = reader.Get<int32_t>();
			auto facecount = reader.Get<int32_t>();

			mesh->MaterialOffsets[i].MaterialIndex = index;
			mesh->MaterialOffsets[i].FaceOffset = facecount;
		}

		int32_t bcount = reader.Get<int32_t>();
		mesh->BoneOffsets.resize(bcount);

		for (int32_t i = 0; i < bcount; i++)
		{
			uint8_t offset[32];
			for (int32_t j = 0; j < 32; j++) offset[j] = reader.Get<uint8_t>();
			auto facecount = reader.Get<int32_t>();

			mesh->BoneOffsets[i].FaceOffset = facecount;
			for (int32_t j = 0; j < 32; j++) mesh->BoneOffsets[i].BoneIndex[j] = offset[i];
		}
	}

	void Model_IO::LoadDeformer(Deformer* deformer, BinaryReader& reader, const achar* path)
	{
		int32_t bcount = reader.Get<int32_t>();
		deformer->Bones.resize(bcount);

		for (int32_t i = 0; i < bcount; i++)
		{
			auto name = reader.Get<ace::astring>();
			auto parent = reader.Get<int32_t>();
			eRotationOrder rotationOrder = reader.Get<eRotationOrder>();
			auto localMat = reader.Get<Matrix44>();
			auto matInv = reader.Get<Matrix44>();

			deformer->Bones[i].Name = name;
			deformer->Bones[i].ParentBoneIndex = parent;
			deformer->Bones[i].RotationType = rotationOrder;
			deformer->Bones[i].LocalMat = localMat;
			deformer->Bones[i].GlobalMatInv = matInv;
		}
	}

	void Model_IO::LoadMaterials(std::vector<Material>* materials, BinaryReader& reader, const achar* path)
	{
		int32_t materialCount = reader.Get<int32_t>();
		materials->resize(materialCount);

		for (int32_t i = 0; i < materialCount; i++)
		{
			LoadMaterial(&(materials->at(i)), reader, path);
		}
	}

	void Model_IO::LoadMaterial(Material* material, BinaryReader& reader, const achar* path)
	{
		auto type = reader.Get<int32_t>();
		if (type == 1)
		{
			auto path = reader.Get<ace::astring>();
		}
		else
		{
			auto pathColor = reader.Get<ace::astring>();
			auto pathNormal = reader.Get<ace::astring>();
			auto pathSpecular = reader.Get<ace::astring>();

			material->ColorTexture = CombinePath(path, pathColor.c_str());
			material->NormalTexture = CombinePath(path, pathNormal.c_str());
			material->SpecularTexture = CombinePath(path, pathSpecular.c_str());
		}
	}

	void Model_IO::LoadAnimationSource(AnimationSource* as, BinaryReader& reader, const achar* path)
	{
		auto sourceName = reader.Get<ace::astring>();
		auto keyframesCount = reader.Get<int32_t>();

		as->Name = sourceName;
		as->KeyframeAnimations.resize(keyframesCount);

		for (int32_t i = 0; i < keyframesCount; i++)
		{
			LoadKeyframeAnimation(&(as->KeyframeAnimations[i]), reader, path);
		}
	}

	void Model_IO::LoadKeyframeAnimation(KeyframeAnimation* ka, BinaryReader& reader, const achar* path)
	{
		auto name = reader.Get<ace::astring>();
		ka->Name = name;

		auto keyCount = reader.Get<int32_t>();
		ka->Keyframes.resize(keyCount);

		for (int32_t i = 0; i < keyCount; i++)
		{
			ka->Keyframes[i].KeyValue = reader.Get<Vector2DF>();
			ka->Keyframes[i].LeftHandle = reader.Get<Vector2DF>();
			ka->Keyframes[i].RightHandle = reader.Get<Vector2DF>();
			ka->Keyframes[i].InterpolationType = (eInterpolationType) reader.Get<int32_t>();
		}
	}

	void Model_IO::LoadAnimationClip(AnimationClip* ac, BinaryReader& reader, const achar* path)
	{
		auto clipName = reader.Get<ace::astring>();
		auto sourceIndex = reader.Get<int32_t>();

		ac->Name = clipName;
		ac->Index = sourceIndex;
	}

	void ModelUtils::CalculateBoneMatrixes(std::vector<Matrix44>& dst, const std::vector<Model_IO::Bone>& bones, const std::vector<Matrix44>& localMatrixes)
	{
		// 計算
		for (auto i = 0; i < bones.size(); i++)
		{
			auto& b = bones[i];

			// ローカル行列の計算
			dst[i] = localMatrixes[i];

			Matrix44::Mul(dst[i], b.LocalMat, dst[i]);

			if (b.ParentBoneIndex >= 0)
			{
				Matrix44::Mul(dst[i], dst[b.ParentBoneIndex], dst[i]);
			}
		}

		for (auto i = 0; i < bones.size(); i++)
		{
			auto& b = bones[i];
			Matrix44::Mul(dst[i], b.GlobalMatInv, dst[i]);
		}
	}

	Matrix44 ModelUtils::CalcMatrix(float position[3], float rotation [4], float scale[3], eRotationOrder rotationType)
	{
		if (rotationType == ROTATION_ORDER_QUATERNION)
		{
			Matrix44 mat, matS, matR, matT;
			matS.SetScale(scale[0], scale[1], scale[2]);
			matT.SetTranslation(position[0], position[1], position[2]);
			matR.SetQuaternion(rotation[0], rotation[1], rotation[2], rotation[3]);

			mat = Matrix44::Mul(mat, matR, matS);
			mat = Matrix44::Mul(mat, matT, mat);

			return mat;
		}
		else if (rotationType == ROTATION_ORDER_AXIS)
		{
			Matrix44 mat, matS, matR, matT;
			matS.SetScale(scale[0], scale[1], scale[2]);
			matT.SetTranslation(position[0], position[1], position[2]);
			matR.SetRotationAxis(Vector3DF(rotation[0], rotation[2], -rotation[1]), rotation[3]);

			mat = Matrix44::Mul(mat, matR, matS);
			mat = Matrix44::Mul(mat, matT, mat);

			return mat;
		}
		else
		{
			Matrix44 mat, matS, matRx, matRy, matRz, matT;
			matS.SetScale(scale[0], scale[1], scale[2]);
			matT.SetTranslation(position[0], position[1], position[2]);
			matRx.SetRotationX(rotation[0]);
			matRy.SetRotationY(rotation[1]);
			matRz.SetRotationZ(rotation[2]);

			if (rotationType == ROTATION_ORDER_XZY)
			{
				mat = Matrix44::Mul(mat, matRx, matS);
				mat = Matrix44::Mul(mat, matRz, mat);
				mat = Matrix44::Mul(mat, matRy, mat);
				mat = Matrix44::Mul(mat, matT, mat);
			}

			if (rotationType == ROTATION_ORDER_XYZ)
			{
				mat = Matrix44::Mul(mat, matRx, matS);
				mat = Matrix44::Mul(mat, matRy, mat);
				mat = Matrix44::Mul(mat, matRz, mat);
				mat = Matrix44::Mul(mat, matT, mat);
			}

			if (rotationType == ROTATION_ORDER_ZXY)
			{
				mat = Matrix44::Mul(mat, matRz, matS);
				mat = Matrix44::Mul(mat, matRx, mat);
				mat = Matrix44::Mul(mat, matRy, mat);
				mat = Matrix44::Mul(mat, matT, mat);
			}

			if (rotationType == ROTATION_ORDER_ZYX)
			{
				mat = Matrix44::Mul(mat, matRz, matS);
				mat = Matrix44::Mul(mat, matRy, mat);
				mat = Matrix44::Mul(mat, matRz, mat);
				mat = Matrix44::Mul(mat, matT, mat);
			}

			if (rotationType == ROTATION_ORDER_YXZ)
			{
				mat = Matrix44::Mul(mat, matRy, matS);
				mat = Matrix44::Mul(mat, matRx, mat);
				mat = Matrix44::Mul(mat, matRz, mat);
				mat = Matrix44::Mul(mat, matT, mat);
			}

			if (rotationType == ROTATION_ORDER_YZX)
			{
				mat = Matrix44::Mul(mat, matRy, matS);
				mat = Matrix44::Mul(mat, matRz, mat);
				mat = Matrix44::Mul(mat, matRx, mat);
				mat = Matrix44::Mul(mat, matT, mat);
			}

			return mat;
		}

		return Matrix44();
	}


	float ModelUtils::GetKeyframeValue(float time, const std::vector<FCurveKeyframe>& keyframes)
	{
		if (keyframes.size() == 0) return 0;

		if (keyframes[0].KeyValue.X > time) return keyframes[0].KeyValue.Y;

		if (keyframes[keyframes.size() - 1].KeyValue.X <= time) return keyframes[keyframes.size() - 1].KeyValue.Y;

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

		return 0;
	}

	bool ModelUtils::GetAnimationTarget(astring& targetName, eAnimationCurveTargetType& targetType, eAnimationCurveTargetAxis& targetAxis, const astring& name)
	{
		auto strs = split(astring(name), ToAString("."));

		// ボーン向け設定の取得
		targetName = astring();
		targetType = ANIMATION_CURVE_TARGET_TYPE_NONE;
		targetAxis = ANIMATION_CURVE_TARGET_AXIS_NONE;

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
			targetType = ANIMATION_CURVE_TARGET_TYPE_POSITON;
		}

		if (strs[strs.size() - 2] == ToAString("rot"))
		{
			targetType = ANIMATION_CURVE_TARGET_TYPE_ROTATION;
		}

		if (strs[strs.size() - 2] == ToAString("scl"))
		{
			targetType = ANIMATION_CURVE_TARGET_TYPE_SCALE;
		}

		if (strs[strs.size() - 1] == ToAString("x"))
		{
			targetAxis = ANIMATION_CURVE_TARGET_AXIS_X;
		}

		if (strs[strs.size() - 1] == ToAString("y"))
		{
			targetAxis = ANIMATION_CURVE_TARGET_AXIS_Y;
		}

		if (strs[strs.size() - 1] == ToAString("z"))
		{
			targetAxis = ANIMATION_CURVE_TARGET_AXIS_Z;
		}

		if (strs[strs.size() - 1] == ToAString("w"))
		{
			targetAxis = ANIMATION_CURVE_TARGET_AXIS_W;
		}

		return true;
	}

	void ModelUtils::SetBoneValue(
		float position[3],
		float rotation[4],
		float scale[3],
		eAnimationCurveTargetType targetType,
		eAnimationCurveTargetAxis targetAxis,
		float value)
	{
		if (targetType == eAnimationCurveTargetType::ANIMATION_CURVE_TARGET_TYPE_NONE) return;
		if (targetAxis == eAnimationCurveTargetAxis::ANIMATION_CURVE_TARGET_AXIS_NONE) return;
		
		if (targetType == eAnimationCurveTargetType::ANIMATION_CURVE_TARGET_TYPE_POSITON)
		{
			position[targetAxis] = value;
		}
		else if (targetType == eAnimationCurveTargetType::ANIMATION_CURVE_TARGET_TYPE_ROTATION)
		{
			rotation[targetAxis] = value / 180.0f * 3.141592f;
		}
		else if (targetType == eAnimationCurveTargetType::ANIMATION_CURVE_TARGET_TYPE_SCALE)
		{
			scale[targetAxis] = value;
		}
	}
}