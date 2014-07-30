#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include <Math/ace.Vector4DF.h>
#include <Math/ace.Matrix44.h>

#include "../ace.Graphics_Imp.h"
#include "../ace.DeviceObject.h"

#include <cstddef>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
		@brief	シェーダー
	*/
	class NativeShader_Imp
		: public DeviceObject
	{
	public:
	protected:
		uint8_t*			m_vertexConstantBuffer;
		uint8_t*			m_pixelConstantBuffer;
		Texture*			m_textureSlots[Graphics_Imp::MaxTextureCount];
		std::string			m_textureNames[Graphics_Imp::MaxTextureCount];
		TextureWrapType		textureWrapTypes[Graphics_Imp::MaxTextureCount];
		TextureFilterType	textureFilterTypes[Graphics_Imp::MaxTextureCount];

		virtual void CreateVertexConstantBufferInternal(int32_t size, std::vector <ConstantBufferInformation>& info)
		{
		}

		virtual void CreatePixelConstantBufferInternal(int32_t size, std::vector <ConstantBufferInformation>& info)
		{
		}

		int32_t GetBufferSize(eConstantBufferFormat type, int32_t count);

		/**
			@note	キャッシュ用キー
		*/
		astring		m_key;

	public:
		NativeShader_Imp(Graphics* graphics);
		virtual ~NativeShader_Imp();

		void SetFloat(const char* name, const float& value);

		void SetVector2DF(const char* name, const Vector2DF& value);

		void SetVector3DF(const char* name, const Vector3DF& value);

		void SetVector4DF(const char* name, const Vector4DF& value);

		void SetMatrix44(const char* name, const Matrix44& value);

		void SetFloat(int32_t id, const float& value);

		void SetVector2DF(int32_t id, const Vector2DF& value);

		void SetVector3DF(int32_t id, const Vector3DF& value);

		void SetVector4DF(int32_t id, const Vector4DF& value);

		void SetMatrix44(int32_t id, const Matrix44& value);

		virtual int32_t GetConstantBufferID(const char* name) = 0;

		virtual int32_t GetTextureID(const char* name) = 0;

		virtual void SetConstantBuffer(const char* name, const void* data, int32_t size) = 0;

		virtual void SetConstantBuffer(int32_t id, const void* data, int32_t size) = 0;

		virtual void SetTexture(const char* name, Texture* texture, TextureFilterType filterType, TextureWrapType wrapType) = 0;

		virtual void SetTexture(int32_t id, Texture* texture, TextureFilterType filterType, TextureWrapType wrapType) = 0;

		virtual void AssignConstantBuffer() = 0;

		const achar* GetKey() { return m_key.c_str(); }
		void SetKey(const achar* key) { m_key = key; }

		template<typename T>
		T& GetVertexConstantBuffer()
		{
			assert(m_vertexConstantBuffer != nullptr);
			return *((T*) m_vertexConstantBuffer);
		}

		uint8_t* GetVertexConstantBuffer()
		{
			return m_vertexConstantBuffer;
		}

		template<typename T>
		T& GetPixelConstantBuffer()
		{
			assert(m_pixelConstantBuffer != nullptr);
			return *((T*) m_pixelConstantBuffer);
		}

		uint8_t* GetPixelConstantBuffer()
		{
			return m_pixelConstantBuffer;
		}

		/**
		@brief	定数バッファを生成する。[廃止予定]
		*/
		template<typename T>
		void CreateVertexConstantBuffer(std::vector<ConstantBufferInformation>& info)
		{
			assert((sizeof(T)%(sizeof(float) * 4)) == 0);
			SafeDeleteArray(m_vertexConstantBuffer);

			m_vertexConstantBuffer = new uint8_t[sizeof(T)];
			CreateVertexConstantBufferInternal(sizeof(T), info);
		}

		/**
		@brief	定数バッファを生成する。[廃止予定]
		*/
		void CreateVertexConstantBuffer(int32_t size, std::vector<ConstantBufferInformation>& info)
		{
			SafeDeleteArray(m_vertexConstantBuffer);

			if (size == 0) return;
			m_vertexConstantBuffer = new uint8_t[size];
			CreateVertexConstantBufferInternal(size, info);
		}

		/**
		@brief	定数バッファを生成する。[廃止予定]
		*/
		template<typename T>
		void CreatePixelConstantBuffer(std::vector<ConstantBufferInformation>& info)
		{
			SafeDeleteArray(m_pixelConstantBuffer);

			m_pixelConstantBuffer = new uint8_t[sizeof(T)];
			CreatePixelConstantBufferInternal(sizeof(T), info);
		}

		/**
			@brief	定数バッファを生成する。[廃止予定]
		*/
		void CreatePixelConstantBuffer(int32_t size, std::vector<ConstantBufferInformation>& info)
		{
			SafeDeleteArray(m_pixelConstantBuffer);

			if (size == 0) return;
			m_pixelConstantBuffer = new uint8_t[size];
			CreatePixelConstantBufferInternal(size, info);
		}

		void SetTexture(const char* name, Texture* texture, TextureFilterType filterType, TextureWrapType wrapType, int32_t index);

		bool GetTexture(char*& name, Texture*& texture, TextureFilterType& filterType, TextureWrapType& wrapType, int32_t index);

		/**
			@brief	ShaderConstantValueの配列から定数を設定する。
			@param	constantValues	配列の先頭ポインタ
			@param	constantValueCount	配列の個数
		*/
		void SetConstantValues(ShaderConstantValue* constantValues, int32_t constantValueCount);
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
