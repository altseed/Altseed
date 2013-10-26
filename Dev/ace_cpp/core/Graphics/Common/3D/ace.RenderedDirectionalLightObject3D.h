
#pragma once

#include "ace.RenderedObject3D.h"

namespace ace
{
	/**
		@brief	無限遠から注ぐ光源オブジェクトのクラス
		@note
		ローカル座標の(0,0,1)の方向に光を照射する。
	*/
	class RenderedDirectionalLightObject3D
		: public RenderedObject3D
	{
	private:
		struct
		{
			Color		color;
		} m_values;

		struct
		{
			Color		color;

		} m_values_FR;

	public:
		RenderedDirectionalLightObject3D(Graphics* graphics);
		virtual ~RenderedDirectionalLightObject3D();

		void Flip() override;
		void Rendering(RenderingProperty& prop) override;

		Color GetColor_FR();
		void SetColor(Color color);

		Vector3DF GetDirection_FR();

		eRenderedObject3DType GetObjectType() const override { return RENDERED_OBJECT3D_TYPE_DIRECTIONALLIGHT; }
	};
}