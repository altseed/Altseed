
#pragma once

#include "ace.RenderedObject3D.h"
#include <Graphics/ace.Graphics.Common.h>
#include <Math/ace.Vector4DF.h>

namespace ace
{
	class RenderedMassModelObject3DProxy
		: public RenderedObject3DProxy
	{
	private:
		GraphicsDeviceType		deviceType;
		
	public:
		RenderedMassModelObject3DProxy(Graphics* graphics);
		virtual ~RenderedMassModelObject3DProxy();

		MassModel*				ModelPtr = nullptr;

		void Rendering(RenderingCommandHelper* helper, RenderingProperty& prop) override;
		void Draw(RenderingCommandHelper* helper, RenderingProperty& prop, std::vector<RenderedMassModelObject3DProxy*>& proxies, int32_t offset, int32_t count);
	};

	class RenderedMassModelObject3D
		: public RenderedObject3D
	{
		friend class RenderedMassModelObject3DProxy;

	private:

		RenderedMassModelObject3DProxy* proxy = nullptr;

		MassModel*						model = nullptr;

	public:
		RenderedMassModelObject3D(Graphics* graphics);
		virtual ~RenderedMassModelObject3D();

		MassModel* GetModel();
		void SetModel(MassModel* model);

		void Flip(float deltaTime) override;

		RenderedObject3DProxy* GetProxy() const override { return proxy; }

		eRenderedObject3DType GetObjectType() const override { return RENDERED_OBJECT3D_TYPE_MASSOBJECT; }
	};
}