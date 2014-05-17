
#include "ace.CoreFadeOut_Imp.h"
#include "../../Graphics/2D/ace.LayerRenderer.h"

namespace ace
{
	FadeOut_Imp::FadeOut_Imp(LayerRenderer* renderer)
		: m_renderer(renderer)
	{
		SafeAddRef(m_renderer);
	}

	FadeOut_Imp::~FadeOut_Imp()
	{
		SafeRelease(m_renderer);
	}

	void FadeOut_Imp::DrawCurrentScreenToTriangle(
		Vector2DF pos1, Vector2DF uv1, Color col1,
		Vector2DF pos2, Vector2DF uv2, Color col2,
		Vector2DF pos3, Vector2DF uv3, Color col3)
	{
		{
			ace::Vector2DF positions[3];
			ace::Color colors[3];
			ace::Vector2DF uvs[3];

			colors[0] = col1;
			colors[1] = col2;
			colors[2] = col3;

			positions[0] = pos1;
			positions[1] = pos2;
			positions[2] = pos3;

			uvs[0] = uv1;
			uvs[1] = uv2;
			uvs[2] = uv3;

			m_renderer->AddTriangle(positions, colors, uvs);
		}
	}
}