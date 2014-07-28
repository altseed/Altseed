
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
	int32_t NativeShader_Imp::GetBufferSize(eConstantBufferFormat type, int32_t count)
	{
		if (type == eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT1)
		{
			return sizeof(float) * 1 * count;
		}
		
		if (type == eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT2)
		{
			return sizeof(float) * 2 * count;
		}

		if (type == eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT3)
		{
			return sizeof(float) * 3 * count;
		}

		if (type == eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT4)
		{
			return sizeof(float) * 4 * count;
		}

		if (type == eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_MATRIX44)
		{
			return sizeof(float) * 16 * count;
		}

		if (type == eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_MATRIX44_ARRAY)
		{
			return sizeof(float) * 16 * count;
		}

		return 0;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	NativeShader_Imp::NativeShader_Imp(Graphics* graphics)
		: DeviceObject(graphics)
		, m_vertexConstantBuffer(nullptr)
		, m_pixelConstantBuffer(nullptr)
	{
		for (int32_t i = 0; i < Graphics_Imp::MaxTextureCount; i++)
		{
			m_textureSlots[i] = nullptr;
			textureFilterTypes[i] = TextureFilterType::Nearest;
			textureWrapTypes[i] = TextureWrapType::Clamp;
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	NativeShader_Imp::~NativeShader_Imp()
	{
		SafeDeleteArray(m_vertexConstantBuffer);
		SafeDeleteArray(m_pixelConstantBuffer);

		for (int32_t i = 0; i < Graphics_Imp::MaxTextureCount; i++)
		{
			auto t = (Texture2D*) m_textureSlots[i];
			SafeRelease(t);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void NativeShader_Imp::SetFloat(const char* name, const float& value)
	{
		SetConstantBuffer(name, &value, sizeof(float));
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void NativeShader_Imp::SetVector2DF(const char* name, const Vector2DF& value)
	{
		SetConstantBuffer(name, &value, sizeof(Vector2DF));
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void NativeShader_Imp::SetVector3DF(const char* name, const Vector3DF& value)
	{
		SetConstantBuffer(name, &value, sizeof(Vector3DF));
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void NativeShader_Imp::SetVector4DF(const char* name, const Vector4DF& value)
	{
		SetConstantBuffer(name, &value, sizeof(Vector4DF));
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void NativeShader_Imp::SetMatrix44(const char* name, const Matrix44& value)
	{
		SetConstantBuffer(name, &value, sizeof(Matrix44));
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void NativeShader_Imp::SetTexture(const char* name, Texture* texture, TextureFilterType filterType, TextureWrapType wrapType, int32_t index)
	{
		if (index >= Graphics_Imp::MaxTextureCount) return;
		
		SafeAddRef(texture);

		{
			auto t = m_textureSlots[index];
			SafeRelease(t);
		}

		{
			auto t = texture;
			m_textureSlots[index] = t;
			m_textureNames[index] = name;
			textureFilterTypes[index] = filterType;
			textureWrapTypes[index] = wrapType;
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool NativeShader_Imp::GetTexture(char*& name, Texture*& texture, TextureFilterType& filterType, TextureWrapType& wrapType, int32_t index)
	{
		if (index >= Graphics_Imp::MaxTextureCount) return false;
		if (m_textureSlots[index] == nullptr) return false;

		name = (char*)m_textureNames[index].c_str();
		texture = m_textureSlots[index];
		filterType = textureFilterTypes[index];
		wrapType = textureWrapTypes[index];
		return true;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}