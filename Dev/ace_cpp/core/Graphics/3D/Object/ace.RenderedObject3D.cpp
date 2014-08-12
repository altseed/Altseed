
#include "ace.RenderedObject3D.h"
#include "../../ace.Graphics_Imp.h"

namespace ace
{
	const Matrix44& RenderedObject3DProxy::GetLocalMatrix()
	{
		if (isSRTChanged)
		{
			localMatrix = RenderedObject3D::CalcLocalMatrix(
				Position,
				Rotation,
				Scale);
			isSRTChanged = false;
		}

		return localMatrix;
	}


	const Matrix44& RenderedObject3DProxy::GetGlobalMatrix()
	{
		return GetLocalMatrix();
	}

	Vector3DF RenderedObject3DProxy::GetGlobalPosition()
	{
		auto mat = GetLocalMatrix();
		return Vector3DF(mat.Values[0][3], mat.Values[1][3], mat.Values[2][3]);
	}

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

	RenderedObject3D::RenderedObject3D(Graphics* graphics)
		: m_graphics(nullptr)
	{
		m_graphics = (Graphics_Imp*) graphics;
		SafeAddRef(m_graphics);

		scale = Vector3DF(1.0f, 1.0f, 1.0f);
		isSRTChanged = true;
	}

	RenderedObject3D::~RenderedObject3D()
	{
		SafeRelease(m_graphics);
	}

	Vector3DF RenderedObject3D::GetPosition() const
	{
		return position;
	}

	void RenderedObject3D::SetPosition(const Vector3DF& pos)
	{
		if (position != pos) isSRTChanged = true;
		position = pos;
	}

	Vector3DF RenderedObject3D::GetRotation() const
	{
		return rotation;
	}

	void RenderedObject3D::SetRotation(const Vector3DF& rot)
	{
		if (rotation != rot)isSRTChanged = true;
		rotation = rot;
	}

	Vector3DF RenderedObject3D::GetScale() const
	{
		return scale;
	}

	void RenderedObject3D::SetScale(const Vector3DF& scale)
	{
		if (this->scale != scale) isSRTChanged = true;
		this->scale = scale;
	}

	const Matrix44& RenderedObject3D::GetLocalMatrix()
	{
		if (isSRTChanged)
		{
			localMatrix = CalcLocalMatrix(
				position,
				rotation,
				scale);
			isSRTChanged = false;
		}
		
		return localMatrix;
	}

	void RenderedObject3D::Flip()
	{
		auto proxy = GetProxy();

		proxy->Position = position;
		proxy->Rotation = rotation;
		proxy->Scale = scale;
		proxy->isSRTChanged = isSRTChanged;

		if (!isSRTChanged)
		{
			proxy->localMatrix = localMatrix;
		}
	}
}