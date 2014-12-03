
#pragma once

#include "ace.RenderedObject3D.h"
#include <Graphics/ace.Graphics.Common.h>
#include <Math/ace.Vector4DF.h>

namespace ace
{
	struct BoneProperty
	{
		float	Position[3];
		float	Rotation[4];
		float	Scale[3];

		float	PositionWeight[3];
		float	RotationWeight[4];
		float	ScaleWeight[3];

		bool	IsAnimationPlaying;

		BoneProperty();

		Matrix44 CalcMatrix(eRotationOrder rotationType);
	};

	struct PlayedAnimation
	{
		std::shared_ptr<AnimationClip>		Animation;
		float								CurrentWeight;
		float								Variation;
		int32_t								Time;
	};

	class RenderedModelObject3DProxy
		: public RenderedObject3DProxy
	{
	private:
		static const int32_t					AnimationCount = 4;

		std::shared_ptr<ace::NativeShader_Imp>	m_shaderDF;
		std::shared_ptr<ace::NativeShader_Imp>	m_shaderDF_ND;
		std::shared_ptr<ace::NativeShader_Imp>	m_shaderLightweight;

		std::vector<ShaderConstantValue> shaderConstants;

		RenderedObject3DCullingProxy	cullingProxy;

	public:
		bool									calcAnimationOnProxy = false;
		std::vector<Matrix44>					m_matrixes;
		std::vector<Matrix44>					m_matrixes_temp;

		std::vector<std::shared_ptr<Mesh>>		m_meshes;
		std::shared_ptr<Deformer>				m_deformer;
		std::vector <BoneProperty>				m_boneProps;

		std::vector<PlayedAnimation>			m_animationPlaying[AnimationCount];
		float									m_animationWeight[AnimationCount];

		std::vector<std::vector<std::shared_ptr<MaterialPropertyBlock>>>	materialPropertyBlocks;

		Culling3D::Object*						CullingObject = nullptr;

		RenderedModelObject3DProxy(Graphics* graphics);
		virtual ~RenderedModelObject3DProxy();

		void OnUpdateAsync() override;

		void OnAdded(Renderer3DProxy* renderer) override;

		void OnRemoving(Renderer3DProxy* renderer) override;

		void Rendering(RenderingCommandHelper* helper, RenderingProperty& prop) override;

		eRenderedObject3DType GetObjectType() const override { return RENDERED_OBJECT3D_TYPE_MESH; }
	};

	class RenderedModelObject3D
		: public RenderedObject3D
	{
	private:
		static const int32_t					AnimationCount = 4;

		std::vector<std::shared_ptr<Mesh>>		m_meshes;
		std::shared_ptr<Deformer>				m_deformer;
		std::vector<Matrix44>					m_matrixes;
		std::vector<Matrix44>					m_matrixes_temp;

		std::vector<std::vector<std::shared_ptr<MaterialPropertyBlock>>>	materialPropertyBlocks;

		std::vector <BoneProperty>				m_boneProps;

		Model_Imp*								m_model = nullptr;

		std::map<astring, AnimationClip*>		m_animationClips;

		std::vector<PlayedAnimation>			m_animationPlaying[AnimationCount];
		float									m_animationWeight[AnimationCount];

		Renderer3D*								m_renderer = nullptr;
		RenderedModelObject3DProxy*				proxy = nullptr;

	public:
		RenderedModelObject3D(Graphics* graphics);
		virtual ~RenderedModelObject3D();

		void SetModel(Model* model);

		void AddMesh(Mesh* mesh);

		void SetDeformer(Deformer* deformer);

		void SetMaterialPropertyBlock(int32_t meshIndex, int32_t materialIndex, MaterialPropertyBlock* block);

		void OnAdded(Renderer3D* renderer) override;

		void OnRemoving(Renderer3D* renderer) override;

		void OnApplyingNextSRT(Matrix44& nextMat) override;

		void Flip(float deltaTime) override;

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

		AnimationClip* GetAnimationClip(const achar* name);

		void AddAnimationClip(const achar* name, AnimationClip* animationClip);

		void PlayAnimation(int32_t index, const achar* name);

		void StopAnimation(int32_t index);

		void SetAnimationWeight(int32_t index, float weight);

		void CrossFadeAnimation(int32_t index, const achar* name, float time);

		bool IsAnimationPlaying(int32_t index);

		eRenderedObject3DType GetObjectType() const override { return RENDERED_OBJECT3D_TYPE_MESH; }

#if !SWIG
		void SetMaterialPropertyBlock(int32_t meshIndex, int32_t materialIndex, std::shared_ptr<MaterialPropertyBlock> block)
		{
			SetMaterialPropertyBlock(meshIndex, materialIndex, block);
		}

		void AddMesh(std::shared_ptr<Mesh>& mesh)
		{
			AddMesh(mesh.get());
		}
#endif
	};
}