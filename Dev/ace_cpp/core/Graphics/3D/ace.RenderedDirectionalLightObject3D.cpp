
#include "ace.RenderedDirectionalLightObject3D.h"

#include "../ace.Graphics_Imp.h"

#include "../Resource/ace.RenderTexture2D_Imp.h"
#include "../Resource/ace.DepthBuffer_Imp.h"

namespace ace
{
	RenderedDirectionalLightObject3D::RenderedDirectionalLightObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
	{
		m_values.color = Color(255, 255, 255, 255);
		m_values_RT.color = Color(255, 255, 255, 255);

		m_shadowTexture = GetGraphics()->CreateRenderTexture2D_Imp(ShadowBufferSize, ShadowBufferSize, eTextureFormat::TEXTURE_FORMAT_GL_R16G16_FLOAT);
		m_shadowDepthBuffer = GetGraphics()->CreateDepthBuffer_Imp(ShadowBufferSize, ShadowBufferSize);
	}

	RenderedDirectionalLightObject3D::~RenderedDirectionalLightObject3D()
	{
		SafeRelease(m_shadowTexture);
		SafeRelease(m_shadowDepthBuffer);
	}

	void RenderedDirectionalLightObject3D::Flip()
	{
		RenderedObject3D::Flip();
		m_values_RT.color = m_values.color;
	}

	void RenderedDirectionalLightObject3D::Rendering(RenderingProperty& prop)
	{
		
	}

	void RenderedDirectionalLightObject3D::CalcShadowMatrix(Vector3DF viewPosition, Vector3DF viewDirection, Matrix44 matCameraProj, float zn, float zf, Matrix44& lightView, Matrix44& lightProjection)
	{
		auto calcAABB = [](std::vector<Vector3DF>& points, Vector3DF& max_, Vector3DF& min_) -> void
		{
			min_ = Vector3DF(100000.0f, 100000.0f, 100000.0f);
			max_ = Vector3DF(-100000.0f, -100000.0f, -100000.0f);
			for (auto p : points)
			{
				if (min_.X > p.X) min_.X = p.X;
				if (min_.Y > p.Y) min_.Y = p.Y;
				if (min_.Z > p.Z) min_.Z = p.Z;

				if (max_.X < p.X) max_.X = p.X;
				if (max_.Y < p.Y) max_.Y = p.Y;
				if (max_.Z < p.Z) max_.Z = p.Z;
			}
		};

		auto calcCubeClipMatrix = [this](Vector3DF& max_, Vector3DF& min_)->Matrix44
		{
			Matrix44 matCubeClip;
			matCubeClip.SetIndentity();
			matCubeClip.Values[0][0] = 2.0f / (max_.X - min_.X);
			matCubeClip.Values[0][1] = 0.0f;
			matCubeClip.Values[0][2] = 0.0f;
			matCubeClip.Values[0][3] = -(max_.X + min_.X) / (max_.X - min_.X);

			matCubeClip.Values[1][0] = 0.0f;
			matCubeClip.Values[1][1] = 2.0f / (max_.Y - min_.Y);
			matCubeClip.Values[1][2] = 0.0f;
			matCubeClip.Values[1][3] = -(max_.Y + min_.Y) / (max_.Y - min_.Y);

			// もしかしたら符号が逆の可能性あり
			if (GetGraphics()->GetGraphicsType() == GraphicsType::DirectX11)
			{
				matCubeClip.Values[2][0] = 0.0f;
				matCubeClip.Values[2][1] = 0.0f;
				matCubeClip.Values[2][2] = -1.0f / (max_.Z - min_.Z);
				matCubeClip.Values[2][3] = min_.Z / (max_.Z - min_.Z) + 1.0f;
			}
			else
			{
				matCubeClip.Values[2][0] = 0.0f;
				matCubeClip.Values[2][1] = 0.0f;
				matCubeClip.Values[2][2] = -2.0f / (max_.Z - min_.Z);
				matCubeClip.Values[2][3] = (max_.Z + min_.Z) / (max_.Z - min_.Z);
			}

			return matCubeClip;
		};

		// LiSPSMで行列を計算する。

		// 初期化
		auto lightDirection = GetDirection_RT();
		viewDirection.Normalize();
		lightDirection.Normalize();

		m_shadowObjectPoints.clear();

		// ライトビューに含むオブジェクトの座標算出
		auto matCPInv = matCameraProj.GetInverted();

		if (GetGraphics()->GetGraphicsType() == GraphicsType::DirectX11)
		{
			Vector3DF points[8] = {
				Vector3DF(1.0f, 1.0f, 0.0f),
				Vector3DF(-1.0f, 1.0f, 0.0f),
				Vector3DF(1.0f, -1.0f, 0.0f),
				Vector3DF(-1.0f, -1.0f, 0.0f),
				Vector3DF(1.0f, 1.0f, 1.0f),
				Vector3DF(-1.0f, 1.0f, 1.0f),
				Vector3DF(1.0f, -1.0f, 1.0f),
				Vector3DF(-1.0f, -1.0f, 1.0f),
			};

			for (int32_t i = 0; i < 8; i++)
			{
				points[i] = matCPInv.Transform3D(points[i]);
				m_shadowObjectPoints.push_back(points[i]);
			}
		}
		else
		{
			Vector3DF points[8] = {
				Vector3DF(1.0f, 1.0f, -1.0f),
				Vector3DF(-1.0f, 1.0f, -1.0f),
				Vector3DF(1.0f, -1.0f, -1.0f),
				Vector3DF(-1.0f, -1.0f, -1.0f),
				Vector3DF(1.0f, 1.0f, 1.0f),
				Vector3DF(-1.0f, 1.0f, 1.0f),
				Vector3DF(1.0f, -1.0f, 1.0f),
				Vector3DF(-1.0f, -1.0f, 1.0f),
			};

			for (int32_t i = 0; i < 8; i++)
			{
				points[i] = matCPInv.Transform3D(points[i]);
				m_shadowObjectPoints.push_back(points[i]);
			}
		}

		m_shadowObjectPointsBack = m_shadowObjectPoints;
		auto back2 = m_shadowObjectPoints;

		// LiSPSMかUSMか
		auto vlCross = Vector3DF::Cross(viewDirection, lightDirection);
		auto vlS = vlCross.GetLength();
		auto vlC = Vector3DF::Dot(viewDirection, lightDirection);
		auto vlAngle = atan2(vlS, vlC);

		if (fabsf(vlAngle) < 0.01f || fabsf(vlAngle - PI) < 0.01f)
		{
			// 視線とライトの方向が近い場合はUSM

			// ライトビューの計算
			auto eye = viewPosition;
			lightView.SetLookAtRH(eye, eye + lightDirection, viewDirection);

			// AABB計算
			for (auto i = 0; i < m_shadowObjectPoints.size(); i++)
			{
				m_shadowObjectPoints[i] = lightView.Transform3D(m_shadowObjectPoints[i]);
			}

			Vector3DF min_, max_;
			calcAABB(m_shadowObjectPoints, max_, min_);

			lightProjection = calcCubeClipMatrix(max_, min_);

			// デバッグ用
			/*
			for (auto i = 0; i < m_shadowObjectPoints.size(); i++)
			{
				m_shadowObjectPoints[i] = lightProjection.Transform3D(m_shadowObjectPoints[i]);
			}
			

			{
				Vector3DF points[8] = {
					Vector3DF(0.5f, 0.5f, -0.5f),
					Vector3DF(-0.5f, 0.5f, -0.5f),
					Vector3DF(0.5f, -0.5f, -0.5f),
					Vector3DF(-0.5f, -0.5f, -0.5f),
					Vector3DF(0.5f, 0.5f, 0.5f),
					Vector3DF(-0.5f, 0.5f, 0.5f),
					Vector3DF(0.5f, -0.5f, 0.5f),
					Vector3DF(-0.5f, -0.5f, 0.5f),
				};

				for (int32_t i = 0; i < 8; i++)
				{
					back2.push_back(points[i]);
				}
			}

			auto back = back2;
			auto lvp = lightProjection * lightView;
			{
				for (auto i = 0; i < back2.size(); i++)
				{
					back2[i] = lvp.Transform3D(back2[i]);
				}
			}
			*/
			return;
		}

		auto sinGamma = sqrtf(1.0f - vlC * vlC);

		// Upの計算
		auto upLeft = Vector3DF::Cross(lightDirection, viewDirection );
		auto up = Vector3DF::Cross(upLeft, lightDirection);
		up.Normalize();

		// Eyeの計算
		auto eye = viewPosition;

		// ライトビューの計算
		lightView.SetLookAtRH(eye, eye + lightDirection, up);

		// AABB計算
		for (auto i = 0; i < m_shadowObjectPoints.size(); i++)
		{
			m_shadowObjectPoints[i] = lightView.Transform3D(m_shadowObjectPoints[i]);
		}

		Vector3DF min_, max_;
		calcAABB(m_shadowObjectPoints, max_, min_);


		// 視錐台計算
		const float factor = 1.0f / sinGamma;
		const float z_n = factor * zn;
		const float d = abs(max_.Y - min_.Y);

#if 0
		// New Formula written in ShaderX4
		const float z0 = -z_n;
		const float z1 = -(z_n + d * sinGamma);
		const float n = d / (sqrtf(z1 / z0) - 1.0f);
#else
		// Old Formula written in papers
		const float z_f = z_n + d * sinGamma;
		const float n = (z_n + sqrtf(z_f * z_n)) * factor;
#endif
		const float f = n + d;

		// ライトビューの位置補正
		auto pos = eye - up * (n - zn);
		lightView.SetLookAtRH(pos, (pos + lightDirection), up);

		// Y方向への射影行列を取得
		Matrix44 matPerspective;
		if (GetGraphics()->GetGraphicsType() == GraphicsType::DirectX11)
		{
			// [1,	0,	0,	0]
			// [0,	a,	0,	b]
			// [0,	0,	1,	0]
			// [0,	1,	0,	0]
			// a = f / (f - n);
			// b = - n * f / (f - n)

			matPerspective.SetIndentity();
			matPerspective.Values[1][1] = f / (f - n);
			matPerspective.Values[3][1] = 1.0f;
			matPerspective.Values[1][3] = - n * f / (f - n);
			matPerspective.Values[3][3] = 0.0f;
		}
		else
		{
			// [1,	0,	0,	0]
			// [0,	a,	0,	b]
			// [0,	0,	1,	0]
			// [0,	1,	0,	0]
			// a = (f + n) / (f - n)
			// b = -2.0f * n * f / (f - n)

			matPerspective.SetIndentity();
			matPerspective.Values[1][1] = (f + n) / (f - n);
			matPerspective.Values[3][1] = 1.0f;
			matPerspective.Values[1][3] = -2.0f * n * f / (f - n);
			matPerspective.Values[3][3] = 0.0f;
		}

		// 透視変換後の空間へ変換する
		Matrix44 lightProjection_ = matPerspective * lightView;

		// AABBを計算
		for (auto i = 0; i < m_shadowObjectPointsBack.size(); i++)
		{
			m_shadowObjectPointsBack[i] = lightProjection_.Transform3D(m_shadowObjectPointsBack[i]);
		}

		Vector3DF min__, max__;
		calcAABB(m_shadowObjectPointsBack, max__, min__);

		Matrix44 matCubeClip = calcCubeClipMatrix(max__, min__);

		lightProjection = matCubeClip * matPerspective;

		// デバッグ用コード
		/*
		{
			Vector3DF points[8] = {
				Vector3DF(0.5f, 0.5f, -0.5f),
				Vector3DF(-0.5f, 0.5f, -0.5f),
				Vector3DF(0.5f, -0.5f, -0.5f),
				Vector3DF(-0.5f, -0.5f, -0.5f),
				Vector3DF(0.5f, 0.5f, 0.5f),
				Vector3DF(-0.5f, 0.5f, 0.5f),
				Vector3DF(0.5f, -0.5f, 0.5f),
				Vector3DF(-0.5f, -0.5f, 0.5f),
			};

			for (int32_t i = 0; i < 8; i++)
			{
				back2.push_back(points[i]);
			}
		}

		auto back = back2;
		auto lvp = lightProjection * lightView;
		{
			for (auto i = 0; i < back2.size(); i++)
			{
				back2[i] = lvp.Transform3D(back2[i]);
			}
		}
		*/
	}

	Color RenderedDirectionalLightObject3D::GetColor_RT()
	{
		return m_values_RT.color;
	}

	Vector3DF RenderedDirectionalLightObject3D::GetDirection_RT()
	{
		auto& mat = GetLocalMatrix_RT();
		return Vector3DF(mat.Values[0][2], mat.Values[1][2], mat.Values[2][2]);
	}

	Color RenderedDirectionalLightObject3D::GetColor()
	{
		return m_values.color;
	}

	void RenderedDirectionalLightObject3D::SetColor(Color color)
	{
		m_values.color = color;
	}
}