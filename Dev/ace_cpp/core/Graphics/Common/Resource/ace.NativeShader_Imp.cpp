
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.NativeShader_Imp.h"
#include "ace.Texture2D_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	NativeShader_Imp::NativeShader_Imp(Graphics* graphics)
		: DeviceObject(graphics)
		, m_vertexConstantBuffer(nullptr)
		, m_pixelConstantBuffer(nullptr)
	{
		for (int32_t i = 0; i < TextureCountMax; i++)
		{
			m_textureSlots[i] = nullptr;
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	NativeShader_Imp::~NativeShader_Imp()
	{
		SafeDeleteArray(m_vertexConstantBuffer);
		SafeDeleteArray(m_pixelConstantBuffer);

		for (int32_t i = 0; i < TextureCountMax; i++)
		{
			auto t = (Texture2D*) m_textureSlots[i];
			SafeRelease(t);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void NativeShader_Imp::SetTexture(const char* name, Texture* texture, int32_t index)
	{
		if (index >= TextureCountMax) return;
		
		SafeAddRef(texture);

		{
			auto t = m_textureSlots[index];
			SafeRelease(t);
		}

		{
			auto t = texture;
			m_textureSlots[index] = t;
			m_textureNames[index] = name;
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool NativeShader_Imp::GetTexture(char*& name, Texture*& texture, int32_t index)
	{
		if (index >= TextureCountMax) return false;
		if (m_textureSlots[index] == nullptr) return false;

		name = (char*)m_textureNames[index].c_str();
		texture = m_textureSlots[index];
		return true;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}