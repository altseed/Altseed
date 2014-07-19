
#include "ace.Transition.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	Transition::Transition()
	{
		coreTransition = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateTransition());
	}

	Transition::~Transition()
	{
	
	}

	void Transition::OnUpdate()
	{
	
	}

	void Transition::ChangeScene()
	{
		coreTransition->ChangeScene();
	}

	void Transition::Finish()
	{
		coreTransition->Finish();
	}

	void Transition::DrawNextScene(
		Vector2DF pos1, Vector2DF uv1, Color col1,
		Vector2DF pos2, Vector2DF uv2, Color col2,
		Vector2DF pos3, Vector2DF uv3, Color col3)
	{
		coreTransition->DrawNextScene(
			pos1, uv1, col1,
			pos2, uv2, col2,
			pos3, uv3, col3);
	}

	void Transition::DrawPreviousScene(
		Vector2DF pos1, Vector2DF uv1, Color col1,
		Vector2DF pos2, Vector2DF uv2, Color col2,
		Vector2DF pos3, Vector2DF uv3, Color col3)
	{
		coreTransition->DrawPreviousScene(
			pos1, uv1, col1,
			pos2, uv2, col2,
			pos3, uv3, col3);
	}
}