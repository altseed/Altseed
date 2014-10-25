
#pragma once

#include <Math/ace.Matrix44.h>

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
		Vector4DF* MallocVector4DFArrayBuffer(int32_t count);
		Matrix44* MallocMatrix44ArrayBuffer(int32_t count);

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

		struct CubemapTexturePair
		{
			CubemapTexture* Ptr;
			TextureFilterType Filter;
			TextureWrapType Wrap;
			CubemapTexturePair(CubemapTexture* ptr, TextureFilterType filter, TextureWrapType wrap)
			{
				Ptr = ptr;
				Filter = filter;
				Wrap = wrap;
			}
		};

		template<typename T>
		struct Array
		{
			T* Ptr;
			int32_t Count;
			
			Array(T* ptr, int32_t count)
				: Ptr(ptr)
				, Count(count)
			{
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

		void DrawWithPtr(int32_t polyOffset, int32_t polyCount, VertexBuffer_Imp* vb, IndexBuffer_Imp* ib, NativeShader_Imp* shader, RenderState rs, ShaderConstantValue* values, int32_t count);

		void DrawInstancedWithPtr(int32_t polyCount, int32_t instanceCount, VertexBuffer_Imp* vb, IndexBuffer_Imp* ib, NativeShader_Imp* shader, RenderState rs, ShaderConstantValue* values, int32_t count);

		void Draw(int32_t polyCount, VertexBuffer_Imp* vb, IndexBuffer_Imp* ib, NativeShader_Imp* shader, RenderState rs);

		void DrawEffect(Matrix44 projMat, Matrix44 cameraMat);

		void DrawSprite(Matrix44 projMat, Matrix44 cameraMat);

		template<typename T>
		ShaderConstantValue CreateConstantValue(NativeShader_Imp* shader, const char* name, const T& v)
		{
			auto value = ShaderConstantValue(v);
			value.ID = GetConstantBufferID(shader, name);
			return value;
		}

		//template<>
		ShaderConstantValue CreateConstantValue(NativeShader_Imp* shader, const char* name, const Texture2DPair& v)
		{
			auto value = ShaderConstantValue(v.Ptr, v.Filter, v.Wrap);
			value.ID = GetTextureID(shader, name);
			return value;
		}

		//template<>
		ShaderConstantValue CreateConstantValue(NativeShader_Imp* shader, const char* name, const CubemapTexturePair& v)
		{
			auto value = ShaderConstantValue(v.Ptr, v.Filter, v.Wrap);
			value.ID = GetTextureID(shader, name);
			return value;
		}

		//template<>
		ShaderConstantValue CreateConstantValue(NativeShader_Imp* shader, const char* name, const std::vector<Matrix44>& v)
		{
			if (v.size() == 0)
			{
				Matrix44* buf_ = nullptr;
				auto ret = ShaderConstantValue(buf_, 0);
				ret.ID = -1;
			}

			auto buf = MallocMatrix44ArrayBuffer(v.size());
			memcpy(buf, v.data(), v.size() * sizeof(Matrix44));
			auto value = ShaderConstantValue(buf, v.size());
			value.ID = GetConstantBufferID(shader, name);
			return value;
		}

		//template<>
		ShaderConstantValue CreateConstantValue(NativeShader_Imp* shader, const char* name, const Array<Matrix44>& v)
		{
			if (v.Count == 0)
			{
				Matrix44* buf_ = nullptr;
				auto ret = ShaderConstantValue(buf_, 0);
				ret.ID = -1;
			}

			auto buf = MallocMatrix44ArrayBuffer(v.Count);
			memcpy(buf, v.Ptr, v.Count * sizeof(Matrix44));
			auto value = ShaderConstantValue(buf, v.Count);
			value.ID = GetConstantBufferID(shader, name);
			return value;
		}

		//template<>
		ShaderConstantValue CreateConstantValue(NativeShader_Imp* shader, const char* name, const std::vector<Vector4DF>& v)
		{
			if (v.size() == 0)
			{
				Vector4DF* buf_ = nullptr;
				auto ret = ShaderConstantValue(buf_, 0);
				ret.ID = -1;
			}

			auto buf = MallocVector4DFArrayBuffer(v.size());
			memcpy(buf, v.data(), v.size() * sizeof(Vector4DF));
			auto value = ShaderConstantValue(buf, v.size());
			value.ID = GetConstantBufferID(shader, name);
			return value;
		}

		//template<>
		ShaderConstantValue CreateConstantValue(NativeShader_Imp* shader, const char* name, const Array<Vector4DF>& v)
		{
			if (v.Count == 0)
			{
				Vector4DF* buf_ = nullptr;
				auto ret = ShaderConstantValue(buf_, 0);
				ret.ID = -1;
			}

			auto buf = MallocVector4DFArrayBuffer(v.Count);
			memcpy(buf, v.Ptr, v.Count * sizeof(Vector4DF));
			auto value = ShaderConstantValue(buf, v.Count);
			value.ID = GetConstantBufferID(shader, name);
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

		template<typename... Ts>
		void DrawInstanced(int32_t polyCount, VertexBuffer_Imp* vb, IndexBuffer_Imp* ib, NativeShader_Imp* shader, RenderState rs, Ts... args)
		{
			ShaderConstantValue values[sizeof...(Ts)];
			setValues_impl(shader, values, 0, args...);
			DrawInstancedWithPtr(polyCount, vb, ib, shader, rs, values, sizeof...(Ts));
		}
	};
}
