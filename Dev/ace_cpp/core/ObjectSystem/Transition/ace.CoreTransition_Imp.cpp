
#include "ace.CoreTransition_Imp.h"
#include "../../Graphics/2D/ace.LayerRenderer.h"
#include "../../Graphics/Resource/ace.RenderTexture2D_Imp.h"
#include "../ace.CoreScene_Imp.h"

namespace ace
{
	CoreTransition_Imp::CoreTransition_Imp()
	{
		
	}

	CoreTransition_Imp::~CoreTransition_Imp()
	{
	}

	void CoreTransition_Imp::DrawCache(LayerRenderer* renderer, CoreScene* nextScene, CoreScene* previousScene)
	{
		renderer->ClearCache();

		if (previousScene != nullptr)
		{
			for (auto& t : previousTriangles)
			{
				ace::Vector2DF positions[3];
				ace::Color colors[3];
				ace::Vector2DF uvs[3];

				colors[0] = t.Col1;
				colors[1] = t.Col2;
				colors[2] = t.Col3;

				positions[0] = t.Pos1;
				positions[1] = t.Pos2;
				positions[2] = t.Pos3;

				uvs[0] = t.UV1;
				uvs[1] = t.UV2;
				uvs[2] = t.UV3;

				renderer->AddTriangle(positions, colors, uvs);
			}
			renderer->SetTexture(previousScene->GetBaseTarget());
			renderer->DrawCache();
			renderer->ClearCache();
		}
		

		if (isSceneChanged)
		{
			if (nextScene != nullptr)
			{
				for (auto& t : nextTriangles)
				{
					ace::Vector2DF positions[3];
					ace::Color colors[3];
					ace::Vector2DF uvs[3];

					colors[0] = t.Col1;
					colors[1] = t.Col2;
					colors[2] = t.Col3;

					positions[0] = t.Pos1;
					positions[1] = t.Pos2;
					positions[2] = t.Pos3;

					uvs[0] = t.UV1;
					uvs[1] = t.UV2;
					uvs[2] = t.UV3;

					renderer->AddTriangle(positions, colors, uvs);
				}
				renderer->SetTexture(nextScene->GetBaseTarget());
				renderer->DrawCache();
				renderer->ClearCache();
			}
		}
	}

	void CoreTransition_Imp::ClearCache()
	{
		previousTriangles.clear();
		nextTriangles.clear();
	}

	void CoreTransition_Imp::ChangeScene()
	{
		isSceneChanged = true;
	}

	void CoreTransition_Imp::DrawTriangleWithNextScene(
		Vector2DF pos1, Vector2DF pos2, Vector2DF pos3,
		Color col1, Color col2, Color col3,
		Vector2DF uv1, Vector2DF uv2, Vector2DF uv3)
	{

		Triangle t;
		t.Pos1 = pos1;
		t.UV1 = uv1;
		t.Col1 = col1;
		t.Pos2 = pos2;
		t.UV2 = uv2;
		t.Col2 = col2;
		t.Pos3 = pos3;
		t.UV3 = uv3;
		t.Col3 = col3;

		nextTriangles.push_back(t);
	}

	void CoreTransition_Imp::DrawRectangleWithNextScene(
		Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
		Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
		Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV)
	{
		DrawTriangleWithNextScene(
			upperLeftPos, upperRightPos, lowerRightPos,
			upperLeftCol, upperRightCol, lowerRightCol,
			upperLeftUV, upperRightUV, lowerRightUV);

		DrawTriangleWithNextScene(
			upperLeftPos, lowerRightPos, lowerLeftPos,
			upperLeftCol, lowerRightCol, lowerLeftCol,
			upperLeftUV, lowerRightUV, lowerLeftUV);
	}

	void CoreTransition_Imp::DrawTriangleWithPreviousScene(
		Vector2DF pos1, Vector2DF pos2, Vector2DF pos3,
		Color col1, Color col2, Color col3,
		Vector2DF uv1, Vector2DF uv2, Vector2DF uv3)
	{
		Triangle t;
		t.Pos1 = pos1;
		t.UV1 = uv1;
		t.Col1 = col1;
		t.Pos2 = pos2;
		t.UV2 = uv2;
		t.Col2 = col2;
		t.Pos3 = pos3;
		t.UV3 = uv3;
		t.Col3 = col3;

		previousTriangles.push_back(t);
	}

	void CoreTransition_Imp::DrawRectangleWithPreviousScene(
		Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
		Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
		Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV)
	{
		DrawTriangleWithPreviousScene(
			upperLeftPos, upperRightPos, lowerRightPos,
			upperLeftCol, upperRightCol, lowerRightCol,
			upperLeftUV, upperRightUV, lowerRightUV);

		DrawTriangleWithPreviousScene(
			upperLeftPos, lowerRightPos, lowerLeftPos,
			upperLeftCol, lowerRightCol, lowerLeftCol,
			upperLeftUV, lowerRightUV, lowerLeftUV);
	}
}