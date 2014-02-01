
#pragma once

#include <Math/ace.Vector2DI.h>
#include <Math/ace.Vector3DF.h>

#include "ace.RenderedObject3D.h"

namespace ace
{
	class RenderedEffectObject3D
		: public RenderedObject3D
	{
	private:

		struct
		{


		} m_values;

		struct
		{


		} m_values_FR;

		std::vector<Effekseer::Handle>	m_handles;
		Effect*							m_effect;
		Renderer3D*						m_renderer;

	public:
		RenderedEffectObject3D(Graphics* graphics);
		virtual ~RenderedEffectObject3D();

		void Flip() override;
		void Rendering(RenderingProperty& prop) override;

		void SetEffect(Effect* effect);

		void Play();

		void OnAdded(Renderer3D* renderer);

		void OnRemoving(Renderer3D* renderer);

		eRenderedObject3DType GetObjectType() const override { return RENDERED_OBJECT3D_TYPE_EFFECT; }
	};
}