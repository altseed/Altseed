
#pragma once

#include "ace.RenderedObject3D.h"
#include <Graphics/ace.Graphics.Common.h>
#include <Math/ace.Vector4DF.h>

namespace ace
{
	class RenderedModelObject3D
		: public RenderedObject3D
	{
		struct VertexConstantBuffer
		{
			Matrix44	MMatrices[32];
			Matrix44	CMatrix;
			Matrix44	PMatrix;
			Matrix44	LightVPMatrix;

			Vector3DF	DirectionalLightDirection;
			float		Padding0;

			Vector3DF	DirectionalLightColor;
			float		Padding1;
		};

		struct PixelConstantBuffer
		{
			Vector4DF	HasTextures;
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
			std::vector<Mesh_Imp*>		m_meshes;
			Deformer_Imp*	m_deformer;

		public:
			std::vector<Matrix44>	m_matrixes_fr;
			std::vector<Matrix44>	m_matrixes;

			std::vector < BoneProperty>	m_boneProps;

			MeshGroup();
			~MeshGroup();

			void Flip(AnimationClip* animationClip, int32_t time);

			void CalculateAnimation(AnimationClip* animationClip, int32_t time);
			void CalclateBoneMatrices();
			
			void AddMesh(Mesh_Imp* mesh);
			void SetDeformer(Deformer_Imp* deformer);

			std::vector<Mesh_Imp*>& GetMeshes() { return m_meshes; }
		};
		

	private:
		std::vector<std::shared_ptr<MeshGroup>>	m_meshGroups;
		std::vector<std::shared_ptr<MeshGroup>>	m_meshGroups_fr;

		Model_Imp*								m_model = nullptr;

		std::shared_ptr<ace::NativeShader_Imp>	m_shader;
		std::shared_ptr<ace::NativeShader_Imp>	m_shaderShadow;
		std::shared_ptr<ace::NativeShader_Imp>	m_shaderNormalDepth;

		std::map<astring, AnimationClip*>		m_animationClips;

		AnimationClip*							m_animationPlaying;
		int32_t									m_animationTime;

	public:
		RenderedModelObject3D(Graphics* graphics);
		virtual ~RenderedModelObject3D();

		void SetModel(Model* model);

		void AddMeshGroup();

		int32_t GetMeshGroupCount();

		void AddMesh(int32_t meshGroupIndex, Mesh* mesh);

		void SetDeformer(int32_t meshGroupIndex, Deformer* deformer);

		void Flip() override;
		void Rendering(RenderingProperty& prop) override;
		void RenderingShadowMap(RenderingShadowMapProperty& prop) override;
		void RenderingNormalDepth(RenderingProperty& prop) override;

		/**
			@brief	モデルの解除を行わずに、現在設定されているインスタンスを解除する。
		*/
		void UnloadModel();

		/**
			@brief	現在設定されているモデルからインスタンスを読み込む。
		*/
		void LoadModel();

		void ReloadModel();

		void AddAnimationClip(const achar* name, AnimationClip* animationClip);

		void PlayAnimation(const achar* name);

		eRenderedObject3DType GetObjectType() const override { return RENDERED_OBJECT3D_TYPE_MESH; }

#if !SWIG
		void AddMesh(int32_t meshGroupIndex, std::shared_ptr<Mesh>& mesh)
		{
			AddMesh(meshGroupIndex, mesh.get());
		}
#endif
	};
}