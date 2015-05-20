
#include "ace.Core.Base_Imp.h"

#include "Graphics/Resource/ace.Shader2D.h"
#include "Graphics/Resource/ace.Material2D.h"
#include "Graphics/Resource/ace.CubemapTexture.h"

#include <Math/ace.Matrix44.h>

namespace ace
{
	ShaderConstantValue::ShaderConstantValue()
	{
		ValueType = ShaderVariableType::Unknown;
		Data.Texture2DPtr.Ptr = nullptr;
		ID = -1;
	}

	ShaderConstantValue::ShaderConstantValue(const ShaderConstantValue& value)
	{
		ValueType = value.ValueType;
		Data = value.Data;
		ID = value.ID;

		if (value.ValueType == ShaderVariableType::Texture2D)
		{
			SafeAddRef(Data.Texture2DPtr.Ptr);
		}

		if (value.ValueType == ShaderVariableType::CubemapTexture)
		{
			SafeAddRef(Data.CubemapTexturePtr.Ptr);
		}
	}

	ShaderConstantValue::ShaderConstantValue(const float& value)
	{
		ValueType = ShaderVariableType::Float;
		Data.Float4[0] = value;
	}

	ShaderConstantValue::ShaderConstantValue(const Vector2DF& value)
	{
		ValueType = ShaderVariableType::Vector2DF;
		Data.Float4[0] = value.X;
		Data.Float4[1] = value.Y;
	}

	ShaderConstantValue::ShaderConstantValue(const Vector3DF& value)
	{
		ValueType = ShaderVariableType::Vector3DF;
		Data.Float4[0] = value.X;
		Data.Float4[1] = value.Y;
		Data.Float4[2] = value.Z;
	}

	ShaderConstantValue::ShaderConstantValue(const Vector4DF& value)
	{
		ValueType = ShaderVariableType::Vector4DF;
		Data.Float4[0] = value.X;
		Data.Float4[1] = value.Y;
		Data.Float4[2] = value.Z;
		Data.Float4[3] = value.W;
	}

	ShaderConstantValue::ShaderConstantValue(Vector4DF* value, int32_t count)
	{
		ValueType = ShaderVariableType::Vector4DF_Array;
		Data.Vector4DFArray.Ptr = value;
		Data.Vector4DFArray.Count = count;
	}

	ShaderConstantValue::ShaderConstantValue(const Matrix44& value)
	{
		ValueType = ShaderVariableType::Matrix44;
		memcpy(Data.Mat44, value.Values, sizeof(float) * 16);
	}

	ShaderConstantValue::ShaderConstantValue(Matrix44* value, int32_t count)
	{
		ValueType = ShaderVariableType::Matrix44_Array;
		Data.Mat44Array.Ptr = value;
		Data.Mat44Array.Count = count;
	}

	ShaderConstantValue::ShaderConstantValue(Texture2D* value, TextureFilterType filterType, TextureWrapType wrapType)
	{
		ValueType = ShaderVariableType::Texture2D;
		Data.Texture2DPtr.Ptr = value;
		Data.Texture2DPtr.FilterType = filterType;
		Data.Texture2DPtr.WrapType = wrapType;

		SafeAddRef(Data.Texture2DPtr.Ptr);
	}

	ShaderConstantValue::ShaderConstantValue(CubemapTexture* value, TextureFilterType filterType, TextureWrapType wrapType)
	{
		ValueType = ShaderVariableType::CubemapTexture;
		Data.CubemapTexturePtr.Ptr = value;
		Data.CubemapTexturePtr.FilterType = filterType;
		Data.CubemapTexturePtr.WrapType = wrapType;

		SafeAddRef(Data.CubemapTexturePtr.Ptr);
	}

	ShaderConstantValue::~ShaderConstantValue()
	{
		if (ValueType == ShaderVariableType::Texture2D)
		{
			SafeRelease(Data.Texture2DPtr.Ptr);
		}

		if (ValueType == ShaderVariableType::CubemapTexture)
		{
			SafeRelease(Data.CubemapTexturePtr.Ptr);
		}
	}

	ShaderConstantValue& ShaderConstantValue::operator=(const ShaderConstantValue& value)
	{
		if (value.ValueType == ShaderVariableType::Texture2D)
		{
			SafeAddRef(value.Data.Texture2DPtr.Ptr);
		}

		if (value.ValueType == ShaderVariableType::CubemapTexture)
		{
			SafeAddRef(value.Data.CubemapTexturePtr.Ptr);
		}

		if (ValueType == ShaderVariableType::Texture2D)
		{
			SafeRelease(Data.Texture2DPtr.Ptr);
		}

		if (ValueType == ShaderVariableType::CubemapTexture)
		{
			SafeRelease(Data.CubemapTexturePtr.Ptr);
		}

		ValueType = value.ValueType;
		Data = value.Data;
		ID = value.ID;

		return *this;
	}
}