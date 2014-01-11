
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Material_Imp.h"
#include "ace.Texture2D.h"

#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Material_Imp::~Material_Imp()
	{
		for (auto& v : m_values)
		{
			if (v.second.ValueType == SHADER_VARIABLE_TYPE_TEXTURE2D)
			{
				SafeRelease(v.second.Data.TexturePtr);
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
	Texture2D* Material_Imp::GetTexture2D_Imp(const achar* name)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_TEXTURE2D) return nullptr;
			auto ret = m_values[name].Data.TexturePtr;
			SafeAddRef(ret);
			return ret;
		}
		return nullptr;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Material_Imp::SetTexture2D_Imp(const achar* name, Texture2D* value)
	{
		if (m_values.count(name) > 0)
		{
			if (m_values[name].ValueType != SHADER_VARIABLE_TYPE_TEXTURE2D) return;
			SafeSubstitute(m_values[name].Data.TexturePtr, value);
		}
		else
		{
			m_values[name].ValueType = SHADER_VARIABLE_TYPE_TEXTURE2D;
			m_values[name].Data.TexturePtr = value;
			SafeAddRef(value);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}