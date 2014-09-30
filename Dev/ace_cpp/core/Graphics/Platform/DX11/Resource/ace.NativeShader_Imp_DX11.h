#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../../Resource/ace.NativeShader_Imp.h"
#include "../ace.Graphics_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class NativeShader_Imp_DX11
		: public NativeShader_Imp
	{
	private:
		ID3D11VertexShader*			m_vertexShader;
		ID3D11PixelShader*			m_pixelShader;
		ID3D11InputLayout*			m_vertexDeclaration;
		ID3D11Buffer*				m_constantBufferToVS;
		ID3D11Buffer*				m_constantBufferToPS;

		struct ConstantLayout
		{
			int32_t					Index;
			std::string				Name;
			eConstantBufferFormat	Type;
			//GLint			ID;
			int32_t			Offset;
			int32_t			Count;
		};

		struct TextureLayout
		{
			int32_t					Index;
			std::string				Name;
			int32_t					ID;
		};

		std::map < std::string, ConstantLayout>	m_vs_constantLayouts;
		std::map < std::string, ConstantLayout>	m_ps_constantLayouts;
		std::vector<ConstantLayout*>	constantLayoutsArray;

		std::map<std::string, TextureLayout> m_vs_textureLayouts;
		std::map<std::string, TextureLayout> m_ps_textureLayouts;
		std::vector<TextureLayout*>	textureLayoutsArray;

		static ID3DBlob* CompileVertexShader(Graphics_Imp_DX11* g, const char* vertexShaderText, const char* vertexShaderFileName, std::vector <Macro>& macro, Log* log);
		static ID3DBlob* CompilePixelShader(Graphics_Imp_DX11* g, const char* vertexShaderText, const char* vertexShaderFileName, std::vector <Macro>& macro, Log* log);

		static void Reflect(void* buf, int32_t bufSize, std::vector<ConstantLayout>& uniformLayouts, int32_t& uniformBufferSize, std::vector<TextureLayout>& textures);
	public:
		NativeShader_Imp_DX11(
			Graphics* graphics, 
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

		void AssignConstantBuffer();

		static NativeShader_Imp_DX11* Create(
			Graphics* graphics,
			const char* vertexShaderText,
			const char* vertexShaderFileName,
			const char* pixelShaderText,
			const char* pixelShaderFileName,
			std::vector <VertexLayout>& layout,
			std::vector <Macro>& macro,
			Log* log);

		ID3D11InputLayout* GetLayout() { return m_vertexDeclaration; };
		ID3D11VertexShader *GetVertexShader() { return m_vertexShader; }
		ID3D11PixelShader* GetPixelShader() { return m_pixelShader; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}