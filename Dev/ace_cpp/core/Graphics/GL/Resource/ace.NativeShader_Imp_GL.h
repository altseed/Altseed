#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../Common/Resource/ace.NativeShader_Imp.h"
#include "../ace.Graphics_Imp_GL.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class NativeShader_Imp_GL
		: public NativeShader_Imp
	{
	private:
		struct Layout
		{
			GLint		attribute;
			GLenum		type;
			uint16_t	count;
			uint16_t	offset;
			bool		normalized;
		};

		struct ConstantLayout
		{
			std::string				Name;
			eConstantBufferFormat	Type;
			GLint			ID;
			int32_t			Offset;
			int32_t			Count;
		};

		GLuint m_program;
		std::vector<Layout> m_layout;
		int32_t		m_vertexSize;

		std::vector<ConstantLayout>	m_vertexConstantLayouts;
		std::vector<ConstantLayout>	m_pixelConstantLayouts;

		std::map < std::string, ConstantLayout>	m_constantLayouts;

		NativeShader_Imp_GL(
			Graphics* graphics, 
			GLuint program, 
			std::vector<Layout>& layout, 
			int32_t vertexSize,
			std::vector<ConstantLayout>& uniformLayouts, 
			int32_t uniformBufferSize, 
			std::vector<std::string>& textures);

		virtual ~NativeShader_Imp_GL();

		static void Reflect(GLuint program, std::vector<ConstantLayout>& uniformLayouts, int32_t& uniformBufferSize, std::vector<std::string>& textures);

	protected:
		void CreateVertexConstantBufferInternal(int32_t size, std::vector <ConstantBufferInformation>& info);
		void CreatePixelConstantBufferInternal(int32_t size, std::vector <ConstantBufferInformation>& info);

	public:
		void SetConstantBuffer(const char* name, const void* data, int32_t size);

		void AssignConstantBuffer();

		static NativeShader_Imp_GL* Create(
			Graphics* graphics,
			const char* vertexShaderText,
			const char* vertexShaderFileName,
			const char* pixelShaderText,
			const char* pixelShaderFileName,
			std::vector <VertexLayout>& layout,
			std::vector <Macro>& macro,
			Log* log);

		void SetLayout();

		void Disable();

		GLuint GetProgram() { return m_program; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}