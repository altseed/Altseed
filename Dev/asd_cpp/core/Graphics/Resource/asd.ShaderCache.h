
#pragma once

#include "../../asd.Core.Base.h"
#include "../../asd.Core.Base_Imp.h"

namespace asd
{
	class ShaderCache
	{
	private:
		Graphics_Imp*							m_graphics;
		std::map<astring, NativeShader_Imp*>	m_shaders;
	public:
		
		ShaderCache(Graphics_Imp* graphics);
		virtual ~ShaderCache();

		std::shared_ptr<NativeShader_Imp> CreateFromCode(
			const achar* key,
			const char* vertexShaderCode, 
			const char* pixelShaderCode,
			std::vector <VertexLayout>& layout,
			std::vector <Macro>& macro);

		std::shared_ptr<NativeShader_Imp> CreateFromBinary(
			const achar* key,
			const uint8_t* vertexShader,
			int32_t vertexShaderSize,
			const uint8_t* pixelShader,
			int32_t pixelShaderSize,
			std::vector <VertexLayout>& layout);

		/**
			@brief	シェーダーをキャッシュから取り除く
			@param	shader	シェーダー
			@note
			自動的に呼ばれるため、開発者、ユーザーが実行する必要はない。
		*/
		void RemoveShader(NativeShader_Imp* shader);
	};
}
