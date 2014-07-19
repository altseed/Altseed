
#include "ace.CoreTransition.h"

namespace ace
{
	class CoreTransition_Imp
		: public CoreTransition
		, public ReferenceObject
	{
	private:
		bool		isSceneChanged = false;

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

		bool GetIsSceneChanged() const { return isSceneChanged; }
		void ChangeScene() override;

		void DrawNextScene(
			Vector2DF pos1, Vector2DF uv1, Color col1,
			Vector2DF pos2, Vector2DF uv2, Color col2,
			Vector2DF pos3, Vector2DF uv3, Color col3) override;

		void DrawPreviousScene(
			Vector2DF pos1, Vector2DF uv1, Color col1,
			Vector2DF pos2, Vector2DF uv2, Color col2,
			Vector2DF pos3, Vector2DF uv3, Color col3) override;

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}