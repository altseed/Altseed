
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
		auto vbuf = m_shader->GetNativeShader()->GetVertexConstantBuffer();
		auto pbuf = m_shader->GetNativeShader()->GetPixelConstantBuffer();

		auto g = (Graphics_Imp*) shader->GetNativeShader()->GetGraphics();
		auto& state = g->GetRenderState()->GetActiveState();

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
						memcpy(&(buf[i.Offset]), v.Data.Float4, sizeof(float) * 2);
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

						state.TextureFilterTypes[i.Offset] = v.Data.TexturePtr->GetFilter();
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