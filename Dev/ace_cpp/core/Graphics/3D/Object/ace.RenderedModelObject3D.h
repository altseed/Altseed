
#pragma once

#include "ace.RenderedObject3D.h"
#include <Graphics/ace.Graphics.Common.h>
#include <Math/ace.Vector4DF.h>

namespace ace
{
	class RenderedModelObject3DProxy
		: public RenderedObject3DProxy
	{
	public:
	};

	class RenderedModelObject3D
		: public RenderedObject3D
	{
		struct VertexConstantBufferLightweight
		{
			Matrix44	matM[32];
			Matrix44	matC;
			Matrix44	matP;
			Vector3DF	directionalLightDirection;
			float		Padding1;

			Vector3DF	directionalLightColor;
			float		Padding2;

			Vector3DF	skyLightColor;
			float		Padding3;

			Vector3DF	groundLightColor;
			float		Padding4;
		};

		struct VertexConstantBufferDeferredRendering
		{
			Matrix44	matM[32];
			Matrix44	matC;
			Matrix44	matP;

			Vector3DF	depthParams;
			float		Padding0;
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
		public:
			std::vector<Matrix44>	m_matrixes_fr;
			std::vector<Matrix44>	m_matrixes;

			std::vector < BoneProperty>	m_boneProps;

			MeshGroup();
			~MeshGroup();
		};
		

	private:
		std::vector<std::shared_ptr<Mesh>>		m_meshes;
		std::vector<std::shared_ptr<Mesh>>		m_meshes_rt;

		std::shared_ptr<Deformer>				m_deformer;
		std::shared_ptr<Deformer>				m_deformer_rt;

		std::vector<Matrix44>					m_matrixes_rt;
		std::vector<Matrix44>					m_matrixes;
		std::vector <BoneProperty>				m_boneProps;

		Model_Imp*								m_model = nullptr;

		std::shared_ptr<ace::NativeShader_Imp>	m_shaderDF;
		std::shared_ptr<ace::NativeShader_Imp>	m_shaderDF_ND;
		std::shared_ptr<ace::NativeShader_Imp>	m_shaderLightweight;
		
		std::map<astring, AnimationClip*>		m_animationClips;

		AnimationClip*							m_animationPlaying;
		int32_t									m_animationTime;

		Renderer3D*								m_renderer = nullptr;
		RenderedModelObject3DProxy*				proxy = nullptr;

		void Flip(AnimationClip* animationClip, int32_t time);
		void CalculateAnimation(AnimationClip* animationClip, int32_t time);
		void CalclateBoneMatrices(bool isPlayingAnimation);

	public:
		RenderedModelObject3D(Graphics* graphics);
		virtual ~RenderedModelObject3D();

		void SetModel(Model* model);

		void AddMesh(Mesh* mesh);

		void SetDeformer(Deformer* deformer);

		void OnAdded(Renderer3D* renderer) override;

		void OnRemoving(Renderer3D* renderer) override;

		void Flip() override;
		void Rendering(RenderingProperty& prop) override;

		RenderedObject3DProxy* GetProxy() const override { return proxy; }

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
		void AddMesh(std::shared_ptr<Mesh>& mesh)
		{
			AddMesh(mesh.get());
		}
#endif
	};
}