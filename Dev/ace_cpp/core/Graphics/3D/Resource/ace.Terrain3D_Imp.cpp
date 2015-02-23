
#include "ace.Terrain3D_Imp.h"
#include "../../ace.Graphics_Imp.h"

#include "../../Resource/ace.VertexBuffer_Imp.h"
#include "../../Resource/ace.IndexBuffer_Imp.h"

#include "../../Resource/ace.Material3D.h"
#include "../../Resource/ace.Shader3D.h"
#include "../../Resource/ace.Shader3D_Imp.h"

namespace ace
{
	#define _NEW 0

	Terrain3D_Imp::Terrain3D_Imp(Graphics* graphics)
		: m_graphics(graphics)
	{
		SafeAddRef(graphics);
	}

	Terrain3D_Imp::~Terrain3D_Imp()
	{
		SafeRelease(m_graphics);
	}

	void Terrain3D_Imp::GenerateTerrainChips()
	{
		for (int32_t y = 0; y < gridHeightCount; y++)
		{
			for (int32_t x = 0; x < gridWidthCount; x++)
			{
				auto& chip = Chips[x + y * gridWidthCount];

				chip.Vertecies.clear();
				chip.Faces.clear();

				auto getHeight = [this](int32_t x_, int32_t y_) -> float
				{
					x_ = Clamp(x_, this->gridWidthCount - 1, 0);
					y_ = Clamp(y_, this->gridHeightCount - 1, 0);

					return this->heights[x_ + y_ * this->gridWidthCount];
				};

				auto v0 = Vector3DF(
					x * gridSize - gridWidthCount * gridSize / 2.0f,
					(getHeight(x - 1, y - 1) + getHeight(x + 0, y - 1) + getHeight(x - 1, y + 0) + getHeight(x + 0, y + 0)) / 4.0f,
					y * gridSize - gridHeightCount * gridSize / 2.0f);

				auto v1 = Vector3DF(
					(x + 1) * gridSize - gridWidthCount * gridSize / 2.0f,
					(getHeight(x + 0, y - 1) + getHeight(x + 1, y - 1) + getHeight(x + 0, y + 0) + getHeight(x + 1, y + 0)) / 4.0f,
					y * gridSize - gridHeightCount * gridSize / 2.0f);

				auto v2 = Vector3DF(
					x * gridSize - gridWidthCount * gridSize / 2.0f,
					(getHeight(x - 1, y + 0) + getHeight(x + 0, y + 0) + getHeight(x - 1, y + 1) + getHeight(x + 0, y + 1)) / 4.0f,
					(y + 1) * gridSize - gridHeightCount * gridSize / 2.0f);

				auto v3 = Vector3DF(
					(x + 1) * gridSize - gridWidthCount * gridSize / 2.0f,
					(getHeight(x + 0, y + 0) + getHeight(x + 1, y + 0) + getHeight(x + 0, y + 1) + getHeight(x + 1, y + 1)) / 4.0f,
					(y + 1) * gridSize - gridHeightCount * gridSize / 2.0f);

				chip.Vertecies.push_back(v0);
				chip.Vertecies.push_back(v1);
				chip.Vertecies.push_back(v2);
				chip.Vertecies.push_back(v3);

				ChipFace f1;
				f1.Indexes[0] = 0;
				f1.Indexes[1] = 1;
				f1.Indexes[2] = 3;

				ChipFace f2;
				f2.Indexes[0] = 0;
				f2.Indexes[1] = 3;
				f2.Indexes[2] = 2;

				chip.Faces.push_back(f1);
				chip.Faces.push_back(f2);
			}
		}

		for (int32_t y = 0; y < gridHeightCount; y++)
		{
			for (int32_t x = 0; x < gridWidthCount; x++)
			{
				auto& chip = Chips[x + y * gridWidthCount];

				for (size_t i = 0; i < chip.Faces.size(); i++)
				{
					auto& face = chip.Faces[i];

					auto normal = Vector3DF::Cross(
						(chip.Vertecies[face.Indexes[2]] - chip.Vertecies[face.Indexes[0]]),
						(chip.Vertecies[face.Indexes[1]] - chip.Vertecies[face.Indexes[0]]));

					normal.Normalize();

					face.Normal = normal;

					auto tangent = Vector3DF::Cross(normal, Vector3DF(0, 0, 1));
					tangent.Normalize();

					face.Binormal = Vector3DF::Cross(tangent, normal);
					face.Binormal.Normalize();
				}
			}
		}
	}

	void Terrain3D_Imp::GenerateTerrainMesh(int32_t chip_x, int32_t chip_y, int32_t chip_width, int32_t chip_height, std::vector<Vertex>& vertices, std::vector<Face>& faces)
	{
		std::vector<Vector3DF> chipVertices;
		std::vector<ChipFace> chipFaces;
		std::map<Vector3DF, int32_t> chipVertexPositionToVertexIndexes;
		std::map<Vector3DF, std::vector<int32_t>> chipVertexPositionToFaceIndexes;

		auto cx_min = Clamp(chip_x - 1, gridWidthCount - 1, 0);
		auto cy_min = Clamp(chip_y - 1, gridHeightCount - 1, 0);
		auto cx_max = Clamp(chip_x + chip_width + 1, gridWidthCount - 1, 0);
		auto cy_max = Clamp(chip_y + chip_height + 1, gridHeightCount - 1, 0);

		for (size_t y = cy_min; y < cy_max; y++)
		{
			for (size_t x = cx_min; x < cx_max; x++)
			{
				auto indexOffset = chipVertices.size();

				auto& chip = Chips[x + y * gridWidthCount];

				for (auto& v : chip.Vertecies)
				{
					auto ind = chipVertexPositionToFaceIndexes.find(v);
					if (ind == chipVertexPositionToFaceIndexes.end())
					{
						chipVertices.push_back(v);
						chipVertexPositionToVertexIndexes[v] = (int32_t) (chipVertices.size() - 1);
						chipVertexPositionToFaceIndexes[v] = std::vector<int32_t>();
					}
				}

				for (auto& f : chip.Faces)
				{
					auto f_ = f;
					for (size_t i = 0; i < 3; i++)
					{
						auto v = chip.Vertecies[f.Indexes[i]];
						f_.Indexes[i] = chipVertexPositionToVertexIndexes[v];
						chipVertexPositionToFaceIndexes[v].push_back(faces.size());
					}

					chipFaces.push_back(f_);
				}
			}
		}

		vertices.clear();
		faces.clear();

		for (size_t i = 0; i < chipVertices.size(); i++)
		{
			auto v = chipVertices[i];
			Vertex cv;

			cv.Position = v;

			Vector3DF normal;
			Vector3DF binormal;
			for (auto ind : chipVertexPositionToFaceIndexes[v])
			{
				auto f = chipFaces[ind];
				normal += f.Normal;
				binormal += f.Binormal;
			}

			normal /= (float) chipVertexPositionToFaceIndexes[v].size();
			binormal /= (float) chipVertexPositionToFaceIndexes[v].size();

			cv.Normal = normal;
			cv.Binormal = binormal;

			vertices.push_back(cv);
		}

		for (size_t i = 0; i < chipFaces.size(); i++)
		{
			auto f = chipFaces[i];
			Face face;
			face.Index1 = f.Indexes[0];
			face.Index2 = f.Indexes[1];
			face.Index3 = f.Indexes[2];
			faces.push_back(face);
		}

		std::map<int32_t, int32_t> indToNewInd;

		auto tempV = vertices;
		vertices.clear();
		
		for (size_t i = 0; i < tempV.size(); i++)
		{
			auto v = tempV[i];

			if (v.Position.X < chip_x * gridSize - gridWidthCount * gridSize / 2.0f ||
				v.Position.X >(chip_x + chip_width) * gridSize - gridWidthCount * gridSize / 2.0f ||
				v.Position.Z < chip_y * gridSize - gridHeightCount * gridSize / 2.0f ||
				v.Position.Z >(chip_y + chip_height) * gridSize - gridHeightCount * gridSize / 2.0f)
			{
			}
			else
			{
				indToNewInd[i] = vertices.size();
				vertices.push_back(v);
			}
		}

		auto tempF = faces;
		faces.clear();

		for (size_t i = 0; i < tempF.size(); i++)
		{
			auto f = tempF[i];

			if (indToNewInd.count(f.Index1) > 0)
			{
				f.Index1 = indToNewInd[f.Index1];
			}
			else
			{
				continue;
			}

			if (indToNewInd.count(f.Index2) > 0)
			{
				f.Index2 = indToNewInd[f.Index2];
			}
			else
			{
				continue;
			}

			if (indToNewInd.count(f.Index3) > 0)
			{
				f.Index3 = indToNewInd[f.Index3];
			}
			else
			{
				continue;
			}

			faces.push_back(f);
		}
	}

	bool Terrain3D_Imp::Commit()
	{
		if (!isChanged) return false;
		isChanged = false;

#if _NEW
		GenerateTerrainChips();
#endif

		auto g = (Graphics_Imp*) m_graphics;

		Proxy.GridWidthCount = gridWidthCount;
		Proxy.GridHeightCount = gridHeightCount;
		Proxy.GridSize = gridSize;
		Proxy.Material_ = material_;

		Proxy.Surfaces.clear();
		for (auto& surface : surfaceNameToSurface)
		{
			SurfaceProxy p;

			p.ColorTexture = surface.second.ColorTexture;
			p.NormalTexture = surface.second.NormalTexture;
			p.MetalnessTexture = surface.second.MetalnessTexture;

			auto ind = surfaceNameToIndex[surface.first];
			auto& surface_ = surfaces[ind];

			p.DensityTexture = g->CreateEmptyTexture2D(
				gridWidthCount * pixelInGrid,
				gridHeightCount * pixelInGrid,
				TextureFormat::R8_UNORM);

			TextureLockInfomation info;
			if (p.DensityTexture->Lock(info))
			{
				memcpy(info.Pixels, surface_.data(), (gridWidthCount * pixelInGrid) * (gridHeightCount * pixelInGrid));
				p.DensityTexture->Unlock();
			}
			
			Proxy.Surfaces.push_back(p);
		}

		if (Proxy.GridWidthCount == 0) return true;
		if (Proxy.GridHeightCount == 0) return true;


		const int32_t ClusterCount = 16;

		Proxy.ClusterWidthCount = Proxy.GridWidthCount / ClusterCount;
		Proxy.ClusterHeightCount = Proxy.GridHeightCount / ClusterCount;
		if (Proxy.ClusterWidthCount * ClusterCount != Proxy.GridWidthCount) Proxy.ClusterWidthCount++;
		if (Proxy.ClusterHeightCount * ClusterCount != Proxy.GridHeightCount) Proxy.ClusterHeightCount++;

		Proxy.Clusters.resize(Proxy.ClusterWidthCount*Proxy.ClusterHeightCount);

		for (auto c = 0; c < Proxy.ClusterWidthCount*Proxy.ClusterHeightCount; c++)
		{
			Proxy.Clusters[c] = std::make_shared<ClusterProxy>();
		}

		for (auto cy = 0; cy < Proxy.ClusterHeightCount; cy++)
		{
			for (auto cx = 0; cx < Proxy.ClusterWidthCount; cx++)
			{
				auto& cluster = Proxy.Clusters[cx + cy * Proxy.ClusterWidthCount];

				auto xoffset = cx * ClusterCount;
				auto yoffset = cy * ClusterCount;
				auto width = Min(ClusterCount, Proxy.GridWidthCount - xoffset);
				auto height = Min(ClusterCount, Proxy.GridHeightCount - yoffset);

#if _NEW
				std::vector<Vertex> vs;
				std::vector<Face> fs;
				GenerateTerrainMesh(xoffset, yoffset, width, height, vs, fs);
#endif

				cluster->Size.X = width * gridSize;
				cluster->Size.Z = height * gridSize;

				cluster->Center.X = (xoffset + width / 2) * gridSize - gridWidthCount * gridSize / 2.0f;
				cluster->Center.Z = (yoffset + height / 2) * gridSize - gridHeightCount * gridSize / 2.0f;

				// 下地
#if _NEW
				cluster->Black.VB = g->CreateVertexBuffer_Imp(sizeof(Vertex), vs.size(), false);
				cluster->Black.IB = g->CreateIndexBuffer_Imp(fs.size() * 3, false, true);

				{
					cluster->Black.VB->Lock();
					auto buf = cluster->Black.VB->GetBuffer<Vertex>(vs.size());
					for (auto i = 0; i < vs.size(); i++)
					{
						Vertex v = vs[i];
						v.VColor = Color(0, 0, 0, 255);
						buf[i] = v;
					}

					cluster->Black.VB->Unlock();
				}

				{
					cluster->Black.IB->Lock();
					auto buf = cluster->Black.IB->GetBuffer<int32_t>(fs.size() * 3);
					for (auto i = 0; i < fs.size(); i++)
					{
						buf[i * 3 + 0] = fs[i].Index1;
						buf[i * 3 + 1] = fs[i].Index2;
						buf[i * 3 + 2] = fs[i].Index3;
					}
					cluster->Black.IB->Unlock();
				}
#else
				cluster->Black.VB = g->CreateVertexBuffer_Imp(sizeof(Vertex), (width + 1) * (height + 1), false);
				cluster->Black.IB = g->CreateIndexBuffer_Imp((width) * (height) * 2 * 3, false, true);

				{
					cluster->Black.VB->Lock();
					auto buf = cluster->Black.VB->GetBuffer<Vertex>((width + 1) * (height + 1));
					for (auto y_ = 0; y_ < height + 1; y_++)
					{
						for (auto x_ = 0; x_ < width + 1; x_++)
						{
							auto x = x_ + xoffset;
							auto y = y_ + yoffset;

							Vertex v;

							v.Position.X = (x - (gridWidthCount + 1) / 2) * gridSize;
							v.Position.Y = 0;
							v.Position.Z = (y - (gridHeightCount + 1) / 2) * gridSize;

							v.Normal.X = 0.0f;
							v.Normal.Y = 1.0f;
							v.Normal.Z = 0.0f;

							v.Binormal.X = 0.0f;
							v.Binormal.Y = 0.0f;
							v.Binormal.Z = 1.0f;

							v.VColor = Color(0, 0, 0, 255);

							buf[x_ + y_ * (width + 1)] = v;
						}
					}

					cluster->Black.VB->Unlock();
				}

				{
					cluster->Black.IB->Lock();
					auto buf = cluster->Black.IB->GetBuffer<int32_t>((width) * (height) * 2 * 3);
					for (auto y_ = 0; y_ < height; y_++)
					{
						for (auto x_ = 0; x_ < width; x_++)
						{
							auto x = x_;
							auto y = y_;

							auto w = width + 1;

							buf[(x_ + y_ * width) * 6 + 0] = (x) +(y) * w;
							buf[(x_ + y_ * width) * 6 + 1] = (x + 1) + (y) * w;
							buf[(x_ + y_ * width) * 6 + 2] = (x + 1) + (y + 1) * w;
							buf[(x_ + y_ * width) * 6 + 3] = (x) +(y) * w;
							buf[(x_ + y_ * width) * 6 + 4] = (x + 1) + (y + 1) * w;
							buf[(x_ + y_ * width) * 6 + 5] = (x) +(y + 1) * w;
						}
					}
					cluster->Black.IB->Unlock();
				}
#endif

				// サーフェイス
				int32_t surfaceInd = 0;
				for (auto& surface : surfaceNameToSurface)
				{
					auto ind_ = surfaceNameToIndex[surface.first];
					auto& surface_ = surfaces[ind_];

					// 内容確認
					bool hasPixel = false;
					for (auto y_ = yoffset * pixelInGrid; y_ < yoffset * pixelInGrid + height * pixelInGrid; y_++)
					{
						for (auto x_ = xoffset * pixelInGrid; x_ < xoffset * pixelInGrid + width * pixelInGrid; x_++)
						{
							if (surface_[x_ + y_ * gridWidthCount * pixelInGrid] > 0)
							{
								hasPixel = true;
								goto Exit;
							}
						}
					}
				Exit:;

					if (hasPixel)
					{
#if _NEW
						SurfacePolygon p;
						p.SurfaceIndex = surfaceInd;

						p.VB = g->CreateVertexBuffer_Imp(sizeof(Vertex), vs.size(), false);
						p.IB = g->CreateIndexBuffer_Imp(fs.size() * 3, false, true);

						{
							p.VB->Lock();
							auto buf = p.VB->GetBuffer<Vertex>(vs.size());
							for (auto i = 0; i < vs.size(); i++)
							{
								Vertex v = vs[i];

								v.UV1.X = (v.Position.X + (gridWidthCount * gridSize / 2.0f)) / surface.second.Size;
								v.UV1.Y = (v.Position.Z + (gridHeightCount * gridSize / 2.0f)) / surface.second.Size;

								v.UV2.X = (v.Position.X + (gridWidthCount * gridSize / 2.0f)) / (float) gridSize / (float) (gridWidthCount);
								v.UV2.Y = (v.Position.Z + (gridHeightCount * gridSize / 2.0f)) / (float) gridSize / (float) (gridHeightCount);
							
								v.VColor = Color(255, 255, 255, 255);

								buf[i] = v;
							}

							p.VB->Unlock();
						}

						{
							p.IB->Lock();
							auto buf = p.IB->GetBuffer<int32_t>(fs.size() * 3);
							for (auto i = 0; i < fs.size(); i++)
							{
								buf[i * 3 + 0] = fs[i].Index1;
								buf[i * 3 + 1] = fs[i].Index2;
								buf[i * 3 + 2] = fs[i].Index3;
							}
							p.IB->Unlock();
						}

						cluster->Surfaces.push_back(p);
#else
						
						SurfacePolygon p;
						p.SurfaceIndex = surfaceInd;
						
						p.VB = g->CreateVertexBuffer_Imp(sizeof(Vertex), (width + 1) * (height + 1), false);
						p.IB = g->CreateIndexBuffer_Imp((width) * (height) * 2 * 3, false, true);

						{
							p.VB->Lock();
							auto buf = p.VB->GetBuffer<Vertex>((width + 1) * (height + 1));
							for (auto y_ = 0; y_ < height + 1; y_++)
							{
								for (auto x_ = 0; x_ < width + 1; x_++)
								{
									auto x = x_ + xoffset;
									auto y = y_ + yoffset;

									Vertex v;

									v.Position.X = (x - (gridWidthCount + 1) / 2) * gridSize;
									v.Position.Y = 0.0f;
									v.Position.Z = (y - (gridHeightCount + 1) / 2) * gridSize;

									v.UV1.X = x * gridSize / surface.second.Size;
									v.UV1.Y = y * gridSize / surface.second.Size;

									v.UV2.X = (float) x / (float) (gridWidthCount);
									v.UV2.Y = (float) y / (float) (gridHeightCount);

									v.Normal.X = 0.0f;
									v.Normal.Y = 1.0f;
									v.Normal.Z = 0.0f;

									v.Binormal.X = 0.0f;
									v.Binormal.Y = 0.0f;
									v.Binormal.Z = 1.0f;

									v.VColor = Color(255, 255, 255, 255);

									buf[x_ + y_ * (width + 1)] = v;
								}
							}

							p.VB->Unlock();
						}

						{
							p.IB->Lock();
							auto buf = p.IB->GetBuffer<int32_t>((width) * (height) * 2 * 3);
							for (auto y_ = 0; y_ < height; y_++)
							{
								for (auto x_ = 0; x_ < width; x_++)
								{
									auto x = x_;
									auto y = y_;

									auto w = width + 1;

									buf[(x_ + y_ * width) * 6 + 0] = (x) +(y) * w;
									buf[(x_ + y_ * width) * 6 + 1] = (x + 1) + (y) * w;
									buf[(x_ + y_ * width) * 6 + 2] = (x + 1) + (y + 1) * w;
									buf[(x_ + y_ * width) * 6 + 3] = (x) +(y) * w;
									buf[(x_ + y_ * width) * 6 + 4] = (x + 1) + (y + 1) * w;
									buf[(x_ + y_ * width) * 6 + 5] = (x) +(y + 1) * w;
								}
							}
							p.IB->Unlock();
						}

						cluster->Surfaces.push_back(p);
#endif
					}

					surfaceInd++;
				}
			}
		}

		return true;
	}

	void Terrain3D_Imp::New(float gridSize, int32_t gridWidthCount, int32_t gridHeightCount)
	{
		this->gridWidthCount = gridWidthCount;
		this->gridHeightCount = gridHeightCount;
		this->gridSize = gridSize;

		this->surfaceNameToIndex.clear();
		this->surfaceNameToSurface.clear();
		this->surfaces.clear();

		this->heights.resize(gridWidthCount * gridHeightCount);

		this->Chips.resize(gridWidthCount * gridHeightCount);

		for (size_t i = 0; i < this->heights.size(); i++)
		{
			this->heights[i] = 0.0f;
		}

		isChanged = true;
	}

	void Terrain3D_Imp::AddSurface(const achar* name, float size, const achar* color, const achar* normal, const achar* metalness)
	{
		Surface sf;
		sf.Size = size;

		sf.ColorPath = color;
		sf.NormalPath = normal;
		sf.MetalnessPath = metalness;

		sf.ColorTexture = m_graphics->CreateTexture2D(color);
		sf.NormalTexture = m_graphics->CreateTexture2DAsRawData(normal);
		sf.MetalnessTexture = m_graphics->CreateTexture2D(metalness);

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

		x /= gridSize;
		y /= gridSize;
		radius /= gridSize;

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

	void Terrain3D_Imp::RaiseWithCircle(float x, float y, float radius, float value, float fallout)
	{
		if (fallout > 1.0f) fallout = 1.0f;
		if (fallout < 0.0f) fallout = 0.0f;

		x += gridWidthCount * gridSize / 2;
		y += gridHeightCount * gridSize / 2;

		x /= gridSize;
		y /= gridSize;
		radius /= gridSize;

		for (float y_ = y - radius; y_ < y + radius; y_ += 1.0f)
		{
			for (float x_ = x - radius; x_ < x + radius; x_ += 1.0f)
			{
				int32_t x_ind = (int32_t) x_;
				int32_t y_ind = (int32_t) y_;
				int32_t ind = x_ind + y_ind * (gridWidthCount);

				if (x_ind < 0) continue;
				if (x_ind >= gridWidthCount) continue;
				if (y_ind < 0) continue;
				if (y_ind >= gridHeightCount) continue;

				// ブラシの値を計算
				auto distance = sqrt((x_ - x) * (x_ - x) + (y_ - y) * (y_ - y));

				if (distance > radius) continue;

				auto variation = 0.0f;

				if (distance < radius * (1.0f - fallout))
				{
					variation = value;
				}
				else
				{
					variation = value * (1.0f - (distance - radius * (1.0f - fallout)) / (radius * fallout));

				}

				heights[ind] += variation;
			}
		}

		isChanged = true;
	}
}