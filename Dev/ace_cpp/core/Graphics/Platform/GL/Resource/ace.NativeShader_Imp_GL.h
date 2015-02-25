#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../../Resource/ace.NativeShader_Imp.h"
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
			int32_t					Index;
			std::string				Name;

			ConstantBufferFormat	Type;
			GLint			ID;
			int32_t			Offset;
			int32_t			Count;
		};

		struct TextureLayout
		{
			int32_t					Index;
			std::string				Name;
			
			int32_t					ID;
		};

		GLuint m_program;
		std::vector<Layout> m_layout;
		int32_t		m_vertexSize;

		uint8_t*								m_constantBuffer;

		std::map < std::string, ConstantLayout>	m_constantLayouts;
		std::vector<ConstantLayout*>			constantLayoutsArray;
		
		std::map<std::string, TextureLayout> m_textureLayouts;
		std::vector<TextureLayout*>	textureLayoutsArray;

		NativeShader_Imp_GL(
			Graphics* graphics, 
			GLuint program, 
			std::vector<Layout>& layout, 
			int32_t vertexSize,
			std::vector<ConstantLayout>& uniformLayouts, 
			int32_t uniformBufferSize, 
			std::vector<TextureLayout>& textures);

		virtual ~NativeShader_Imp_GL();

		static void Reflect(GLuint program, std::vector<ConstantLayout>& uniformLayouts, int32_t& uniformBufferSize, std::vector<TextureLayout>& textures);
	public:

		int32_t GetConstantBufferID(const char* name) override;
		int32_t GetTextureID(const char* name) override;

		void SetConstantBuffer(const char* name, const void* data, int32_t size) override;
		void SetConstantBuffer(int32_t id, const void* data, int32_t size) override;

		void SetTexture(const char* name, Texture* texture, TextureFilterType filterType, TextureWrapType wrapType) override;
		void SetTexture(int32_t id, Texture* texture, TextureFilterType filterType, TextureWrapType wrapType) override;

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