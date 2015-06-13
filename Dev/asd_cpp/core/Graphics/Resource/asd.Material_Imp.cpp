
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Material_Imp.h"
#include "asd.Texture2D.h"
#include "asd.NativeShader_Imp.h"

#include "asd.MaterialPropertyBlock_Imp.h"

#include <Math/asd.Vector2DF.h>
#include <Math/asd.Vector3DF.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	MaterialCommand::MaterialCommand(NativeShader_Imp* shader, std::shared_ptr<MaterialPropertyBlock>& block)
		: shader(shader)
	{
		SafeAddRef(shader);

		auto b = (MaterialPropertyBlock_Imp*) block.get();

		b->AddValuesTo(shader, constantValues);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	MaterialCommand::~MaterialCommand()
	{
		SafeRelease(shader);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Material_Imp::Material_Imp()
	{
		MaterialPropertyBlock* block_ = new MaterialPropertyBlock_Imp();
		block = CreateSharedPtrWithReleaseDLL(block_);
	}

	Material_Imp::~Material_Imp()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	float Material_Imp::GetFloat_Imp(const achar* name)
	{
		return block->GetFloat(name);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Material_Imp::SetFloat_Imp(const achar* name, float value)
	{
		block->SetFloat(name, value);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Vector2DF Material_Imp::GetVector2DF_Imp(const achar* name)
	{
		return block->GetVector2DF(name);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Material_Imp::SetVector2DF_Imp(const achar* name, Vector2DF value)
	{
		block->SetVector2DF(name, value);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Vector3DF Material_Imp::GetVector3DF_Imp(const achar* name)
	{
		return block->GetVector3DF(name);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Material_Imp::SetVector3DF_Imp(const achar* name, Vector3DF value)
	{
		block->SetVector3DF(name, value);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Vector4DF Material_Imp::GetVector4DF_Imp(const achar* name)
	{
		return block->GetVector4DF(name);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Material_Imp::SetVector4DF_Imp(const achar* name, Vector4DF value)
	{
		block->SetVector4DF(name, value);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D* Material_Imp::GetTexture2D_(const achar* name)
	{
		return block->GetTexture2D(name).get();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Material_Imp::SetTexture2D_(const achar* name, Texture2D* value)
	{
		block->SetTexture2D(name, value);
	}

	TextureFilterType Material_Imp::GetTextureFilterType(const achar* name)
	{
		return block->GetTextureFilterType(name);
	}

	void Material_Imp::SetTextureFilterType(const achar* name, TextureFilterType filter)
	{
		block->SetTextureFilterType(name, filter);
	}

	TextureWrapType Material_Imp::GetTextureWrapType(const achar* name)
	{
		return block->GetTextureWrapType(name);
	}

	void Material_Imp::SetTextureWrapType(const achar* name, TextureWrapType wrap)
	{
		block->SetTextureWrapType(name, wrap);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}