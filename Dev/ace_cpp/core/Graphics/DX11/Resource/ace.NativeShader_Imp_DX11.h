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

		static ID3DBlob* CompileVertexShader(Graphics_Imp_DX11* g, const char* vertexShaderText, const char* vertexShaderFileName, std::vector <Macro>& macro, Log* log);
		static ID3DBlob* CompilePixelShader(Graphics_Imp_DX11* g, const char* vertexShaderText, const char* vertexShaderFileName, std::vector <Macro>& macro, Log* log);

	protected:
		void CreateVertexConstantBufferInternal(int32_t size, std::vector <ConstantBufferInformation>& info);
		void CreatePixelConstantBufferInternal(int32_t size, std::vector <ConstantBufferInformation>& info);

	public:
		NativeShader_Imp_DX11(Graphics* graphics, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11InputLayout* layout);
		virtual ~NativeShader_Imp_DX11();

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