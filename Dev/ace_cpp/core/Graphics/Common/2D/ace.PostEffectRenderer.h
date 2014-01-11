
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>

#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Vector2DI.h>
#include <Graphics/ace.Color.h>

#include "../../../ace.Core.Base.h"
#include "../../../ace.ReferenceObject.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
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
			Color		Color_;

			Vertex() {}
			Vertex(ace::Vector3DF pos, ace::Vector2DF uv)
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
			@brief	マテリアルを用いてテクスチャに画像を描画する。
			@param	command	描画命令
		*/
		void DrawOnTexture2DWithMaterialWithCommand(std::shared_ptr<Material2DCommand> command);
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}