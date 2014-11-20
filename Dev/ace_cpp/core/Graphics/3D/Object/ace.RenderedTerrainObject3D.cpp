
#include "ace.RenderedTerrainObject3D.h"

#include "../Resource/ace.Terrain3D_Imp.h"

#include "../Renderer/ace.Renderer3D.h"

#include "../../ace.Graphics_Imp.h"


namespace ace
{
	RenderedTerrainObject3DProxy::RenderedTerrainObject3DProxy(Graphics* graphics)
	{
		auto g = (Graphics_Imp*) graphics;
	}

	RenderedTerrainObject3DProxy::~RenderedTerrainObject3DProxy()
	{
		SafeRelease(TerrainPtr);
	}

	void RenderedTerrainObject3DProxy::Rendering(RenderingCommandHelper* helper, RenderingProperty& prop)
	{
		using h = RenderingCommandHelper;

		if (TerrainPtr == nullptr) return;

		auto terrain = (Terrain3D_Imp*)TerrainPtr;

		for (auto& polygon : terrain->Polygons)
		{

		}
	}

	RenderedTerrainObject3D::RenderedTerrainObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
	{
		proxy = new RenderedTerrainObject3DProxy(graphics);
	}

	RenderedTerrainObject3D::~RenderedTerrainObject3D()
	{
		SafeRelease(terrain);
		SafeRelease(proxy);
	}

	void RenderedTerrainObject3D::SetTerrain(Terrain3D* terrain)
	{
		SafeSubstitute(this->terrain, terrain);
	}

	void RenderedTerrainObject3D::Flip(float deltaTime)
	{
		RenderedObject3D::Flip(deltaTime);

		auto t = (Terrain3D_Imp*) terrain;
		t->Commit();

		SafeSubstitute(proxy->TerrainPtr, terrain);
	}
}