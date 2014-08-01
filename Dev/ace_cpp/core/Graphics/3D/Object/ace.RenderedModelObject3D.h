
#pragma once

#include "ace.RenderedObject3D.h"
#include <Graphics/ace.Graphics.Common.h>
#include <Math/ace.Vector4DF.h>

namespace ace
{
	class RenderedModelObject3DProxy
		: public RenderedObject3DProxy
	{
	private:
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

		std::shared_ptr<ace::NativeShader_Imp>	m_shaderDF;
		std::shared_ptr<ace::NativeShader_Imp>	m_shaderDF_ND;
		std::shared_ptr<ace::NativeShader_Imp>	m_shaderLightweight;

		std::vector<ShaderConstantValue> shaderConstants;

	public:
		std::vector<Matrix44>					m_matrixes_rt;
		std::vector<std::shared_ptr<Mesh>>		m_meshes_rt;
		std::shared_ptr<Deformer>				m_deformer_rt;

		RenderedModelObject3DProxy(Graphics* graphics);
		virtual ~RenderedModelObject3DProxy();

		void Rendering(RenderingCommandHelper* helper, RenderingProperty& prop) override;
	};

	class RenderedModelObject3D
		: public RenderedObject3D
	{
		struct BoneProperty
		{
			float	Position[3];
			float	Rotation[4];
			float	Scale[3];

			BoneProperty();

			Matrix44 CalcMatrix(eRotationOrder rotationType);
		};

	private:
		std::vector<std::shared_ptr<Mesh>>		m_meshes;
		std::shared_ptr<Deformer>				m_deformer;
		std::vector<Matrix44>					m_matrixes;

		std::vector <BoneProperty>				m_boneProps;

		Model_Imp*								m_model = nullptr;

		std::map<astring, AnimationClip*>		m_animationClips;

		AnimationClip*							m_animationPlaying;
		int32_t									m_animationTime;

		Renderer3D*								m_renderer = nullptr;
		RenderedModelObject3DProxy*				proxy = nullptr;

		static void CalculateAnimation(std::vector <BoneProperty>& boneProps, Deformer* deformer, AnimationClip* animationClip, int32_t time);
		static void CalclateBoneMatrices(std::vector<Matrix44>& matrixes, std::vector <BoneProperty>& boneProps, Deformer* deformer, bool isPlayingAnimation);

	public:
		RenderedModelObject3D(Graphics* graphics);
		virtual ~RenderedModelObject3D();

		void SetModel(Model* model);

		void AddMesh(Mesh* mesh);

		void SetDeformer(Deformer* deformer);

		void OnAdded(Renderer3D* renderer) override;

		void OnRemoving(Renderer3D* renderer) override;

		void Flip() override;

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