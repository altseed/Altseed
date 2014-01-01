
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

			Matrix44 CalcMatrix(eBoneRotationType rotationType);
		};

	private:
		Mesh_Imp*		m_mesh;
		Armature_Imp*	m_armature;

		std::vector<Matrix44>	m_matrixes_fr;
		std::vector<Matrix44>	m_matrixes;

		std::vector < BoneProperty>	m_boneProps;

		std::shared_ptr<ace::NativeShader_Imp>	m_shader;

		std::map<astring, AnimationClip*>		m_animationClips;

		AnimationClip*							m_animationPlaying;
		int32_t									m_animationTime;

	public:
		RenderedMeshObject3D(Graphics* graphics);
		virtual ~RenderedMeshObject3D();

		void Flip() override;
		void Rendering(RenderingProperty& prop) override;

		void SetMesh(Mesh* mesh);

		void SetArmature(Armature* armature);

		void AddAnimationClip(const achar* name, AnimationClip* animationClip);

		void PlayAnimation(const achar* name);

		eRenderedObject3DType GetObjectType() const override { return RENDERED_OBJECT3D_TYPE_MESH; }
	};
}