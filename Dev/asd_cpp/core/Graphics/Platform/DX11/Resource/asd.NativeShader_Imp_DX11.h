#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../../Resource/asd.NativeShader_Imp.h"
#include "../asd.Graphics_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class NativeShader_Imp_DX11
		: public NativeShader_Imp
	{
	private:
		ar::Shader*					rhi = nullptr;
		ar::ConstantBuffer*			rhiConstantVB = nullptr;
		ar::ConstantBuffer*			rhiConstantPB = nullptr;

		int32_t vs_uniformBufferSize_ = 0;
		int32_t ps_uniformBufferSize_ = 0;

		struct ConstantLayout
		{
			int32_t					InternalIndex;
			std::string				Name;
			ConstantBufferFormat	Type;
			int32_t			Offset;
			int32_t			Count;
		};

		struct TextureLayout
		{
			int32_t					InternalIndex;
			int32_t					Index;
			std::string				Name;
		};

		std::map < std::string, ConstantLayout>	m_vs_constantLayouts;
		std::map < std::string, ConstantLayout>	m_ps_constantLayouts;
		std::vector<ConstantLayout*>	constantLayoutsArray;

		std::map<std::string, TextureLayout> m_vs_textureLayouts;
		std::map<std::string, TextureLayout> m_ps_textureLayouts;
		std::vector<TextureLayout*>	textureLayoutsArray;

	public:
		NativeShader_Imp_DX11(
			Graphics* graphics,
			ar::Shader* rhi,
			ID3D11VertexShader* vertexShader, 
			ID3D11PixelShader* pixelShader, 
			ID3D11InputLayout* layout,
			std::vector<ConstantLayout> vs_uniformLayouts,
			int32_t vs_uniformBufferSize,
			std::vector<TextureLayout> vs_textures,
			std::vector<ConstantLayout> ps_uniformLayouts,
			int32_t ps_uniformBufferSize,
			std::vector<TextureLayout> ps_textures
			);
		virtual ~NativeShader_Imp_DX11();

		int32_t GetConstantBufferID(const char* name) override;
		int32_t GetTextureID(const char* name) override;

		void SetConstantBuffer(const char* name, const void* data, int32_t size) override;
		void SetConstantBuffer(int32_t id, const void* data, int32_t size) override;

		void SetTexture(const char* name, Texture* texture, TextureFilterType filterType, TextureWrapType wrapType) override;
		void SetTexture(int32_t id, Texture* texture, TextureFilterType filterType, TextureWrapType wrapType) override;

		void AssignConstantBuffer() override;

		static NativeShader_Imp_DX11* Create(
			Graphics* graphics,
			const char* vertexShaderText,
			const char* vertexShaderFileName,
			const char* pixelShaderText,
			const char* pixelShaderFileName,
			std::vector <VertexLayout>& layout,
			bool is32Bit,
			std::vector <Macro>& macro,
			Log* log);

		ar::Shader* GetRHI() const { return rhi; }
		ar::ConstantBuffer* GetRHIVertexConstantBuffer() const { return rhiConstantVB; }
		ar::ConstantBuffer* GetRHIPixelConstantBuffer() const { return rhiConstantPB; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}