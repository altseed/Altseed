
#pragma once

#include "ace.Terrain3D.h"

namespace ace
{
	class Terrain3D_Imp
		: public Terrain3D
		, public ReferenceObject
	{
	private:
		const int32_t pixelInGrid = 4;

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

		class Polygon
		{
		public:
			std::shared_ptr<VertexBuffer_Imp> VB;
			std::shared_ptr<IndexBuffer_Imp> IB;

			std::shared_ptr<Texture2D>	ColorTexture;
			std::shared_ptr<Texture2D>	NormalTexture;
			std::shared_ptr<Texture2D>	SpecularTexture;

			std::shared_ptr<Texture2D>	DensityTexture;
		};

		std::vector<std::shared_ptr<Polygon>>	Polygons;

		struct
		{
			std::shared_ptr<VertexBuffer_Imp> VB;
			std::shared_ptr<IndexBuffer_Imp> IB;
			float			GridSize = 0.0f;
			int32_t			GridWidthCount = 0;
			int32_t			GridHeightCount = 0;
		} Proxy;

	private:

		class Surface
		{
		public:
			float	Size;

			astring	ColorPath;
			astring	NormalPath;
			astring	SpecularPath;

			std::shared_ptr<Texture2D>	ColorTexture;
			std::shared_ptr<Texture2D>	NormalTexture;
			std::shared_ptr<Texture2D>	SpecularTexture;
		};

		Graphics*		m_graphics;

		float			gridSize = 0.0f;
		int32_t			gridWidthCount = 0;
		int32_t			gridHeightCount = 0;
	
		std::vector<std::vector<uint8_t>>	surfaces;
		std::map<astring, int32_t>			surfaceNameToIndex;
		std::map<astring, Surface>			surfaceNameToSurface;

		

		bool isChanged = false;

	public:
		Terrain3D_Imp(Graphics* graphics);
		virtual ~Terrain3D_Imp();

	public:

		void Commit();

		void New(float gridSize, int32_t gridWidthCount, int32_t gridHeightCount);

		void AddSurface(const achar* name, float size, const achar* color, const achar* normal, const achar* specular);

		int32_t GetSurfaceIndex(const achar* name);

		void AssignSurfaceWithCircle(int32_t surfaceIndex, float x, float y, float radius, float value, float fallout);

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}