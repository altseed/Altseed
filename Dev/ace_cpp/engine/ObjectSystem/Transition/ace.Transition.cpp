
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

	bool Transition::IsSceneChanged() const
	{
		return coreTransition->GetIsSceneChanged();
	}

	void Transition::ChangeScene()
	{
		coreTransition->ChangeScene();
	}

	bool Transition::IsFinished() const
	{
		return coreTransition->IsFinished();
	}

	void Transition::Finish()
	{
		coreTransition->Finish();
	}

	void Transition::DrawTriangleWithNextScene(
		Vector2DF pos1, Vector2DF pos2, Vector2DF pos3,
		Color col1, Color col2, Color col3,
		Vector2DF uv1, Vector2DF uv2, Vector2DF uv3)
	{
		coreTransition->DrawTriangleWithNextScene(
			pos1, pos2, pos3,
			col1, col2, col3,
			uv1, uv2, uv3 );
	}

	void Transition::DrawRectangleWithNextScene(
		Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
		Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
		Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV)
	{
		coreTransition->DrawRectangleWithNextScene(
			upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos,
			upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol,
			upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV);
	}

	void Transition::DrawTriangleWithPreviousScene(
		Vector2DF pos1, Vector2DF pos2, Vector2DF pos3,
		Color col1, Color col2, Color col3,
		Vector2DF uv1, Vector2DF uv2, Vector2DF uv3)
	{
		coreTransition->DrawTriangleWithPreviousScene(
			pos1, pos2, pos3,
			col1, col2, col3,
			uv1, uv2, uv3);
	}

	void Transition::DrawRectangleWithPreviousScene(
		Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
		Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
		Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV)
	{
		coreTransition->DrawRectangleWithPreviousScene(
			upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos,
			upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol,
			upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV);
	}
}