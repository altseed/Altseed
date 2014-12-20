
#include "ace.Model_Imp.h"

#include "ace.Mesh_Imp.h"
#include "ace.Deformer_Imp.h"

#include "../Object/ace.RenderedModelObject3D.h"

#include "Animation/ace.AnimationClip_Imp.h"
#include "Animation/ace.AnimationSource_Imp.h"
#include "Animation/ace.KeyframeAnimation_Imp.h"

#include "../../ace.Graphics_Imp.h"
#include "../../ace.GraphicsResourceContainer.h"

namespace ace
{
	Model_Imp::MeshGroup::~MeshGroup()
	{
		for (auto& m : Meshes)
		{
			m->Release();
		}
		SafeRelease(Deformer_);
	}

	void Model_Imp::Reset()
	{
		SafeRelease(m_meshGroup);

		for (auto& a : m_animationClips)
		{
			a->Release();
		}
		m_animationClips.clear();
	}

	Model_Imp::Model_Imp(Graphics* graphics)
		: m_graphics(graphics)
		, m_meshGroup(nullptr)
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

		Model_IO io;
		if (!io.Load(data, path)) return false;

		// メッシュグループ
		m_meshGroup = LoadMeshGroup(g, io, path);

		// アニメーション
		std::vector<AnimationSource*> sources;
		for (int32_t i = 0; i < io.AnimationSources.size(); i++)
		{
			auto source = LoadAnimationSource(io.AnimationSources[i]);
			sources.push_back(source);
		}

		for (auto i = 0; i < io.AnimationClips.size(); i++)
		{
			LoadAnimationClip(io.AnimationClips[i], sources);
		}

		for (auto& s : sources)
		{
			s->Release();
		}

		return true;
	}

	void Model_Imp::Attach(RenderedModelObject3D* model)
	{
		assert(m_observers.find(model) == m_observers.end());

		m_observers.insert(model);
	}

	void Model_Imp::Detach(RenderedModelObject3D* model)
	{
		assert(m_observers.find(model) != m_observers.end());

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

	Model_Imp::MeshGroup* Model_Imp::LoadMeshGroup(Graphics* g, Model_IO& io, const achar* path)
	{
		MeshGroup* mg = new MeshGroup();

		for (auto i = 0; i < io.Meshes.size(); i++)
		{
			auto mesh = LoadMesh(g, io.Meshes[i], path);
			mg->Meshes.push_back(mesh);
		}

		auto deformer = LoadDeformer(g, io, path);
		mg->Deformer_ = deformer;

		return mg;
	}

	Mesh_Imp* Model_Imp::LoadMesh(Graphics* g, Model_IO::Mesh& io, const achar* path)
	{
		auto g_ = (Graphics_Imp*) g;
		auto mesh = (Mesh_Imp*)g_->CreateMesh_();

		for (auto& dm_s : io.DividedMeshes)
		{
			std::vector<Mesh_Imp::Vertex> vertexes;
			std::vector<Mesh_Imp::Face> faces;
			std::vector<Mesh_Imp::BoneConnector> boneConnectors;
			std::vector<Mesh_Imp::MaterialOffset> materialOffsets;

			for (auto i = 0; i < dm_s.Vertices.size(); i++)
			{
				Mesh_Imp::Vertex t;
				t.Position = dm_s.Vertices[i].Position;
				t.Normal = dm_s.Vertices[i].Normal;
				t.Binormal = dm_s.Vertices[i].Binormal;
				t.UV1 = dm_s.Vertices[i].UV1;
				t.UV2 = dm_s.Vertices[i].UV2;
				t.VColor = dm_s.Vertices[i].VColor;

				memcpy(&t.BoneWeights, dm_s.Vertices[i].BoneWeights, sizeof(int32_t));
				memcpy(&t.BoneIndexes, dm_s.Vertices[i].BoneIndexes, sizeof(int32_t));
				memcpy(&t.BoneIndexesOriginal, dm_s.Vertices[i].BoneIndexesOriginal, sizeof(int32_t));

				vertexes.push_back(t);
			}

			for (auto i = 0; i < dm_s.Faces.size(); i++)
			{
				Mesh_Imp::Face t;
				t.Index1 = dm_s.Faces[i].Indexes[0];
				t.Index2 = dm_s.Faces[i].Indexes[1];
				t.Index3 = dm_s.Faces[i].Indexes[2];

				faces.push_back(t);
			}

			for (auto i = 0; i < dm_s.BoneConnectors.size(); i++)
			{
				Mesh_Imp::BoneConnector t;
				t.TargetIndex = dm_s.BoneConnectors[i].TargetIndex;
				t.BoneToMesh = dm_s.BoneConnectors[i].OffsetMatrix;
				boneConnectors.push_back(t);
			}
			
			for (auto i = 0; i < dm_s.MaterialOffsets.size(); i++)
			{
				Mesh_Imp::MaterialOffset t;
				t.MaterialIndex = dm_s.MaterialOffsets[i].MaterialIndex;
				t.FaceOffset = dm_s.MaterialOffsets[i].FaceOffset;
				materialOffsets.push_back(t);
			}

			mesh->AddDividedMesh(vertexes, faces, boneConnectors, materialOffsets);
		}

		
		for (auto i = 0; i < io.Materials.size(); i++)
		{
			auto& material = io.Materials[i];
			auto ind = mesh->AddMaterial();
	
			if (material.ColorTexture != astring()) mesh->SetColorTexture(ind, g->CreateTexture2D(material.ColorTexture.c_str()).get());
			if (material.NormalTexture != astring()) mesh->SetNormalTexture(ind, g->CreateTexture2DAsRawData(material.NormalTexture.c_str()).get());
			if (material.MetalnessTexture != astring()) mesh->SetMetalnessTexture(ind, g->CreateTexture2DAsRawData(material.MetalnessTexture.c_str()).get());
		}

		return mesh;
	}

	Deformer_Imp* Model_Imp::LoadDeformer(Graphics* g, Model_IO& io, const achar* path)
	{
		auto g_ = (Graphics_Imp*) g;
		auto deformer = (Deformer_Imp*) g_->CreateDeformer_();
		for (auto i = 0; i < io.Deformer_.Bones.size(); i++)
		{
			auto& bone = io.Deformer_.Bones[i];
			deformer->AddBone( bone.Name.c_str(), bone.ParentBoneIndex, bone.RotationType, bone.LocalMat);
		}

		return deformer;
	}

	void Model_Imp::LoadAnimationClip(Model_IO::AnimationClip& io, std::vector<AnimationSource*>& sources)
	{
		auto clip = new AnimationClip_Imp();
		
		auto clipName = io.Name;
		auto sourceIndex = io.Index;

		clip->SetSource(sources[sourceIndex]);

		m_animationClips.push_back(clip);
		m_animationClipNames.push_back(clipName);
	}

	AnimationSource* Model_Imp::LoadAnimationSource(Model_IO::AnimationSource& io)
	{
		auto source = new AnimationSource_Imp();

		auto sourceName = io.Name;

		for (auto i = 0; i < io.KeyframeAnimations.size(); i++)
		{
			auto keyframeanimation = LoadKeyframeAnimation(io.KeyframeAnimations[i]);
			source->AddAnimation(keyframeanimation);
			keyframeanimation->Release();
		}

		return source;
	}

	KeyframeAnimation* Model_Imp::LoadKeyframeAnimation(Model_IO::KeyframeAnimation& io)
	{
		auto keyframeAnimation = new KeyframeAnimation_Imp();

		auto name = io.Name;
		keyframeAnimation->SetName(name.c_str());
		
		for (int32_t i = 0; i < io.Keyframes.size(); i++)
		{
			auto keyframe = io.Keyframes[i];
			keyframeAnimation->AddKeyframe(keyframe);
		}

		return keyframeAnimation;
	}

	Mesh* Model_Imp::GetMesh_(int32_t index)
	{
		if (!(0 <= index && index < m_meshGroup->Meshes.size())) return nullptr;
		return m_meshGroup->Meshes[index];
	}
}