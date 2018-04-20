#pragma once
#include <memory>
#include <list>
#include <map>
#include "../../asd.CoreToEngine.h"
#include "../asd.Layer.h"
#include "../../Shape/asd.Shape.h"
#include "asd.Object2D.h"

#include "../Component/asd.Layer2DComponent.h"
#include "../asd.ObjectManager.h"
#include "../Component/asd.ComponentManager.h"
#include "../Registration/asd.IObjectRegisterable.h"
#include "../Registration/asd.IComponentRegisterable.h"

namespace asd
{
	template<typename TObject> class ObjectManager;
	template<typename TComponent> class ComponentManager;

	/**
		@brief	2Dオブジェクトの更新と描画を管理するレイヤーの機能を提供する抽象クラス。
	*/
	class Layer2D
		: public Layer
		, public IObjectRegisterable<Object2D>
		, public IComponentRegisterable<Layer2DComponent>
	{
		friend class Scene;
		friend class Object2D;
		friend class Layer2DComponent;

	public:
		typedef std::shared_ptr<Layer2D> Ptr;

	private:
		std::shared_ptr<CoreLayer2D>	m_coreLayer;
		std::shared_ptr<ObjectManager<Object2D>> m_objects;
		std::shared_ptr<ComponentManager<Layer2DComponent>> m_components;
		std::unordered_map<int64_t, std::shared_ptr<Collider2D>> collidersMap;

		void BeginUpdating() override;
		void EndUpdateting() override;
		void UpdateInternal() override;
		void DrawAdditionally() override;
		void DisposeInternal() override;

		void ImmediatelyRemoveObject(const Object2D::Ptr& object, bool raiseEvent);
		void ImmediatelyRemoveComponent(astring key);
		void Register(const Object2D::Ptr& object);
		void Unregister(const Object2D::Ptr& object);
		void AddToCore(const Object2D::Ptr& object) override;
		void RemoveFromCore(const Object2D::Ptr& object) override;
		void Register(const Layer2DComponent::Ptr& component) override;
		void Unregister(const Layer2DComponent::Ptr& component) override;

		void RaiseOnAdded() override;
		void RaiseOnRemoved() override;

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

		bool GetIsAlive() const;

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
		std::list<Object2D::Ptr> GetObjects() const;

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
		Layer2DComponent::Ptr GetComponent(astring key);

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
		void DrawSpriteAdditionally(
			const Vector2DF& upperLeftPos, const Vector2DF& upperRightPos, const Vector2DF& lowerRightPos, const Vector2DF& lowerLeftPos,
			const Color& upperLeftCol, const Color& upperRightCol, const Color& lowerRightCol, const Color& lowerLeftCol,
			const Vector2DF& upperLeftUV, const Vector2DF& upperRightUV, const Vector2DF& lowerRightUV, const Vector2DF& lowerLeftUV,
			std::shared_ptr<Texture2D>  texture, AlphaBlendMode alphaBlend, int32_t priority);

		/**
		@brief	通常の描画に加えて専用のシェーダーを用いて2Dスプライトを描画する。
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
		@param	material		表示に使用するマテリアル
		@param	alphaBlend		アルファブレンドの種類
		@param	priority		描画の優先順位(大きいほど前面に描画される)
		@note
		専用のシェーダーを用いてスプライトを描画する。
		OnDrawAdditionallyの中以外では実行してはいけない。
		*/
		void DrawSpriteWithMaterialAdditionally(
			const Vector2DF& upperLeftPos, const Vector2DF& upperRightPos, const Vector2DF& lowerRightPos, const Vector2DF& lowerLeftPos,
			const Color& upperLeftCol, const Color& upperRightCol, const Color& lowerRightCol, const Color& lowerLeftCol,
			const Vector2DF& upperLeftUV, const Vector2DF& upperRightUV, const Vector2DF& lowerRightUV, const Vector2DF& lowerLeftUV,
			std::shared_ptr<Material2D> material, AlphaBlendMode alphaBlend, int32_t priority);

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

		void DrawRectangleAdditionally(const RectF& drawingArea, const Color& color, const RectF& uv, const std::shared_ptr<Texture2D>& texture, AlphaBlendMode alphaBlend, int32_t priority);

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

		/**
			@brief	エフェクトの歪みが有効かどうか、取得する。
			@return	有効/無効
		*/
		bool GetIsDistortionEnabled() const;

		/**
			@brief	エフェクトの歪みが有効かどうか、設定する。
			@param	value	有効/無効
		*/
		void SetIsDistortionEnabled(bool value);

	};
}