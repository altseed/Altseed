
#pragma once

#include <ace.common.Base.h>

#include <Math/ace.Vector3DF.h>
#include <Math/ace.Vector4DF.h>
#include <Math/ace.Matrix44.h>
#include <Math/ace.RectF.h>

#include "../../../ace.Core.Base.h"
#include "../../../ace.Core.Base_Imp.h"

namespace ace {
	class SpriteRenderer3D
	{
	private:
		/**
			@brief	スプライト描画時に使用する頂点
		*/
		struct SpriteVertex
		{
			Vector3DF	Position;
			Vector2DF	UV;
			Color		Color_;
		};

		struct Sprite
		{
			Vector3DF	Positions[4];
			Color		Colors[4];
			Vector2DF	UV[4];

			Texture2D*	TexturePtr;
			AlphaBlend	AlphaBlendState;
		};

	private:
		/**
			@brief	最大同時スプライト描画数
		*/
		static const int32_t SpriteCount = 1024;

		/**
			@brief	現在描画しているスプライトの状態
		*/
		struct SpriteState
		{
			Texture2D*	TexturePtr;
			AlphaBlend	AlphaBlendState;
		} m_state;

		std::vector<Sprite> sprites;
		std::vector<Sprite*>	drawingSprites;

		Graphics_Imp*	m_graphics;

		std::shared_ptr<ace::VertexBuffer_Imp>	m_vertexBuffer;
		std::shared_ptr<ace::IndexBuffer_Imp>	m_indexBuffer;

		/**
			@brief	テクスチャありのスプライトシェーダー
		*/
		std::shared_ptr<ace::NativeShader_Imp>		m_shader;

		/**
		@brief	テクスチャなしのスプライトシェーダー
		*/
		std::shared_ptr<ace::NativeShader_Imp>		m_shader_nt;

		Matrix44	cameraMatrix;
		Matrix44	projectionMatrix;

	public:
		SpriteRenderer3D(Graphics* graphics);
		virtual ~SpriteRenderer3D();

		void DrawCache();

		void ClearCache();

		void AddSprite(Vector3DF positions[4], Color colors[4], Vector2DF uv[4], Texture2D* texture, AlphaBlend alphaBlend);

		void SetMatrixes(Matrix44 cameraMatrix, Matrix44 projectionMatrix);

	private:
		void StartDrawing();

		void Draw(Sprite& s);

		void EndDrawing();

		void DrawSprite();
	};
}
