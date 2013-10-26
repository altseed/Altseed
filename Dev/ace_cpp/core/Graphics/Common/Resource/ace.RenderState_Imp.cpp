
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.RenderState_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
RenderState_Imp::State::State()
{
	Reset();
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void RenderState_Imp::State::Reset()
{
	DepthTest = false;
	DepthWrite = false;
	AlphaBlend = ALPHA_BLEND_BLEND;
	CullingType = CULLING_DOUBLE;

	TextureFilterTypes[0] = TEXTURE_FILTER_NEAREST;
	TextureFilterTypes[1] = TEXTURE_FILTER_NEAREST;
	TextureFilterTypes[2] = TEXTURE_FILTER_NEAREST;
	TextureFilterTypes[3] = TEXTURE_FILTER_NEAREST;

	TextureWrapTypes[0] = TEXTURE_WRAP_CLAMP;
	TextureWrapTypes[1] = TEXTURE_WRAP_CLAMP;
	TextureWrapTypes[2] = TEXTURE_WRAP_CLAMP;
	TextureWrapTypes[3] = TEXTURE_WRAP_CLAMP;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void RenderState_Imp::State::CopyTo(State& state)
{
	state.DepthTest = DepthTest;
	state.DepthWrite = DepthWrite;
	state.AlphaBlend = AlphaBlend;
	state.CullingType = CullingType;
	state.TextureFilterTypes[0] = TextureFilterTypes[0];
	state.TextureFilterTypes[1] = TextureFilterTypes[1];
	state.TextureFilterTypes[2] = TextureFilterTypes[2];
	state.TextureFilterTypes[3] = TextureFilterTypes[3];

	state.TextureWrapTypes[0] = TextureWrapTypes[0];
	state.TextureWrapTypes[1] = TextureWrapTypes[1];
	state.TextureWrapTypes[2] = TextureWrapTypes[2];
	state.TextureWrapTypes[3] = TextureWrapTypes[3];
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
RenderState_Imp::RenderState_Imp()
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
RenderState_Imp::~RenderState_Imp()
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
RenderState_Imp::State& RenderState_Imp::Push()
{
	State newState;
	m_next.CopyTo(newState);
	m_stateStack.push(m_next);
	m_next = newState;
	return m_next;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void RenderState_Imp::Pop()
{
	assert(!m_stateStack.empty());

	State top = m_stateStack.top();
	m_stateStack.pop();

	m_next = top;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
RenderState_Imp::State& RenderState_Imp::GetActiveState()
{
	return m_next;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
}