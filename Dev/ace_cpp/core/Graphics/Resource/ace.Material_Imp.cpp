
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Material_Imp.h"
#include "ace.Texture2D.h"
#include "ace.NativeShader_Imp.h"

#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	MaterialCommand::MaterialCommand(NativeShader_Imp* shader, std::map<astring, ShaderConstantValue>& values)
		: shader(shader)
	{
		SafeAddRef(shader);

		for (auto& v : values)
		{
			auto v_ = v.second;
			auto str = ToUtf8String(v.first.c_str());

			if (v.second.ValueType == SHADER_VARIABLE_TYPE_TEXTURE2D)
			{
				SafeAddRef(v.second.Data.Texture2DPtr.Ptr);
				v_.ID = shader->GetTextureID(str.c_str());
			}
			else
			{
				v_.ID = shader->GetConstantBufferID(str.c_str());
			}
			
			constantValues.push_back(v_);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	MaterialCommand::~MaterialCommand()
	{
		for (auto& v : constantValues)
		{
			if (v.ValueType == SHADER_VARIABLE_TYPE_TEXTURE2D)
			{
				SafeRelease(v.Data.Texture2DPtr.Ptr);
			}
		}

		SafeRelease(shader);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Material_Imp::~Material_Imp()
	{
		for (auto& v : m_values)
		{
			if (v.second.ValueType == SHADER_VARIABLE_TYPE_TEXTURE2D)
			{
				SafeRelease(v.second.Data.Texture2DPtr.Ptr);
			}
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	float Material_Imp::GetFloat_Imp(const achar* name)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_FLOAT) return 0.0f;
			return m_values[name].Data.Float4[0];
		}
		return 0.0f;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Material_Imp::SetFloat_Imp(const achar* name, float value)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_FLOAT) return;
			m_values[name].Data.Float4[0] = value;
		}
		else
		{
			m_values[name].ValueType = SHADER_VARIABLE_TYPE_FLOAT;
			m_values[name].Data.Float4[0] = value;
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Vector2DF Material_Imp::GetVector2DF_Imp(const achar* name)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_VECTOR2DF) return Vector2DF(0.0f, 0.0f);
			return Vector2DF(m_values[name].Data.Float4[0], m_values[name].Data.Float4[1]);
		}
		return Vector2DF(0.0f, 0.0f);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Material_Imp::SetVector2DF_Imp(const achar* name, Vector2DF value)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_VECTOR2DF) return;
			m_values[name].Data.Float4[0] = value.X;
			m_values[name].Data.Float4[1] = value.Y;
		}
		else
		{
			m_values[name].ValueType = SHADER_VARIABLE_TYPE_VECTOR2DF;
			m_values[name].Data.Float4[0] = value.X;
			m_values[name].Data.Float4[1] = value.Y;
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Vector3DF Material_Imp::GetVector3DF_Imp(const achar* name)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_VECTOR3DF) return Vector3DF(0.0f, 0.0f, 0.0f);
			return Vector3DF(m_values[name].Data.Float4[0], m_values[name].Data.Float4[1], m_values[name].Data.Float4[2]);
		}
		return Vector3DF(0.0f, 0.0f, 0.0f);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Material_Imp::SetVector3DF_Imp(const achar* name, Vector3DF value)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_VECTOR3DF) return;
			m_values[name].Data.Float4[0] = value.X;
			m_values[name].Data.Float4[1] = value.Y;
			m_values[name].Data.Float4[2] = value.Z;
		}
		else
		{
			m_values[name].ValueType = SHADER_VARIABLE_TYPE_VECTOR3DF;
			m_values[name].Data.Float4[0] = value.X;
			m_values[name].Data.Float4[1] = value.Y;
			m_values[name].Data.Float4[2] = value.Z;
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Vector4DF Material_Imp::GetVector4DF_Imp(const achar* name)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_VECTOR4DF) return Vector4DF(0.0f, 0.0f, 0.0f, 0.0f);
			return Vector4DF(m_values[name].Data.Float4[0], m_values[name].Data.Float4[1], m_values[name].Data.Float4[2], m_values[name].Data.Float4[3]);
		}
		return Vector4DF();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Material_Imp::SetVector4DF_Imp(const achar* name, Vector4DF value)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_VECTOR4DF) return;
			m_values[name].Data.Float4[0] = value.X;
			m_values[name].Data.Float4[1] = value.Y;
			m_values[name].Data.Float4[2] = value.Z;
			m_values[name].Data.Float4[3] = value.W;
		}
		else
		{
			m_values[name].ValueType = SHADER_VARIABLE_TYPE_VECTOR4DF;
			m_values[name].Data.Float4[0] = value.X;
			m_values[name].Data.Float4[1] = value.Y;
			m_values[name].Data.Float4[2] = value.Z;
			m_values[name].Data.Float4[3] = value.W;
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D* Material_Imp::GetTexture2D_(const achar* name)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_TEXTURE2D) return nullptr;
			auto ret = m_values[name].Data.Texture2DPtr.Ptr;
			SafeAddRef(ret);
			return ret;
		}
		return nullptr;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Material_Imp::SetTexture2D_(const achar* name, Texture2D* value)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_TEXTURE2D) return;
			SafeSubstitute(m_values[name].Data.Texture2DPtr.Ptr, value);
		}
		else
		{
			m_values[name].ValueType = SHADER_VARIABLE_TYPE_TEXTURE2D;
			m_values[name].Data.Texture2DPtr.Ptr = nullptr;
			m_values[name].Data.Texture2DPtr.FilterType = TextureFilterType::Nearest;
			m_values[name].Data.Texture2DPtr.WrapType = TextureWrapType::Clamp;

			m_values[name].Data.Texture2DPtr.Ptr = value;
			SafeAddRef(value);
		}
	}

	TextureFilterType Material_Imp::GetTextureFilterType(const achar* name)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_TEXTURE2D) return TextureFilterType::Linear;
			auto ret = m_values[name].Data.Texture2DPtr.FilterType;
			return ret;
		}
		return TextureFilterType::Linear;
	}

	void Material_Imp::SetTextureFilterType(const achar* name, TextureFilterType filter)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_TEXTURE2D) return;
		}
		else
		{
			m_values[name].ValueType = SHADER_VARIABLE_TYPE_TEXTURE2D;
			m_values[name].Data.Texture2DPtr.Ptr = nullptr;
			m_values[name].Data.Texture2DPtr.FilterType = TextureFilterType::Nearest;
			m_values[name].Data.Texture2DPtr.WrapType = TextureWrapType::Clamp;
		}

		m_values[name].Data.Texture2DPtr.FilterType = filter;
	}

	TextureWrapType Material_Imp::GetTextureWrapType(const achar* name)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_TEXTURE2D) return TextureWrapType::Clamp;
			auto ret = m_values[name].Data.Texture2DPtr.WrapType;
			return ret;
		}
		return TextureWrapType::Clamp;
	}

	void Material_Imp::SetTextureWrapType(const achar* name, TextureWrapType wrap)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_TEXTURE2D) return;
		}
		else
		{
			m_values[name].ValueType = SHADER_VARIABLE_TYPE_TEXTURE2D;
			m_values[name].Data.Texture2DPtr.Ptr = nullptr;
			m_values[name].Data.Texture2DPtr.FilterType = TextureFilterType::Nearest;
			m_values[name].Data.Texture2DPtr.WrapType = TextureWrapType::Clamp;
		}

		m_values[name].Data.Texture2DPtr.WrapType = wrap;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}