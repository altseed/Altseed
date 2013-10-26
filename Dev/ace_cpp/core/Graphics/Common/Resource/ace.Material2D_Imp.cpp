
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Material2D_Imp.h"
#include "ace.Shader2D_Imp.h"
#include "ace.NativeShader_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Material2D_Imp::Material2D_Imp(Shader2D_Imp* shader)
		: m_shader(shader)
	{
		SafeAddRef(m_shader);

		auto setValuesTo = [&](decltype(shader->GetVertexVariableProperties())& props) -> void
		{
			for (auto& i : props)
			{
				if (i.Type == SHADER_VARIABLE_TYPE_FLOAT)
				{
					Value v;
					v.ValueType = SHADER_VARIABLE_TYPE_FLOAT;
					v.Data.Float4[0] = 0.0f;
					m_values[i.Name] = v;
				}
				else if (i.Type == SHADER_VARIABLE_TYPE_VECTOR2DF)
				{
					Value v;
					v.ValueType = SHADER_VARIABLE_TYPE_VECTOR2DF;
					v.Data.Float4[0] = 0.0f;
					v.Data.Float4[1] = 0.0f;
					m_values[i.Name] = v;
				}
				else if (i.Type == SHADER_VARIABLE_TYPE_VECTOR3DF)
				{
					Value v;
					v.ValueType = SHADER_VARIABLE_TYPE_VECTOR3DF;
					v.Data.Float4[0] = 0.0f;
					v.Data.Float4[1] = 0.0f;
					v.Data.Float4[2] = 0.0f;
					m_values[i.Name] = v;
				}
			}
		};

		setValuesTo(shader->GetVertexVariableProperties());
		setValuesTo(shader->GetVertexVariableProperties());

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Material2D_Imp::~Material2D_Imp()
	{
		SafeRelease(m_shader);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Shader2D_Imp* Material2D_Imp::GetShader_Imp()
	{
		return m_shader;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Material2D_Imp::SetShader_Imp(Shader2D_Imp* shader)
	{
		SafeSubstitute(m_shader, shader);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Material2D_Imp::SetValuesToShader()
	{
		auto& shader = m_shader;
		auto vbuf = m_shader->GetNativeShader()->GetVertexConstantBuffer();
		auto pbuf = m_shader->GetNativeShader()->GetPixelConstantBuffer();

		auto setValuesTo = [&](uint8_t* buf, decltype(shader->GetVertexVariableProperties())& props) -> void
			{
				for (auto& i : props)
				{
					if (m_values.count(i.Name) > 0)
					{
						auto& v = m_values[i.Name];

						if (i.Type == SHADER_VARIABLE_TYPE_FLOAT &&
							v.ValueType == SHADER_VARIABLE_TYPE_FLOAT)
						{
							memcpy(&(buf[i.Offset]), v.Data.Float4, sizeof(float));
						}
						else if (i.Type == SHADER_VARIABLE_TYPE_VECTOR2DF &&
							v.ValueType == SHADER_VARIABLE_TYPE_VECTOR2DF)
						{
							memcpy(&(buf[i.Offset]), v.Data.Float4, sizeof(float)*2);
						}
						else if (i.Type == SHADER_VARIABLE_TYPE_VECTOR3DF &&
							v.ValueType == SHADER_VARIABLE_TYPE_VECTOR3DF)
						{
							memcpy(&(buf[i.Offset]), v.Data.Float4, sizeof(float) * 3);
						}
						else if (i.Type == SHADER_VARIABLE_TYPE_TEXTURE2D &&
							v.ValueType == SHADER_VARIABLE_TYPE_TEXTURE2D)
						{
							shader->GetNativeShader()->SetTexture(
								ToUtf8String(i.Name.c_str()).c_str(),
								v.Data.TexturePtr,
								i.Offset);
						}
					}
				}
			};

		//setValuesTo(vbuf, shader->GetVertexVariableProperties());
		setValuesTo(pbuf, shader->GetPixelVariableProperties());
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Material2D_Imp* Material2D_Imp::Create(Shader2D_Imp* shader)
	{
		if (shader == nullptr) return nullptr;

		return new Material2D_Imp(shader);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}