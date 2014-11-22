
#include "ace.Terrain3D_Imp.h"
#include "../../ace.Graphics_Imp.h"

#include "../../Resource/ace.VertexBuffer_Imp.h"
#include "../../Resource/ace.IndexBuffer_Imp.h"

#include "../../Resource/ace.Material3D.h"
#include "../../Resource/ace.Shader3D.h"
#include "../../Resource/ace.Shader3D_Imp.h"

namespace ace
{
	Terrain3D_Imp::Terrain3D_Imp(Graphics* graphics)
		: m_graphics(graphics)
	{
		SafeAddRef(graphics);
	}

	Terrain3D_Imp::~Terrain3D_Imp()
	{
		SafeRelease(m_graphics);
	}

	void Terrain3D_Imp::Commit()
	{
		if (!isChanged) return;

		auto g = (Graphics_Imp*) m_graphics;

		Proxy.GridWidthCount = gridWidthCount;
		Proxy.GridHeightCount = gridHeightCount;
		Proxy.GridSize = gridSize;
		Proxy.Material_ = material_;

		if (Proxy.GridWidthCount == 0) return;
		if (Proxy.GridHeightCount == 0) return;

		{
			Proxy.VB = g->CreateVertexBuffer_Imp(sizeof(Vertex), (gridWidthCount + 1) * (gridHeightCount + 1), false);
			Proxy.IB = g->CreateIndexBuffer_Imp((gridWidthCount) * (gridHeightCount) * 2 * 3, false, true);

			{
				Proxy.VB->Lock();
				auto buf = Proxy.VB->GetBuffer<Vertex>((gridWidthCount + 1) * (gridHeightCount + 1));
				for (auto y = 0; y < gridHeightCount + 1; y++)
				{
					for (auto x = 0; x < gridWidthCount + 1; x++)
					{
						Vertex v;

						v.Position.X = (x - (gridWidthCount + 1) / 2) * gridSize;
						v.Position.Y = 0.0f;
						v.Position.Z = (y - (gridHeightCount + 1) / 2) * gridSize;

						v.Normal.X = 0.0f;
						v.Normal.Y = 1.0f;
						v.Normal.Z = 0.0f;

						v.Binormal.X = 0.0f;
						v.Binormal.Y = 0.0f;
						v.Binormal.Z = 1.0f;

						v.VColor = Color(0, 0, 0, 255);

						buf[x + y * (gridWidthCount + 1)] = v;
					}
				}

				Proxy.VB->Unlock();
			}

			{
				Proxy.IB->Lock();
				auto buf = Proxy.IB->GetBuffer<int32_t>((gridWidthCount) * (gridHeightCount) * 2 * 3);
				for (auto y = 0; y < gridHeightCount; y++)
				{
					for (auto x = 0; x < gridWidthCount; x++)
					{
						auto w = gridWidthCount + 1;

						buf[(x + y * gridWidthCount) * 6 + 0] = (x) +(y) * w;
						buf[(x + y * gridWidthCount) * 6 + 1] = (x + 1) + (y) * w;
						buf[(x + y * gridWidthCount) * 6 + 2] = (x + 1) + (y + 1) * w;
						buf[(x + y * gridWidthCount) * 6 + 3] = (x) +(y) * w;
						buf[(x + y * gridWidthCount) * 6 + 4] = (x + 1) + (y + 1) * w;
						buf[(x + y * gridWidthCount) * 6 + 5] = (x) +(y + 1) * w;
					}
				}
				Proxy.IB->Unlock();
			}
		}

		Polygons.clear();

		for (auto& kv : surfaceNameToSurface)
		{
			auto ind = surfaceNameToIndex[kv.first];
			auto& surface = surfaces[ind];

			auto polygon = std::make_shared<Polygon>();

			polygon->ColorTexture = kv.second.ColorTexture;
			polygon->NormalTexture = kv.second.NormalTexture;
			polygon->SpecularTexture = kv.second.SpecularTexture;
			polygon->DensityTexture = g->CreateEmptyTexture2D(
				gridWidthCount * pixelInGrid, 
				gridHeightCount * pixelInGrid, 
				TextureFormat::R8_UNORM);

			TextureLockInfomation info;
			if (polygon->DensityTexture->Lock(info))
			{
				memcpy(info.Pixels, surface.data(), (gridWidthCount * pixelInGrid) * (gridHeightCount * pixelInGrid));
				polygon->DensityTexture->Unlock();
			}

			polygon->VB = g->CreateVertexBuffer_Imp(sizeof(Vertex), (gridWidthCount + 1) * (gridHeightCount + 1), false);
			polygon->IB = g->CreateIndexBuffer_Imp((gridWidthCount) * (gridHeightCount) * 2 * 3, false, true);

			{
				polygon->VB->Lock();
				auto buf = polygon->VB->GetBuffer<Vertex>((gridWidthCount + 1) * (gridHeightCount + 1));
				for (auto y = 0; y < gridHeightCount + 1; y++)
				{
					for (auto x = 0; x < gridWidthCount + 1; x++)
					{
						Vertex v;

						v.Position.X = (x - (gridWidthCount + 1) / 2) * gridSize;
						v.Position.Y = 0.0f;
						v.Position.Z = (y - (gridHeightCount + 1) / 2) * gridSize;

						v.UV1.X = x / kv.second.Size;
						v.UV1.Y = y / kv.second.Size;

						v.UV2.X = (float) x / (float) (gridWidthCount);
						v.UV2.Y = (float) y / (float) (gridHeightCount);

						v.Normal.X = 0.0f;
						v.Normal.Y = 1.0f;
						v.Normal.Z = 0.0f;

						v.Binormal.X = 0.0f;
						v.Binormal.Y = 0.0f;
						v.Binormal.Z = 1.0f;

						v.VColor = Color(255, 255, 255, 255);

						buf[x + y * (gridWidthCount+1)] = v;
					}
				}

				polygon->VB->Unlock();
			}

			{
				polygon->IB->Lock();
				auto buf = polygon->IB->GetBuffer<int32_t>((gridWidthCount) * (gridHeightCount) * 2 * 3);
				for (auto y = 0; y < gridHeightCount; y++)
				{
					for (auto x = 0; x < gridWidthCount; x++)
					{
						auto w = gridWidthCount + 1;

						buf[(x + y * gridWidthCount) * 6 + 0] = (x) +(y) * w;
						buf[(x + y * gridWidthCount) * 6 + 1] = (x + 1) + (y) * w;
						buf[(x + y * gridWidthCount) * 6 + 2] = (x + 1) + (y + 1) * w;
						buf[(x + y * gridWidthCount) * 6 + 3] = (x) +(y) * w;
						buf[(x + y * gridWidthCount) * 6 + 4] = (x + 1) + (y + 1) * w;
						buf[(x + y * gridWidthCount) * 6 + 5] = (x) +(y + 1) * w;
					}
				}
				polygon->IB->Unlock();
			}

			Polygons.push_back(polygon);
		}
	}

	void Terrain3D_Imp::New(float gridSize, int32_t gridWidthCount, int32_t gridHeightCount)
	{
		this->gridWidthCount = gridWidthCount;
		this->gridHeightCount = gridHeightCount;
		this->gridSize = gridSize;

		this->surfaceNameToIndex.clear();
		this->surfaceNameToSurface.clear();
		this->surfaces.clear();

		isChanged = true;
	}

	void Terrain3D_Imp::AddSurface(const achar* name, float size, const achar* color, const achar* normal, const achar* specular)
	{
		Surface sf;
		sf.Size = size;

		sf.ColorPath = color;
		sf.NormalPath = normal;
		sf.SpecularPath = specular;

		sf.ColorTexture = m_graphics->CreateTexture2D(color);
		sf.NormalTexture = m_graphics->CreateTexture2DAsRawData(normal);
		sf.SpecularTexture = m_graphics->CreateTexture2D(specular);

		auto it = surfaceNameToIndex.find(name);

		if (it != surfaceNameToIndex.end())
		{
			surfaceNameToSurface[name] = sf;
		}
		else
		{
			surfaceNameToSurface[name] = sf;
			surfaceNameToIndex[name] = surfaces.size();
			surfaces.push_back(std::vector<uint8_t>());
			surfaces[surfaces.size() - 1].resize((gridWidthCount * pixelInGrid) * (gridHeightCount * pixelInGrid));

			if (surfaces.size() == 1)
			{
				for (auto i = 0; i < (gridWidthCount * pixelInGrid) * (gridHeightCount * pixelInGrid); i++)
				{
					surfaces[surfaces.size() - 1][i] = 255;
				}
			}
			else
			{
				for (auto i = 0; i < (gridWidthCount * pixelInGrid) * (gridHeightCount * pixelInGrid); i++)
				{
					surfaces[surfaces.size() - 1][i] = 0;
				}
			}
		}
	}

	int32_t Terrain3D_Imp::GetSurfaceIndex(const achar* name)
	{
		auto it = surfaceNameToIndex.find(name);

		if (it != surfaceNameToIndex.end())
		{
			return it->second;
		}

		return -1;
	}

	void Terrain3D_Imp::AssignSurfaceWithCircle(int32_t surfaceIndex, float x, float y, float radius, float value, float fallout)
	{
		if (surfaceIndex < 0) return;
		if (surfaceIndex >= surfaces.size()) return;

		if (fallout > 1.0f) fallout = 1.0f;
		if (fallout < 0.0f) fallout = 0.0f;

		x += gridWidthCount * gridSize / 2;
		y += gridHeightCount * gridSize / 2;

		x *= pixelInGrid;
		y *= pixelInGrid;
		radius *= pixelInGrid;

		for (float y_ = y - radius; y_ < y + radius; y_ += 1.0f)
		{
			for (float x_ = x - radius; x_ < x + radius; x_ += 1.0f)
			{
				int32_t x_ind = (int32_t) x_;
				int32_t y_ind = (int32_t) y_;
				int32_t ind = x_ind + y_ind * (gridWidthCount * pixelInGrid);

				if (x_ind < 0) continue;
				if (x_ind >= gridWidthCount * pixelInGrid) continue;
				if (y_ind < 0) continue;
				if (y_ind >= gridHeightCount * pixelInGrid) continue;

				// ブラシの値を計算
				auto distance = sqrt((x_ - x) * (x_ - x) + (y_ - y) * (y_ - y));

				if (distance > radius) continue;

				auto variation = 0.0f;

				if (distance < radius * (1.0f-fallout))
				{
					variation = value;
				}
				else
				{
					variation = value * (1.0f - (distance - radius * (1.0f - fallout)) / (radius * fallout));
					
				}
					
				// 変化させる対象以外の合計値を計算
				int32_t sum = 0;
				for (int32_t i = 0; i < surfaces.size(); i++)
				{
					if (i != surfaceIndex) sum += surfaces[i][ind];
				}

				// 値を変化
				auto old_v = (int32_t)surfaces[surfaceIndex][ind];
				auto new_v = old_v + variation;

				if (new_v > 255) new_v = 255;
				if (new_v < 0) new_v = 0;

				auto diff = new_v - old_v;

				auto pert = (sum - diff) / (float) sum;

				// 変化した分他の値を修正
				for (int32_t i = 0; i < surfaces.size(); i++)
				{
					if (i != surfaceIndex) surfaces[i][ind] = surfaces[i][ind] * pert;
				}

				// 他の値の修正から値の変化を適用
				sum = 0;
				for (int32_t i = 0; i < surfaces.size(); i++)
				{
					if (i != surfaceIndex) sum += surfaces[i][ind];
				}

				if (sum > 255) sum = 255;
				if (sum < 0) sum = 0;

				surfaces[surfaceIndex][ind] = 255 - sum;
			}
		}

		isChanged = true;
	}

	void Terrain3D_Imp::SetMaterial(Material3D* material)
	{
		SafeAddRef(material);
		auto t = CreateSharedPtrWithReleaseDLL(material);
		material_ = t;

		if (material_ != nullptr)
		{
			auto shader = (Shader3D_Imp*) (material_->GetShader3D().get());
			shader->CompileTerrain();
		}

		isChanged = true;
	}
}