
#include "ace.RenderedObject3D.h"
#include "../ace.Graphics_Imp.h"

namespace ace
{
	Matrix44 RenderedObject3D::CalcLocalMatrix(Vector3DF& t, Vector3DF& r, Vector3DF& s)
	{
		Matrix44 mat, matt, matrx, matry, matrz, mats;
		matt.SetTranslation(t.X, t.Y, t.Z);
		matrx.SetRotationX(r.X / 180.0f * 3.141592f);
		matry.SetRotationY(r.Y / 180.0f * 3.141592f);
		matrz.SetRotationZ(r.Z / 180.0f * 3.141592f);
		mats.SetScale(s.X, s.Y, s.Z);

		Matrix44::Mul(mat, matrz, mats);
		Matrix44::Mul(mat, matrx, mat);
		Matrix44::Mul(mat, matry, mat);
		Matrix44::Mul(mat, matt, mat);

		return mat;
	}

	Matrix44 RenderedObject3D::CalcLocalMatrix()
	{
		return CalcLocalMatrix(
			m_commonValues.position,
			m_commonValues.rotation,
			m_commonValues.scale);
	}

	const Matrix44& RenderedObject3D::GetLocalMatrix_FR()
	{
		return m_commonValues_FR.localMatrix;
	}

	RenderedObject3D::RenderedObject3D(Graphics* graphics)
		: m_graphics(nullptr)
	{
		m_graphics = (Graphics_Imp*) graphics;
		SafeAddRef(m_graphics);

		m_commonValues.scale = Vector3DF(1.0f, 1.0f, 1.0f);
		m_commonValues.isChanged = true;
	}

	RenderedObject3D::~RenderedObject3D()
	{
		SafeRelease(m_graphics);
	}

	Vector3DF RenderedObject3D::GetPosition() const
	{
		return m_commonValues.position;
	}

	void RenderedObject3D::SetPosition(const Vector3DF& pos)
	{
		m_commonValues.position = pos;
	}

	Vector3DF RenderedObject3D::GetRotation() const
	{
		return m_commonValues.rotation;
	}

	void RenderedObject3D::SetRotation(const Vector3DF& rot)
	{
		if (m_commonValues.rotation != rot) m_commonValues.isChanged = true;
		m_commonValues.rotation = rot;
	}

	Vector3DF RenderedObject3D::GetScale() const
	{
		return m_commonValues.scale;
	}

	void RenderedObject3D::SetScale(const Vector3DF& scale)
	{
		if (m_commonValues.scale != scale) m_commonValues.isChanged = true;
		m_commonValues.scale = scale;
	}

	void RenderedObject3D::CalculateMatrix_FR()
	{
		if (m_commonValues_FR.isChanged)
		{
			m_commonValues_FR.localMatrix = CalcLocalMatrix(
				m_commonValues_FR.position,
				m_commonValues_FR.rotation,
				m_commonValues_FR.scale);
			m_commonValues_FR.isChanged = false;
		}
	}

	void RenderedObject3D::Flip()
	{
		if (m_commonValues.isChanged)
		{
			m_commonValues_FR.position = m_commonValues.position;
			m_commonValues_FR.rotation = m_commonValues.rotation;
			m_commonValues_FR.scale = m_commonValues.scale;
			m_commonValues_FR.isChanged = true;

			m_commonValues.isChanged = false;
		}
		else
		{
			m_commonValues_FR.isChanged = false;
		}
	}
}