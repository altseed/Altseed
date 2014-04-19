#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include "../ace.Graphics_Imp.h"

#include <stack>
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
		@brief	描画方法を設定するクラス
	*/
	class RenderState_Imp
	{
	public:

		static const int32_t TextureCount = 8;

		struct State
		{
			bool								DepthTest;
			bool								DepthWrite;
			eAlphaBlend							AlphaBlend;
			eCullingType						CullingType;
			eTextureFilterType					TextureFilterTypes[TextureCount];
			eTextureWrapType					TextureWrapTypes[TextureCount];

			State();

			void Reset();

			void CopyTo(State& state);
		};

	protected:
		std::stack<State>	m_stateStack;
		State				m_active;
		State				m_next;

	public:
		RenderState_Imp();
		virtual ~RenderState_Imp();

		virtual void Update(bool forced) = 0;

		State& Push();
		void Pop();
		State& GetActiveState();
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}