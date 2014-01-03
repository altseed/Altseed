
#pragma once

#include "ace.RenderedObject3D.h"

namespace ace
{
	class RenderedMeshObject3D
		: public RenderedObject3D
	{
		struct VertexConstantBuffer
		{
			Matrix44	MCPMatrices[32];

			Vector3DF	DirectionalLightDirection;
			float		Padding0;

			Vector3DF	DirectionalLightColor;
			float		Padding1;
		};

		struct BoneProperty
		{
			float	Position[3];
			float	Rotation[4];
			float	Scale[3];

			BoneProperty();

			Matrix44 CalcMatrix(eRotationOrder rotationType);
		};

		class MeshGroup
		{
		private:
			Mesh_Imp*		m_mesh;
			Deformer_Imp*	m_deformer;

		public:
			std::vector<Matrix44>	m_matrixes_fr;
			std::vector<Matrix44>	m_matrixes;

			std::vector < BoneProperty>	m_boneProps;

			MeshGroup(Mesh_Imp* mesh);
			~MeshGroup();

			void Flip(AnimationClip* animationClip, int32_t time);

			void CalculateAnimation(AnimationClip* animationClip, int32_t time);
			void CalclateBoneMatrices();

			Mesh_Imp* GetMesh() { return m_mesh; }
		private:
			/**
			@brief	メッシュのDeformerと比較し、違っていたら差し替える。
			*/
			void CheckDeformer();
			void SetInternalDeformer(Deformer* deformer);
		};
		

	private:
		std::vector<std::shared_ptr<MeshGroup>>	m_meshGroups;
		std::vector<std::shared_ptr<MeshGroup>>	m_meshGroups_fr;

		std::shared_ptr<ace::NativeShader_Imp>	m_shader;

		std::map<astring, AnimationClip*>		m_animationClips;

		AnimationClip*							m_animationPlaying;
		int32_t									m_animationTime;

	public:
		RenderedMeshObject3D(Graphics* graphics);
		virtual ~RenderedMeshObject3D();

		void Flip() override;
		void Rendering(RenderingProperty& prop) override;

		void AddMesh(Mesh* mesh);

		void AddAnimationClip(const achar* name, AnimationClip* animationClip);

		void PlayAnimation(const achar* name);

		eRenderedObject3DType GetObjectType() const override { return RENDERED_OBJECT3D_TYPE_MESH; }

#if !SWIG
		void AddMesh(std::shared_ptr<Mesh>& mesh)
		{
			AddMesh(mesh.get());
		}
#endif
	};
}