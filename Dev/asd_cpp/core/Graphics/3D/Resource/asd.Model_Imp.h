
#pragma once

#include "asd.Model.h"

#include <Graphics/3D/asd.Model_IO.h>

namespace asd
{
	class Model_Imp
		: public Model
		, public ReferenceObject
	{
	public:
		class MeshGroup
			: public ReferenceObject
		{
		public:
			std::vector<Mesh_Imp*>		Meshes;
			Deformer_Imp*				Deformer_;

			MeshGroup() = default;
			virtual ~MeshGroup();
		};

	private:
		MeshGroup*					m_meshGroup;

		std::vector<astring>		m_animationClipNames;
		std::vector<AnimationClip*>	m_animationClips;

		Graphics*					m_graphics;

		std::set<RenderedModelObject3D*>	m_observers;

		void Reset();

	public:
		Model_Imp(Graphics* graphics);
		virtual ~Model_Imp();

		bool Load(Graphics* g, const std::vector<uint8_t>& data, const achar* path);

		/**
			@brief	モデルに変更が発生したことを伝える先を登録する。
			@param	model	観察者
		*/
		void Attach(RenderedModelObject3D* model);

		/**
			@brief	モデルに変更が発生したことを伝える先を解除する。
			@param	model	観察者
		*/
		void Detach(RenderedModelObject3D* model);

		void Reload(const std::vector<uint8_t>& data, const achar* path);

	private:
		MeshGroup* LoadMeshGroup(Graphics* g, Model_IO& io, const achar* path);
		Mesh_Imp* LoadMesh(Graphics* g, Model_IO::Mesh& io, const achar* path);
		Deformer_Imp* LoadDeformer(Graphics* g, Model_IO& io, const achar* path);

		void LoadAnimationClip(Model_IO::AnimationClip& io, std::vector<AnimationSource*>& source);
		AnimationSource* LoadAnimationSource(Model_IO::AnimationSource& io);
		KeyframeAnimation* LoadKeyframeAnimation(Model_IO::KeyframeAnimation& io);

	protected:
		Mesh* GetMesh_(int32_t index) override;
		AnimationClip* GetAnimationClip_(int32_t index) override;
		const achar* GetAnimationClipName(int32_t index) override;

	public:

		int32_t GetAnimationClipCount() const { return m_animationClips.size(); }

		std::vector<AnimationClip*>& GetAnimationClips() { return m_animationClips; }
		std::vector<astring>& GetAnimationClipNames() { return m_animationClipNames; }
		MeshGroup* GetMeshGroup() { return m_meshGroup; }
		
		int32_t GetMeshCount() const override { return m_meshGroup->Meshes.size(); }

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}