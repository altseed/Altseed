
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Renderer2D.h"

#include "../../../ace.Core.Base_Imp.h"
#include "../../../ace.ReferenceObject.h"

#include <Math/ace.Vector3DF.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	レイヤーの描画を管理するクラス
	@note
	内部で管理しているスプライトは全て、[左上、右上、右下、左下]の順番で情報を格納する。
	*/
	class LayerRenderer
		: public ReferenceObject
	{
	private:
		/**
		@brief	トライアングル描画時に使用する頂点
		*/
		struct TriangleVertex
		{
			Vector3DF	Position;
			Vector2DF	UV;
			Color		Col;
		};

		/**
		@brief	トライアングル描画時に使用する定数バッファ
		*/
		struct TriangleConstantBuffer
		{
			Vector2DF	Size;
			float		Padding[2];
			Vector2DF	UL;
			Vector2DF	UR;
			Vector2DF	LR;
			Vector2DF	LL;
		};

	private:
		/**
		@brief	最大同時トライアングル描画数
		*/
		static const int32_t TriangleCount = 1024;

		/**
		@brief	描画中の頂点
		*/
		std::vector <TriangleVertex>	m_vertecies;

		Graphics_Imp*	m_graphics;
		Vector2DI		m_windowSize;
		Vector2DF		m_layerPosition[4];
		Texture2D*		m_texture;

		std::shared_ptr<ace::VertexBuffer_Imp>	m_vertexBuffer;
		std::shared_ptr<ace::IndexBuffer_Imp>	m_indexBuffer;

		std::shared_ptr<ace::NativeShader_Imp>		m_shader;

	public:
		LayerRenderer(Graphics* graphics);

	private:
		virtual ~LayerRenderer();

	public:

		void DrawCache();

		void ClearCache();

		/**
		@brief	三角形をキャッシュに追加する。
		@param	positions	頂点座標(-1.0～1.0)
		@param	colors		頂点色
		@param	uv			頂点UV
		*/
		void AddTriangle(Vector2DF positions[3], Color colors[3], Vector2DF uv[3]);

		void SetWindowSize(Vector2DI windowSize);

		/**
			@brief	レイヤーの座標を指定する。
			@param	layerPosition	レイヤーの左上、右上、右下、左下のスクリーン上の座標
		*/
		void SetLayerPosition(Vector2DF layerPosition[4]);

		void SetTexture(Texture2D* texture);
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
