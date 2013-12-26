
#pragma once

#include "ace.RenderedObject3D.h"

namespace ace
{
	class RenderedMeshObject3D
		: public RenderedObject3D
	{
		struct VertexConstantBuffer
		{
			Matrix44	MCPMatrix;

			Vector3DF	DirectionalLightDirection;
			float		Padding0;

			Vector3DF	DirectionalLightColor;
			float		Padding1;
		};

	private:
		Mesh_Imp*		m_mesh;
		Armature_Imp*	m_armature;

		std::vector<Matrix44>	m_matrixes_fr;
		std::vector<Matrix44>	m_matrixes;

		std::shared_ptr<ace::NativeShader_Imp>	m_shader;

	public:
		RenderedMeshObject3D(Graphics* graphics);
		virtual ~RenderedMeshObject3D();

		void Flip() override;
		void Rendering(RenderingProperty& prop) override;

		void SetMesh(Mesh* mesh);

		void SetArmature(Armature* armature);

		eRenderedObject3DType GetObjectType() const override { return RENDERED_OBJECT3D_TYPE_MESH; }
	};
}