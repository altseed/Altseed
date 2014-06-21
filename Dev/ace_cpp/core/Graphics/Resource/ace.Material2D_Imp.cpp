
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Material2D_Imp.h"
#include "ace.Shader2D_Imp.h"
#include "ace.NativeShader_Imp.h"
#include "ace.RenderState_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Material2DCommand::Material2DCommand(Shader2D_Imp* shader, std::map<astring, Value>& values)
		: MaterialCommand(values)
		, m_shader(shader)
		, m_target(nullptr)
	{
		SafeAddRef(m_shader);
	}

	Material2DCommand::~Material2DCommand()
	{
		SafeRelease(m_shader);
		SafeRelease(m_target);
	}

	void Material2DCommand::SetValueToShader()
	{
		auto& shader = m_shader;

		for (auto& value : m_values)
		{
			auto key = ToUtf8String(value.first.c_str());
			if (value.second.ValueType == SHADER_VARIABLE_TYPE_FLOAT)
			{
				shader->GetNativeShader()->SetFloat(key.c_str(), value.second.Data.Float4[0]);
			}
			else if (value.second.ValueType == SHADER_VARIABLE_TYPE_VECTOR2DF)
			{
				shader->GetNativeShader()->SetVector2DF(key.c_str(), Vector2DF(value.second.Data.Float4[0], value.second.Data.Float4[1]));
			}
			else if (value.second.ValueType == SHADER_VARIABLE_TYPE_VECTOR3DF)
			{
				shader->GetNativeShader()->SetVector3DF(key.c_str(), Vector3DF(value.second.Data.Float4[0], value.second.Data.Float4[1], value.second.Data.Float4[2]));
			}
			else if (value.second.ValueType == SHADER_VARIABLE_TYPE_VECTOR4DF)
			{
				shader->GetNativeShader()->SetVector4DF(key.c_str(), Vector4DF(value.second.Data.Float4[0], value.second.Data.Float4[1], value.second.Data.Float4[2], value.second.Data.Float4[3]));
			}
			else if (value.second.ValueType == SHADER_VARIABLE_TYPE_TEXTURE2D)
			{
				shader->GetNativeShader()->SetTexture(
					key.c_str(),
					value.second.Data.Texture2DPtr.Ptr, 
					value.second.Data.Texture2DPtr.FilterType, 
					value.second.Data.Texture2DPtr.WrapType);
			}
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Shader2D_Imp* Material2DCommand::GetShader()
	{
		return m_shader;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture2D* Material2DCommand::GetTarget()
	{
		return m_target;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Material2DCommand::SetTarget(RenderTexture2D* target)
	{
		SafeSubstitute(m_target, target);
	}

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
					MaterialCommand::Value v;
					v.ValueType = SHADER_VARIABLE_TYPE_FLOAT;
					v.Data.Float4[0] = 0.0f;
					m_values[i.Name] = v;
				}
				else if (i.Type == SHADER_VARIABLE_TYPE_VECTOR2DF)
				{
					MaterialCommand::Value v;
					v.ValueType = SHADER_VARIABLE_TYPE_VECTOR2DF;
					v.Data.Float4[0] = 0.0f;
					v.Data.Float4[1] = 0.0f;
					m_values[i.Name] = v;
				}
				else if (i.Type == SHADER_VARIABLE_TYPE_VECTOR3DF)
				{
					MaterialCommand::Value v;
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
	Shader2D* Material2D_Imp::GetShader2D_()
	{
		return m_shader;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Material2D_Imp::SetShader2D(Shader2D* shader)
	{
		auto s = (Shader2D_Imp*) shader;
		SafeSubstitute(m_shader, s);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	std::shared_ptr <Material2DCommand> Material2D_Imp::GenerateShaderCommand()
	{
		auto command = std::make_shared < Material2DCommand>(m_shader, m_values);
		return command;
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