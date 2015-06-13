
#include "asd.CoreTransition.h"

namespace asd
{
	class CoreTransition_Imp
		: public CoreTransition
		, public ReferenceObject
	{
	private:
		bool		isSceneChanged = false;
		bool		isFinished = false;

		struct Triangle
		{
			Vector2DF	Pos1, Pos2, Pos3;
			Vector2DF	UV1, UV2, UV3;
			Color		Col1, Col2, Col3;
		};

		std::vector<Triangle>	nextTriangles;
		std::vector<Triangle>	previousTriangles;

	public:
		CoreTransition_Imp();
		virtual ~CoreTransition_Imp();

		void DrawCache(LayerRenderer* renderer, CoreScene* nextScene, CoreScene* previousScene);
		void ClearCache();

		bool GetIsFinished() const override { return isFinished; }
		void Finish() override { isFinished = true; }

		bool GetIsSceneChanged() const override { return isSceneChanged; }
		void ChangeScene() override;

		void DrawTriangleWithNextScene(
			Vector2DF pos1, Vector2DF pos2, Vector2DF pos3,
			Color col1, Color col2, Color col3,
			Vector2DF uv1, Vector2DF uv2, Vector2DF uv3) override;

		void DrawRectangleWithNextScene(
			Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV) override;

		void DrawTriangleWithPreviousScene(
			Vector2DF pos1, Vector2DF pos2, Vector2DF pos3,
			Color col1, Color col2, Color col3,
			Vector2DF uv1, Vector2DF uv2, Vector2DF uv3) override;

		void DrawRectangleWithPreviousScene(
			Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV) override;

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}