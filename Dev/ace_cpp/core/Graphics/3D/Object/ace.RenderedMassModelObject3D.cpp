
#include "ace.RenderedMassModelObject3D.h"
#include "../Resource/ace.MassModel.h"

#include "../../ace.Graphics_Imp.h"

namespace ace
{
	RenderedMassModelObject3DProxy::RenderedMassModelObject3DProxy(Graphics* graphics)
	{
		deviceType = graphics->GetGraphicsDeviceType();
	}

	RenderedMassModelObject3DProxy::~RenderedMassModelObject3DProxy()
	{
		SafeRelease(ModelPtr);
	}

	void RenderedMassModelObject3DProxy::Rendering(RenderingCommandHelper* helper, RenderingProperty& prop)
	{

	}

	void RenderedMassModelObject3DProxy::Draw(RenderingCommandHelper* helper, RenderingProperty& prop, std::vector<RenderedMassModelObject3DProxy*>& proxies, int32_t offset, int32_t count)
	{

	}

	RenderedMassModelObject3D::RenderedMassModelObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
	{
		proxy = new RenderedMassModelObject3DProxy(graphics);
	}

	RenderedMassModelObject3D::~RenderedMassModelObject3D()
	{
		SafeRelease(model);
		SafeRelease(proxy);
	}

	MassModel* RenderedMassModelObject3D::GetModel()
	{
		return model;
	}

	void RenderedMassModelObject3D::SetModel(MassModel* model)
	{
		SafeSubstitute(this->model, model);
	}

	void RenderedMassModelObject3D::Flip(float deltaTime)
	{
		SafeSubstitute(proxy->ModelPtr, model);
	}
}