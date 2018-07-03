
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <asd.common.Base.h>

#include <Math/asd.Vector2DF.h>
#include <Math/asd.Vector3DF.h>
#include <Math/asd.Vector2DI.h>
#include <Graphics/asd.Color.h>

#include "../../asd.Core.Base.h"
#include "../../asd.ReferenceObject.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	/**
	@brief	ポストエフェクトを描画するためのクラス
	*/
	class PostEffectRenderer
		: public ReferenceObject
	{
		struct Vertex
		{
			Vector3DF	Pos;
			Vector2DF	UV;
			Vector2DF	UVSub1;
			Color		Color_;

			Vertex() {}
			Vertex(asd::Vector3DF pos, asd::Vector2DF uv)
			{
				Pos = pos;
				UV = uv;
				Color_ = Color(255, 255, 255, 255);
			}
		};

	private:
		Graphics_Imp*		m_graphics;
		std::shared_ptr <VertexBuffer_Imp>	m_vertexBuffer;
		std::shared_ptr <IndexBuffer_Imp>	m_indexBuffer;

		PostEffectRenderer(
			Graphics* graphics,
			std::shared_ptr<VertexBuffer_Imp> vertexBuffer,
			std::shared_ptr<IndexBuffer_Imp> indexBuffer);
		virtual ~PostEffectRenderer();

	public:
		static PostEffectRenderer* Create(Graphics* graphics);

		/**
			@brief	描画処理をコマンドとして出力する。
		*/
		void ExportRenderingCommands(RenderingCommandHelper* helper, std::shared_ptr<Material2DCommand> command);

		/**
			@brief	マテリアルを用いてテクスチャに画像を描画する。
			@param	command	描画命令
		*/
		void DrawOnTexture2DWithMaterialWithCommand(std::shared_ptr<Material2DCommand> command);

		/**
		@brief	シェーダーを用いてテクスチャに画像を描画する。
		*/
		void DrawOnTexture2DWithNativeShader(RenderTexture2D_Imp* target, NativeShader_Imp* shader, ShaderConstantValue* constantValues, int32_t constantValueCount);
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}