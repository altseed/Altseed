
#pragma once

#include "asd.Terrain3D.h"

#include <btBulletCollisionCommon.h>

#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib,"x64/Debug/BulletCollision_Debug.lib")
#pragma comment(lib,"x64/Debug/LinearMath_Debug.lib")
#else
#pragma comment(lib,"x64/Release/BulletCollision.lib")
#pragma comment(lib,"x64/Release/LinearMath.lib")
#endif

#else

#ifdef _DEBUG
#pragma comment(lib,"x86/Debug/BulletCollision_Debug.lib")
#pragma comment(lib,"x86/Debug/LinearMath_Debug.lib")
#else
#pragma comment(lib,"x86/Release/BulletCollision.lib")
#pragma comment(lib,"x86/Release/LinearMath.lib")
#endif

#endif

namespace asd
{
	class Terrain3D_Imp
		: public Terrain3D
		, public ReferenceObject
	{
	private:
		const int32_t pixelInGrid = 8;
		const int32_t ClusterCount = 16;

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

			std::shared_ptr<VertexBuffer_Imp> SideVB;
			std::shared_ptr<IndexBuffer_Imp> SideIB;
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

			std::shared_ptr<Texture2D>	SideColorTexture;
			std::shared_ptr<Texture2D>	SideNormalTexture;
			std::shared_ptr<Texture2D>	SideMetalnessTexture;

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

		std::shared_ptr<Texture2D>			sideColorTexture;
		std::shared_ptr<Texture2D>			sideNormalTexture;
		std::shared_ptr<Texture2D>			sideMetalnessTexture;

		std::shared_ptr<Material3D>			material_;

		std::vector<float>					heights;
		std::vector<int32_t>				cliffes;

		bool isMeshChanged = true;
		bool isSurfaceChanged = true;

		class ChipFace
		{
		public:
			int32_t		Indexes[3];
			Vector3DF	Normal;
			Vector3DF	Binormal;

		};

		class ChipVertex
		{
		public:
			Vector3DF	Position;

			// 側面の場合のみ使用
			Vector3DF	OriginalPosition;
			Vector2DF	UV;
			float		ExtendedRate;

			ChipVertex()
				: UV(Vector2DF(FLT_MAX, FLT_MAX))
			{
			}
		};

		class Chip
		{
		public:
			bool IsPlate = false;
			Vector3DF PlatePoints[4];

			std::vector<int32_t> UpperPoints;
			std::vector<int32_t> LowerPoints;

			std::vector<Vector3DF>	UpperVertecies;
			std::vector<ChipFace>	UpperFaces;

			std::vector<Vector3DF>	LowerVertecies;
			std::vector<ChipFace>	LowerFaces;

			std::vector<std::pair<int32_t, int32_t>> Lines;

			std::vector<ChipVertex>	Vertecies;
			std::vector<ChipFace>	Faces;
			std::vector<ChipFace>	SideFaces;

			bool	IsChanged;
			bool	IsMeshGenerated;
			bool	IsCollisionGenerated;
			Chip();
			virtual ~Chip();

			void GenerateTerrainChip(Terrain3D_Imp* terrain, int32_t chip_x, int32_t chip_y);
		};

		class CollisionCluster
		{
			Terrain3D_Imp* terrain;
			int32_t x;
			int32_t y;
			int32_t width;
			int32_t height;

		public:

			btTriangleMesh*				CollisionMesh;
			btBvhTriangleMeshShape*		CollisionMeshShape;
			btCollisionObject*			CollisionObject;

			CollisionCluster(Terrain3D_Imp* terrain, int32_t x, int32_t y, int32_t width, int32_t height);
			virtual ~CollisionCluster();
			void GenerateCollision();
		};

		std::vector<Chip>								Chips;
		std::vector<std::shared_ptr<CollisionCluster>>	collisionClusters;

		btCollisionWorld*					collisionWorld = nullptr;
		btDefaultCollisionConfiguration*	collisionConfiguration = nullptr;
		btCollisionDispatcher*				collisionDispatcher = nullptr;
		btAxisSweep3*						collisionOverlappingPairCache = nullptr;

	public:
		Terrain3D_Imp(Graphics* graphics);
		virtual ~Terrain3D_Imp();

		void GenerateCollision();
		//void GenerateTerrainChip(int32_t chip_x, int32_t chip_y);
		void GenerateTerrainChips();
		void GenerateTerrainMesh(int32_t chip_x, int32_t chip_y, int32_t chip_width, int32_t chip_height, std::vector<Vertex>& vertices, std::vector<Face>& faces);
		void GenerateTerrainSideMesh(int32_t chip_x, int32_t chip_y, int32_t chip_width, int32_t chip_height, std::vector<Vertex>& vertices, std::vector<Face>& faces);

	public:

		bool Commit() override;

		void Fix() override;

		void ClearCollisions() override;

		void New(float gridSize, int32_t gridWidthCount, int32_t gridHeightCount) override;

		void LoadFromMemory(const std::vector<uint8_t>& buffer) override;

		std::vector<uint8_t> SaveToMemory();

		void AddSurface(const achar* name, float size, const achar* color, const achar* normal, const achar* metalness) override;

		int32_t GetSurfaceIndex(const achar* name) override;

		void SetCliffTexture(Texture2D* diffuseTexture, Texture2D* normalTexture, Texture2D* metalnessTexture) override;

		void AssignSurfaceWithCircle(int32_t surfaceIndex, float x, float y, float radius, float value, float fallout) override;

		void SetMaterial(Material3D* material) override;

		void RaiseHeightWithCircle(float x, float y, float radius, float value, float fallout) override;

		void ChangeHeightWithCircle(float x, float y, float radius, float value, float fallout) override;
		
		void SmoothHeightWithCircle(float x, float y, float radius, float value, float fallout) override;

		void ChangeCliffesWithCircle(float x, float y, float radius, int32_t value) override;

		Vector3DF CastRay(const Vector3DF& from, const Vector3DF& to) override;

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}