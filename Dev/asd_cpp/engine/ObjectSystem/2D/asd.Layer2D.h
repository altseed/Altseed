#pragma once
#include <memory>
#include <list>
#include <map>
#include "asd.Object2D.h"
#include "../../asd.CoreToEngine.h"
#include "../asd.Layer.h"
#include "../Component/asd.Layer2DComponent.h"
#include "../Component/asd.ComponentManager.h"
#include "../../Shape/asd.Shape.h"

namespace asd
{
	/**
		@brief	2Dオブジェクトの更新と描画を管理するレイヤーの機能を提供する抽象クラス。
	*/
	class Layer2D : public Layer
	{
		friend class Scene;

	public:
		typedef std::shared_ptr<Layer2D> Ptr;

	private:
		std::shared_ptr<CoreLayer2D>	m_coreLayer;
		ContentsManager<Object2D> m_objects;
		ComponentManager<Layer2D, Layer2DComponent> m_components;

		void BeginUpdating();
		void EndUpdateting();

		void DrawAdditionally();
		void UpdateInternal();
		void Dispose();

	protected:

	public:
		/**
			@brief	コンストラクタ
		*/
		Layer2D();
		/**
			@brief	デストラクタ
		*/
		virtual ~Layer2D();

		/**
			@brief	指定した2Dオブジェクトをこのインスタンスに追加する。
			@param	object	追加するオブジェクト
		*/
		void AddObject(const Object2D::Ptr& object);
		/**
			@brief	指定した2Dオブジェクトをこのインスタンスから削除する。
			@param	object	削除するオブジェクト
		*/
		void RemoveObject(const Object2D::Ptr& object);
		/**
			@brief	このレイヤーに登録されている2Dオブジェクトのリストを取得する。
			@return	登録されているオブジェクトのリスト
		*/
		const std::list<Object2D::Ptr>& GetObjects() const;

		/**
		@brief	レイヤーが所持する全ての2Dオブジェクトをこのインスタンスから削除する。
		*/
		void Clear();

		/**
			@brief	指定したコンポーネントをこのインスタンスに追加する。
			@param	component	追加するコンポーネント
			@param	key			コンポーネントに関連付けるキー
		*/
		void AddComponent(const Layer2DComponent::Ptr& component, astring key);

		/**
			@brief	指定したキーを持つコンポーネントを取得する。
			@param	key		取得するコンポーネントを示すキー
		*/
		const Layer2DComponent::Ptr& GetComponent(astring key);

		/**
			@brief	指定したコンポーネントをこのインスタンスから削除する。
			@param	key		削除するコンポーネントを示すキー
		*/
		bool RemoveComponent(astring key);

		/**
			@brief	通常の描画に加えてテクスチャを描画する。
			@param	upperLeftPos	テクスチャの左上の描画位置
			@param	upperRightPos	テクスチャの右上の描画位置
			@param	lowerRightPos	テクスチャの右下の描画位置
			@param	lowerLeftPos	テクスチャの左下の描画位置
			@param	upperLeftCol	テクスチャの左上の頂点色
			@param	upperRightCol	テクスチャの右上の頂点色
			@param	lowerRightCol	テクスチャの右下の頂点色
			@param	lowerLeftCol	テクスチャの左下の頂点色
			@param	upperLeftUV		テクスチャの左上のUV値
			@param	upperRightUV	テクスチャの右上のUV値
			@param	lowerRightUV	テクスチャの右下のUV値
			@param	lowerLeftUV		テクスチャの左下のUV値
			@param	texture			描画するテクスチャ
			@param	alphaBlend		アルファブレンドの種類
			@param	priority		描画の優先順位(大きいほど前面に描画される)
			@note
			OnDrawAdditionallyの中以外では実行してはいけない。
		*/
		void DrawSpriteAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			std::shared_ptr<Texture2D>  texture, AlphaBlendMode alphaBlend, int32_t priority);

		/**
			@brief	通常の描画に加えて文字列を描画する。
			@param	pos					描画位置
			@param	color				頂点色
			@param	font				フォント
			@param	text				文字列
			@param	writingDirection	行方向
			@param	alphaBlend		アルファブレンドの種類
			@param	priority		描画の優先順位(大きいほど前面に描画される)
			@note
			OnDrawAdditionallyの中以外では実行してはいけない。
		*/
		void DrawTextAdditionally(Vector2DF pos, Color color, std::shared_ptr<Font> font, const achar* text, WritingDirection writingDirection, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawRectangleAdditionally(RectF drawingArea, Color color, RectF uv, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawRotatedRectangleAdditionally(RectF drawingArea, Color color, Vector2DF rotationCenter, float angle, RectF uv, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawTriangleAdditionally(Vector2DF position1, Vector2DF position2, Vector2DF position3, Color color, Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawCircleAdditionally(asd::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, float angle, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawArcAdditionally(asd::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, int startingVerticalAngle, int endingVerticalAngle, float angle, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawLineAdditionally(Vector2DF point1, Vector2DF point2, float thickness, Color color, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawShapeAdditionally(std::shared_ptr<Shape> shape, Color color, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		/**
		@brief	レイヤーの種類を取得する。
		@return	レイヤーの種類
		*/
		LayerType GetLayerType() const override { return LayerType::Layer2D; }

		int GetObjectCount() const;
	};
}