
#include "ace.RenderedDirectionalLightObject3D.h"

namespace ace
{
	RenderedDirectionalLightObject3D::RenderedDirectionalLightObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
	{
		m_values.color = Color(255, 255, 255, 255);
		m_values_FR.color = Color(255, 255, 255, 255);
	}

	RenderedDirectionalLightObject3D::~RenderedDirectionalLightObject3D()
	{
	
	}

	void RenderedDirectionalLightObject3D::Flip()
	{
		RenderedObject3D::Flip();
		m_values_FR.color = m_values.color;
	}

	void RenderedDirectionalLightObject3D::Rendering(RenderingProperty& prop)
	{
		
	}

	Color RenderedDirectionalLightObject3D::GetColor_FR()
	{
		return m_values_FR.color;
	}

	void RenderedDirectionalLightObject3D::SetColor(Color color)
	{
		m_values.color = color;
	}

	Vector3DF RenderedDirectionalLightObject3D::GetDirection_FR()
	{
		auto& mat = GetLocalMatrix_FR();
		return Vector3DF(mat.Values[0][2], mat.Values[1][2], mat.Values[2][2]);
	}
}