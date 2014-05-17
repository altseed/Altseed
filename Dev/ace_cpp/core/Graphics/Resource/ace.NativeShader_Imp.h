#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include <Math/ace.Vector4DF.h>

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
		static const int32_t TextureCountMax = 8;

	protected:
		uint8_t*	m_vertexConstantBuffer;
		uint8_t*	m_pixelConstantBuffer;
		Texture*	m_textureSlots[TextureCountMax];
		std::string	m_textureNames[TextureCountMax];

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

		void SetVector2DF(const char* name, const Vector2DF& value);

		void SetVector4DF(const char* name, const Vector4DF& value);

		virtual void SetConstantBuffer(const char* name, const void* data, int32_t size) = 0;

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


		template<typename T>
		void CreateVertexConstantBuffer(std::vector<ConstantBufferInformation>& info)
		{
			assert((sizeof(T)%(sizeof(float) * 4)) == 0);
			SafeDeleteArray(m_vertexConstantBuffer);

			m_vertexConstantBuffer = new uint8_t[sizeof(T)];
			CreateVertexConstantBufferInternal(sizeof(T), info);
		}

		void CreateVertexConstantBuffer(int32_t size, std::vector<ConstantBufferInformation>& info)
		{
			SafeDeleteArray(m_vertexConstantBuffer);

			if (size == 0) return;
			m_vertexConstantBuffer = new uint8_t[size];
			CreateVertexConstantBufferInternal(size, info);
		}

		template<typename T>
		void CreatePixelConstantBuffer(std::vector<ConstantBufferInformation>& info)
		{
			SafeDeleteArray(m_pixelConstantBuffer);

			m_pixelConstantBuffer = new uint8_t[sizeof(T)];
			CreatePixelConstantBufferInternal(sizeof(T), info);
		}

		void CreatePixelConstantBuffer(int32_t size, std::vector<ConstantBufferInformation>& info)
		{
			SafeDeleteArray(m_pixelConstantBuffer);

			if (size == 0) return;
			m_pixelConstantBuffer = new uint8_t[size];
			CreatePixelConstantBufferInternal(size, info);
		}

		void SetTexture(const char* name, Texture* texture, int32_t index);

		bool GetTexture(char*& name, Texture*& texture, int32_t index);
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
