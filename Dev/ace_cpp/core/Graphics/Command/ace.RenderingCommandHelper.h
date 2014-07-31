
#pragma once

#include "../../ace.Core.Base_Imp.h"

namespace ace
{
	class RenderingCommandHelper
	{
	private:
		std::vector<RenderingCommand*>&	commands;
		RenderingCommandFactory*		factory = nullptr;

		int32_t GetConstantBufferID(NativeShader_Imp* shader, const char* name);
		int32_t GetTextureID(NativeShader_Imp* shader, const char* name);

	public:

		struct Texture2DPair
		{
			Texture2D* Ptr;
			TextureFilterType Filter;
			TextureWrapType Wrap;
			Texture2DPair(Texture2D* ptr, TextureFilterType filter, TextureWrapType wrap)
			{
				Ptr = ptr;
				Filter = filter;
				Wrap = wrap;
			}
		};

		template<typename T>
		struct Value
		{
			const char* name;
			const T& value;
			Value(const char* name, const T& value)
				: name(name)
				, value(value)
			{
			}
		};

		template<typename T>
		static Value<T> GenValue(const char* name, const T& value)
		{
			return Value<T>(name, value);
		}

		RenderingCommandHelper(std::vector<RenderingCommand*>& commands, RenderingCommandFactory* factory);

		void Clear(bool isColorTarget, bool isDepthTarget, const Color& color);
		void SetRenderTarget(RenderTexture2D* renderTexture0, RenderTexture2D* renderTexture1, RenderTexture2D* renderTexture2, RenderTexture2D* renderTexture3, DepthBuffer_Imp* depth);
		void SetRenderTarget(RenderTexture2D* renderTexture0, DepthBuffer_Imp* depth);

		void DrawWithPtr(int32_t polyCount, VertexBuffer_Imp* vb, IndexBuffer_Imp* ib, NativeShader_Imp* shader, RenderState rs, ShaderConstantValue* values, int32_t count);

		void Draw(int32_t polyCount, VertexBuffer_Imp* vb, IndexBuffer_Imp* ib, NativeShader_Imp* shader, RenderState rs);


		template<typename T>
		ShaderConstantValue CreateConstantValue(NativeShader_Imp* shader, const char* name, const T& v)
		{
			auto value = ShaderConstantValue(v);
			value.ID = GetConstantBufferID(shader, name);
			return value;
		}

		template<>
		ShaderConstantValue CreateConstantValue(NativeShader_Imp* shader, const char* name, const Texture2DPair& v)
		{
			auto value = ShaderConstantValue(v.Ptr, v.Filter, v.Wrap);
			value.ID = GetTextureID(shader, name);
			return value;
		}

	private:
		template <typename T>
		void setValues_impl(NativeShader_Imp* shader, ShaderConstantValue* values, int32_t index, const T& v)
		{
			values[index] = CreateConstantValue(shader, v.name, v.value);
		}

		template <typename T, typename... Rest>
		void setValues_impl(NativeShader_Imp* shader, ShaderConstantValue* values, int32_t index, const T& v, Rest... rest)
		{
			values[index] = CreateConstantValue(shader, v.name, v.value);
			setValues_impl(shader, values, index + 1, rest...);
		}
	public:

		template<typename... Ts>
		void Draw(int32_t polyCount, VertexBuffer_Imp* vb, IndexBuffer_Imp* ib, NativeShader_Imp* shader, RenderState rs, Ts... args)
		{
			ShaderConstantValue values[sizeof...(Ts)];
			setValues_impl(shader, values, 0, args...);
			DrawWithPtr(polyCount, vb, ib, shader, rs, values, sizeof...(Ts));
		}
	};
}
