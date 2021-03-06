﻿
#include "asd.TerrainObject3D.h"

namespace asd
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	TerrainObject3D::TerrainObject3D()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateCoreTerrainObject3D());
		m_commonObject = m_coreObject.get();
	}

	TerrainObject3D::~TerrainObject3D()
	{

	}

	void TerrainObject3D::SetTerrain(std::shared_ptr<Terrain3D> terrain)
	{
		m_coreObject->SetTerrain(terrain.get());
	}

	void TerrainObject3D::SetMaterialPropertyBlock(std::shared_ptr<MaterialPropertyBlock> block)
	{
		m_coreObject->SetMaterialPropertyBlock(block.get());
	}
}