
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
	Material2DCommand::Material2DCommand(Shader2D_Imp* shader, std::map<astring, ShaderConstantValue>& values)
		: MaterialCommand(shader->GetNativeShader().get(), values)
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
					m_values[i.Name] = ShaderConstantValue(0.0f);
				}
				else if (i.Type == SHADER_VARIABLE_TYPE_VECTOR2DF)
				{
					m_values[i.Name] = ShaderConstantValue(Vector2DF());
				}
				else if (i.Type == SHADER_VARIABLE_TYPE_VECTOR3DF)
				{
					m_values[i.Name] = ShaderConstantValue(Vector3DF());
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