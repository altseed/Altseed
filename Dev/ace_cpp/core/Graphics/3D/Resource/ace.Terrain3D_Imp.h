﻿
#pragma once

#include "ace.Terrain3D.h"

#include <btBulletCollisionCommon.h>

#ifdef _DEBUG
#pragma comment(lib,"Debug/BulletCollision_Debug.lib")
#pragma comment(lib,"Debug/LinearMath_Debug.lib")
#else
#pragma comment(lib,"Release/BulletCollision.lib")
#pragma comment(lib,"Release/LinearMath.lib")
#endif

namespace ace
{
	class Terrain3D_Imp
		: public Terrain3D
		, public ReferenceObject
	{
	private:
		const int32_t pixelInGrid = 8;

	public:
		struct Vertex
		{
			Vector3DF	Position;
			Vector3DF	Normal;
			Vector3DF	Binormal;
			Vector2DF	UV1;
			Vector2DF	UV2;
			Color		VColor;
		};

		struct Face
		{
			int32_t		Index1;
			int32_t		Index2;
			int32_t		Index3;
		};

		struct SurfacePolygon
		{
			int32_t	SurfaceIndex;
			std::shared_ptr<VertexBuffer_Imp> VB;
			std::shared_ptr<IndexBuffer_Imp> IB;
		};

		struct ClusterProxy
		{
			SurfacePolygon Black;
			std::vector<SurfacePolygon>	Surfaces;
			Vector3DF	Center;
			Vector3DF	Size;
		};

		struct SurfaceProxy
		{
			std::shared_ptr<Texture2D>	ColorTexture;
			std::shared_ptr<Texture2D>	NormalTexture;
			std::shared_ptr<Texture2D>	MetalnessTexture;
			std::shared_ptr<Texture2D>	DensityTexture;
		};

		class Polygon
		{
		public:
			std::shared_ptr<VertexBuffer_Imp> VB;
			std::shared_ptr<IndexBuffer_Imp> IB;

			std::shared_ptr<Texture2D>	ColorTexture;
			std::shared_ptr<Texture2D>	NormalTexture;
			std::shared_ptr<Texture2D>	MetalnessTexture;

			std::shared_ptr<Texture2D>	DensityTexture;
		};

		struct
		{
			std::vector<SurfaceProxy>					Surfaces;
			std::vector<std::shared_ptr<ClusterProxy>>	Clusters;


			int32_t			ClusterWidthCount = 0;
			int32_t			ClusterHeightCount = 0;

			float			GridSize = 0.0f;
			int32_t			GridWidthCount = 0;
			int32_t			GridHeightCount = 0;
			std::shared_ptr<Material3D>	Material_;
		} Proxy;

	private:

		class Surface
		{
		public:
			float	Size;

			astring	ColorPath;
			astring	NormalPath;
			astring	MetalnessPath;

			std::shared_ptr<Texture2D>	ColorTexture;
			std::shared_ptr<Texture2D>	NormalTexture;
			std::shared_ptr<Texture2D>	MetalnessTexture;
		};

		Graphics*		m_graphics;

		float			gridSize = 0.0f;
		int32_t			gridWidthCount = 0;
		int32_t			gridHeightCount = 0;
	
		std::vector<std::vector<uint8_t>>	surfaces;
		std::map<astring, int32_t>			surfaceNameToIndex;
		std::map<astring, Surface>			surfaceNameToSurface;

		std::shared_ptr<Material3D>			material_;

		std::vector<float>					heights;

		bool isChanged = true;

		class ChipFace
		{
		public:
			int32_t		Indexes[3];
			Vector3DF	Normal;
			Vector3DF	Binormal;
		};

		class Chip
		{
		public:
			std::vector<Vector3DF>	Vertecies;
			std::vector<ChipFace>	Faces;

			btTriangleMesh*				CollisionMesh;
			btBvhTriangleMeshShape*		CollisionMeshShape;
			btCollisionObject*			CollisionObject;

			Chip();
			virtual ~Chip();

			void GenerateCollision();
		};

		std::vector<Chip>	Chips;

		btCollisionWorld*					collisionWorld = nullptr;
		btDefaultCollisionConfiguration*	collisionConfiguration = nullptr;
		btCollisionDispatcher*				collisionDispatcher = nullptr;
		btAxisSweep3*						collisionOverlappingPairCache = nullptr;

	public:
		Terrain3D_Imp(Graphics* graphics);
		virtual ~Terrain3D_Imp();

		void GenerateTerrainChips();
		void GenerateTerrainMesh(int32_t chip_x, int32_t chip_y, int32_t chip_width, int32_t chip_height, std::vector<Vertex>& vertices, std::vector<Face>& faces);
	public:

		bool Commit();

		void New(float gridSize, int32_t gridWidthCount, int32_t gridHeightCount) override;

		void AddSurface(const achar* name, float size, const achar* color, const achar* normal, const achar* metalness) override;

		int32_t GetSurfaceIndex(const achar* name) override;

		void AssignSurfaceWithCircle(int32_t surfaceIndex, float x, float y, float radius, float value, float fallout) override;

		void SetMaterial(Material3D* material) override;

		void RaiseWithCircle(float x, float y, float radius, float value, float fallout) override;

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}