
#pragma once

#include "ace.Terrain3D.h"

namespace ace
{
	class Terrain3D_Imp
		: public Terrain3D
		, public ReferenceObject
	{
	private:
		Graphics*		m_graphics;

		int32_t									gridWidthCount = 0;
		int32_t									gridHeightCount = 0;
		std::map<astring, std::vector<uint8_t>>	textures;

	public:
		Terrain3D_Imp(Graphics* graphics);
		virtual ~Terrain3D_Imp();

	public:
		
		void New(float gridSize, int32_t gridWidthCount, int32_t gridHeightCount);

		void AddTexture(const achar* name, const achar* color, const achar* normal, const achar* specular);

		int32_t GetTextureIndex(const achar* name);

		void AssignTextureWithCircle(int32_t textureIndex, float x, float y, float radius, float value, float fallout);


		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}