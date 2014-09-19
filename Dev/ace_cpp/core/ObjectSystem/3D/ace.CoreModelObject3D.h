
#pragma once

#include "ace.CoreObject3D.h"

namespace ace
{
	class CoreModelObject3D
		: public CoreObject3D
	{
	protected:
		CoreModelObject3D(){}
		virtual ~CoreModelObject3D(){}

	public:
		virtual void SetModel(Model* model) = 0;

		virtual void AddMesh(Mesh* mesh) = 0;

		virtual void SetDeformer(Deformer* deformer) = 0;

		virtual AnimationClip* GetAnimationClip(const achar* name) = 0;

		virtual void PlayAnimation(int32_t index, const achar* name) = 0;

		virtual void StopAnimation(int32_t index) = 0;

		virtual void SetAnimationWeight(int32_t index, float weight) = 0;

		virtual void CrossFadeAnimation(int32_t index, const achar* name, float time) = 0;

		virtual bool IsAnimationPlaying(int32_t index) = 0;

		virtual void SetMaterialPropertyBlock(int32_t meshIndex, int32_t materialIndex, MaterialPropertyBlock* block) = 0;
	};
}
