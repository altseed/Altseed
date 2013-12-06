
#include "ace.Renderer3D.h"
#include "ace.RenderedObject3D.h"
#include "ace.RenderedCameraObject3D.h"
#include "ace.RenderedDirectionalLightObject3D.h"
#include "../ace.Graphics_Imp.h"

namespace ace
{
	template<typename T>
	void AddRefToObjects(std::set<T*>& tset)
	{
		for (auto& o : tset)
		{
			SafeAddRef(o);
		}
	}

	template<typename T>
	void ReleaseObjects(std::set<T*>& tset)
	{
		for (auto& o : tset)
		{
			o->Release();
		}
		tset.clear();
	}

	Renderer3D::Renderer3D(Graphics* graphics)
		: m_graphics(nullptr)
	{
		m_graphics = (Graphics_Imp*) graphics;
		SafeAddRef(m_graphics);
	}

	Renderer3D::~Renderer3D()
	{
		ReleaseObjects(m_objects);
		ReleaseObjects(rendering.objects);

		ReleaseObjects(m_cameraObjects);
		ReleaseObjects(rendering.cameraObjects);

		ReleaseObjects(m_directionalLightObjects);
		ReleaseObjects(rendering.directionalLightObjects);

		SafeRelease(m_graphics);
	}

	void Renderer3D::AddObject(RenderedObject3D* o)
	{
		if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_CAMERA)
		{
			if (m_cameraObjects.count(o) == 0)
			{
				SafeAddRef(o);
				m_cameraObjects.insert(o);
			}
		}
		else if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_DIRECTIONALLIGHT)
		{
			if (m_directionalLightObjects.count(o) == 0)
			{
				SafeAddRef(o);
				m_directionalLightObjects.insert(o);
			}
		}
		else
		{
			if (m_objects.count(o) == 0)
			{
				SafeAddRef(o);
				m_objects.insert(o);
			}
		}
	}

	void Renderer3D::RemoveObject(RenderedObject3D* o)
	{
		if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_CAMERA)
		{
			if (m_cameraObjects.count(o) > 0)
			{
				m_cameraObjects.erase(o);
				SafeRelease(o);
			}
		}
		else if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_DIRECTIONALLIGHT)
		{
			if (m_directionalLightObjects.count(o) > 0)
			{
				m_directionalLightObjects.erase(o);
				SafeRelease(o);
			}
		}
		else
		{
			if (m_objects.count(o) > 0)
			{
				m_objects.erase(o);
				SafeRelease(o);
			}
		}
	}

	void Renderer3D::Flip()
	{
		ReleaseObjects(rendering.objects);
		ReleaseObjects(rendering.cameraObjects);
		ReleaseObjects(rendering.directionalLightObjects);

		rendering.objects.insert(m_objects.begin(), m_objects.end());
		rendering.cameraObjects.insert(m_cameraObjects.begin(), m_cameraObjects.end());
		rendering.directionalLightObjects.insert(m_directionalLightObjects.begin(), m_directionalLightObjects.end());

		AddRefToObjects(rendering.objects);
		AddRefToObjects(rendering.cameraObjects);
		AddRefToObjects(rendering.directionalLightObjects);

		for (auto& o : rendering.objects)
		{
			o->Flip();
		}

		for (auto& o : rendering.cameraObjects)
		{
			o->Flip();
		}

		for (auto& o : rendering.directionalLightObjects)
		{
			o->Flip();
		}
	}

	void Renderer3D::Rendering()
	{
		auto g = m_graphics;

		for (auto& o : rendering.objects)
		{
			o->CalculateMatrix_FR();
		}

		for (auto& o : rendering.cameraObjects)
		{
			o->CalculateMatrix_FR();
		}

		for (auto& o : rendering.directionalLightObjects)
		{
			o->CalculateMatrix_FR();
		}

		RenderingProperty prop;

		// ライトの計算
		{
			if (rendering.directionalLightObjects.size() > 0)
			{
				auto light = (RenderedDirectionalLightObject3D*)(*(rendering.directionalLightObjects.begin()));
				prop.DirectionalLightColor = light->GetColor_FR();
				prop.DirectionalLightDirection = light->GetDirection_FR();
				prop.DirectionalLightDirection.X = -prop.DirectionalLightDirection.X;
				prop.DirectionalLightDirection.Y = -prop.DirectionalLightDirection.Y;
				prop.DirectionalLightDirection.Z = -prop.DirectionalLightDirection.Z;
			}
			else
			{
				prop.DirectionalLightColor = Color(255, 255, 255, 255);
				prop.DirectionalLightDirection = Vector3DF(1.0f, 1.0f, 1.0f);
			}
			Vector3DF::Normal(prop.DirectionalLightDirection, prop.DirectionalLightDirection);
		}

		for (auto& co : rendering.cameraObjects)
		{
			auto c = (RenderedCameraObject3D*) co;

			// ただの3D描画

			//g->SetRenderTarget(c->GetRenderTarget(), c->GetDepthBuffer());

			g->Clear(true, true, ace::Color(0, 0, 0, 255));

			// カメラプロジェクション行列計算
			Matrix44 cameraProjMat;
			ace::Matrix44::Mul(cameraProjMat, c->GetCameraMatrix_FR(), c->GetProjectionMatrix_FR());

			
			prop.CameraProjectionMatrix = cameraProjMat;

			for (auto& o : m_objects)
			{
				o->Rendering(prop);
			}
		}
	}
}