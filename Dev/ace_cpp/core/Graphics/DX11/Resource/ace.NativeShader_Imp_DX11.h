#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../Common/Resource/ace.NativeShader_Imp.h"
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
			std::string				Name;
			eConstantBufferFormat	Type;
			//GLint			ID;
			int32_t			Offset;
			int32_t			Count;
		};

		std::map < std::string, ConstantLayout>	m_vs_constantLayouts;
		std::map < std::string, ConstantLayout>	m_ps_constantLayouts;

		static ID3DBlob* CompileVertexShader(Graphics_Imp_DX11* g, const char* vertexShaderText, const char* vertexShaderFileName, std::vector <Macro>& macro, Log* log);
		static ID3DBlob* CompilePixelShader(Graphics_Imp_DX11* g, const char* vertexShaderText, const char* vertexShaderFileName, std::vector <Macro>& macro, Log* log);

		static void Reflect(ID3DBlob* buf, std::vector<ConstantLayout>& uniformLayouts, int32_t& uniformBufferSize, std::vector<std::string>& textures);

	protected:
		void CreateVertexConstantBufferInternal(int32_t size, std::vector <ConstantBufferInformation>& info);
		void CreatePixelConstantBufferInternal(int32_t size, std::vector <ConstantBufferInformation>& info);

	public:
		NativeShader_Imp_DX11(
			Graphics* graphics, 
			ID3D11VertexShader* vertexShader, 
			ID3D11PixelShader* pixelShader, 
			ID3D11InputLayout* layout,
			std::vector<ConstantLayout> vs_uniformLayouts,
			int32_t vs_uniformBufferSize,
			std::vector<std::string> vs_textures,
			std::vector<ConstantLayout> ps_uniformLayouts,
			int32_t ps_uniformBufferSize,
			std::vector<std::string> ps_textures
			);
		virtual ~NativeShader_Imp_DX11();

		void SetConstantBuffer(const char* name, const void* data, int32_t size);

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