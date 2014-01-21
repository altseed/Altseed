
#include "ace.Model_Imp.h"

#include "ace.Mesh_Imp.h"
#include "ace.Deformer_Imp.h"

#include "ace.RenderedModelObject3D.h"

#include "../Animation/ace.AnimationClip_Imp.h"
#include "../Animation/ace.AnimationSource_Imp.h"
#include "../Animation/ace.KeyframeAnimation_Imp.h"

#include "../ace.Graphics_Imp.h"
#include "../ace.GraphicsResourceContainer.h"

namespace ace
{
	void Model_Imp::Reset()
	{
		for (auto& m : m_meshes)
		{
			m->Release();
		}

		for (auto& a : m_animationClips)
		{
			a->Release();
		}
	}

	Model_Imp::Model_Imp(Graphics* graphics)
		: m_graphics(graphics)
	{
		SafeAddRef(graphics);
	}

	Model_Imp::~Model_Imp()
	{
		Reset();

		auto g = (Graphics_Imp*) m_graphics;
		g->GetResourceContainer()->Models.Unregist(this);

		SafeRelease(m_graphics);
	}

	bool Model_Imp::Load(Graphics* g, std::vector<uint8_t>&	data, const achar* path)
	{
		Reset();

		BinaryReader reader;
		reader.ReadIn(data.begin(), data.end());

		// ヘッダーチェック
		uint8_t header_true[] = "MDL";
		for (int32_t i = 0; i < 4; i++)
		{
			auto h = reader.Get<uint8_t>();
			if (header_true[i] != h) return false;
		}
		
		// バージョン
		int32_t version = reader.Get<int32_t>();

		// メッシュグループ
		int32_t meshGroupCount = reader.Get<int32_t>();
		for (int32_t i = 0; i < meshGroupCount; i++)
		{
			auto mesh = LoadMeshGroup(g, reader, path);
			m_meshes.push_back(mesh);
		}

		// アニメーション
		std::vector<AnimationSource*> sources;
		int32_t sourceCount = reader.Get<int32_t>();
		for (int32_t i = 0; i < sourceCount; i++)
		{
			auto source = LoadAnimationSource(reader);
			sources.push_back(source);
		}

		int32_t clipCount = reader.Get<int32_t>();
		for (int32_t i = 0; i < clipCount; i++)
		{
			LoadAnimationClip(reader, sources);
		}

		for (auto& s : sources)
		{
			s->Release();
		}

		return true;
	}

	void Model_Imp::Attach(RenderedModelObject3D* model)
	{
		assert(m_observers.find(model) != m_observers.end());

		m_observers.insert(model);
	}

	void Model_Imp::Detach(RenderedModelObject3D* model)
	{
		assert(m_observers.find(model) == m_observers.end());

		m_observers.erase(model);
	}

	void Model_Imp::Reload(std::vector<uint8_t>& data, const achar* path)
	{
		Load(m_graphics, data, path);

		for (auto& o : m_observers)
		{
			o->ReloadModel();
		}
	}

	Mesh_Imp* Model_Imp::LoadMeshGroup(Graphics* g, BinaryReader& reader, const achar* path)
	{
		auto mesh = LoadMesh(g, reader, path);
		auto deformer = LoadDeformer(g, reader, path);
		mesh->SetDeformer(deformer);

		// 材質
		int32_t materialCount = reader.Get<int32_t>();
		for (int32_t i = 0; i < materialCount; i++)
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

				if (pathColor != ace::ToAString(""))
				{
					mesh->SetColorTexture(i, g->CreateTexture2D(pathColor.c_str()).get());
				}

				if (pathNormal != ace::ToAString(""))
				{
					mesh->SetNormalTexture(i, g->CreateTexture2D(pathNormal.c_str()).get());
				}

				if (pathSpecular != ace::ToAString(""))
				{
					mesh->SetSpecularTexture(i, g->CreateTexture2D(pathSpecular.c_str()).get());
				}
			}
		}

		return mesh;
	}

	Mesh_Imp* Model_Imp::LoadMesh(Graphics* g, BinaryReader& reader, const achar* path)
	{
		auto g_ = (Graphics_Imp*) g;
		auto mesh = (Mesh_Imp*)g_->CreateMesh_();

		int32_t vcount = reader.Get<int32_t>();
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

			mesh->AddInternalVertex(pos, normal, binormal, uv, subuv, color, weights, indexes, indexesOriginal);
		}

		int32_t fcount = reader.Get<int32_t>();
		for (int32_t i = 0; i < fcount; i++)
		{
			auto f1 = reader.Get<int32_t>();
			auto f2 = reader.Get<int32_t>();
			auto f3 = reader.Get<int32_t>();

			mesh->AddFace(f1, f2, f3);
		}

		int32_t mcount = reader.Get<int32_t>();
		for (int32_t i = 0; i < mcount; i++)
		{
			auto index = reader.Get<int32_t>();
			auto facecount = reader.Get<int32_t>();
		
			mesh->AddMaterialCount(index, facecount);
		}

		int32_t bcount = reader.Get<int32_t>();
		for (int32_t i = 0; i < bcount; i++)
		{
			uint8_t offset[32];
			for (int32_t i = 0; i < 32; i++) offset[i] = reader.Get<uint8_t>();
			auto facecount = reader.Get<int32_t>();

			mesh->AddInternalBoneOffset(offset, facecount);
		}

		mesh->SendToGPUMemory();

		return mesh;
	}

	Deformer_Imp* Model_Imp::LoadDeformer(Graphics* g, BinaryReader& reader, const achar* path)
	{
		auto g_ = (Graphics_Imp*) g;
		auto deformer = (Deformer_Imp*) g_->CreateDeformer_();

		int32_t bcount = reader.Get<int32_t>();
		for (int32_t i = 0; i < bcount; i++)
		{
			auto name = reader.Get<ace::astring>();
			auto parent = reader.Get<int32_t>();
			eRotationOrder rotationOrder = reader.Get<eRotationOrder>();
			auto localMat = reader.Get<Matrix44>();
			auto matInv = reader.Get<Matrix44>();

			deformer->AddBone(name.c_str(), parent, rotationOrder, localMat, matInv);
		}

		return deformer;
	}

	void Model_Imp::LoadAnimationClip(BinaryReader& reader, std::vector<AnimationSource*>& sources)
	{
		auto clip = new AnimationClip_Imp();

		auto clipName = reader.Get<ace::astring>();
		auto sourceIndex = reader.Get<int32_t>();

		clip->SetSource(sources[sourceIndex]);

		m_animationClips.push_back(clip);
		m_animationClipNames.push_back(clipName);
	}

	AnimationSource* Model_Imp::LoadAnimationSource(BinaryReader& reader)
	{
		auto source = new AnimationSource_Imp();

		auto sourceName = reader.Get<ace::astring>();

		auto keyframesCount = reader.Get<int32_t>();
		for (int32_t i = 0; i < keyframesCount; i++)
		{
			auto keyframeanimation = LoadKeyframeAnimation(reader);
			source->AddAnimation(keyframeanimation);
		}

		return source;
	}

	KeyframeAnimation* Model_Imp::LoadKeyframeAnimation(BinaryReader& reader)
	{
		auto keyframeAnimation = new KeyframeAnimation_Imp();

		auto name = reader.Get<ace::astring>();
		keyframeAnimation->SetName(name.c_str());
		
		auto keyCount = reader.Get<int32_t>();

		for (int32_t i = 0; i < keyCount; i++)
		{
			auto keyframe = reader.Get< FCurveKeyframe>();
			keyframeAnimation->AddKeyframe(keyframe);
		}

		return keyframeAnimation;
	}
}