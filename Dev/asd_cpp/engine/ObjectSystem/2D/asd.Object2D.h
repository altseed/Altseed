#pragma once
#include <memory>
#include <list>
#include <functional>
#include "../../asd.CoreToEngine.h"
#include "../Component/asd.Object2DComponent.h"
#include "../Component/asd.ComponentManager.h"
#include "../asd.ContentsManager.h"
#include "../../Shape/asd.Shape.h"

namespace asd
{
	class Layer2D;

	/**
		@brief	画面上に描画される2Dオブジェクトを表すクラス。
	*/
	class Object2D
	{
		friend class Layer2D;
		friend class ContentsManager < Object2D > ;
		friend class DrawnObject2D;

	public:
		typedef std::shared_ptr<Object2D> Ptr;

	private:
		class ParentInfo2D
		{
		private:
			Object2D* m_parent;
			ChildManagementMode::Flags m_managementMode;

		public:
			typedef std::shared_ptr<ParentInfo2D> Ptr;

			ParentInfo2D(Object2D* parent, ChildManagementMode::Flags managementMode)
				: m_parent(parent)
				, m_managementMode(managementMode)
			{
			}

			Object2D* GetParent() const
			{
				return m_parent;
			}

			ChildManagementMode::Flags GetManagementMode() const
			{
				return m_managementMode;
			}
		};

		Layer2D* m_owner;
		ParentInfo2D::Ptr m_parentInfo;
		std::list<Object2D::Ptr> m_children;
		ComponentManager<Object2D, Object2DComponent> m_componentManager;
		bool m_isUpdated;
		bool m_isDrawn;
		int m_updatePriority;
		std::function<void(int)> m_onUpdatePriorityChanged;

		void RaiseOnAdded();
		void RaiseOnRemoved();
		void Update();
		void DrawAdditionally();
		void SetLayer(Layer2D* layer);
		virtual CoreObject2D* GetCoreObject() const = 0;

	protected:

		/**
			@brief	オーバーライドして、この2Dオブジェクトがレイヤーに登録されたときの処理を記述できる。
		*/
		virtual void OnAdded();
		/**
			@brief	オーバーライドして、この2Dオブジェクトがレイヤーから登録解除されたときの処理を記述できる。
		*/
		virtual void OnRemoved();
		/**
			@brief	オーバーライドして、この2Dオブジェクトが破棄される際の処理を記述できる。
		*/
		virtual void OnDispose();		
		/**
			@brief	オーバーライドして、このオブジェクトの更新処理を記述できる。
		*/
		virtual void OnUpdate();
		/**
			@brief	オーバーライドして、このオブジェクトに対する追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally();


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
			std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

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

	public:
		/**
			@brief	コンストラクタ
		*/
		Object2D();
		/**
			@brief	デストラクタ
		*/
		virtual ~Object2D();

		/**
			@brief	カメラに表示するグループを取得する。
			@return	グループ
			@note
			カメラのグループと他のオブジェクトのグループで&でビット演算して1以上だった場合、そのカメラにオブジェクトが描画されるようになる。

		*/
		int32_t GetCameraGroup() const;

		/**
		@brief	カメラに表示するグループを設定する。
		@param	group	グループ
		*/
		void SetCameraGroup(int32_t group);

		/**
			@brief	このオブジェクトが更新されるかどうかを取得する。
		*/
		bool GetIsUpdated() const;
		/**
			@brief	このオブジェクトが更新されるかどうかを設定する。
		*/
		void SetIsUpdated(bool value);

		/**
			@brief	このオブジェクトが描画されるかどうかを取得する。
		*/
		bool GetIsDrawn() const;

		/**
			@brief	このオブジェクトが描画されるかどうかを設定する。
		*/
		void SetIsDrawn(bool value);

		/**
			@brief	このオブジェクトが実行中かどうかを取得する。Vanishメソッドによって破棄された時に false を返す。
		*/
		bool GetIsAlive() const;

		/**
			@brief	このオブジェクトを破棄する。
		*/
		void Dispose();

		/**
			@brief	このオブジェクトを保持しているレイヤーを取得する。
		*/
		Layer2D* GetLayer() const;

		/**
			@brief	指定した2Dオブジェクトを子オブジェクトとしてこのインスタンスに追加する。
			@param	child	追加する子オブジェクト
			@param	managementMode	子オブジェクトの管理に関する同期設定。フラグをOR演算でつなげて複数指定することができる。
			@param	transformingmode	子オブジェクトの変形に関する同期設定。
		*/
		void AddChild(const Object2D::Ptr& child, ChildManagementMode::Flags managementMode, ChildTransformingMode transformingMode);
		/**
			@brief	指定した子オブジェクトをこのインスタンスから削除する。
			@param	child	削除する子オブジェクト
		*/
		void RemoveChild(const Object2D::Ptr& child);
		/**
			@brief	このオブジェクトが保持している子オブジェクトを含むコンテナを取得する。
		*/
		const std::list<Object2D::Ptr>& GetChildren() const;
		/**
			@brief	このオブジェクトの親オブジェクトを取得する。親がいなければnullptrを返す。
		*/
		Object2D* GetParent() const;

		/**
			@brief	指定したコンポーネントをこのインスタンスに追加する。
			@param	component	追加するコンポーネント
			@param	key			コンポーネントに関連付けるキー
		*/
		void AddComponent(const Object2DComponent::Ptr& component, astring key);
		/**
			@brief	指定したキーを持つコンポーネントを取得する。
			@param	key		取得するコンポーネントを示すキー
		*/
		const Object2DComponent::Ptr& GetComponent(astring key);
		/**
			@brief	指定したコンポーネントを削除する。
			@param	key		削除するコンポーネントを示すキー
		*/
		bool RemoveComponent(astring key);


		/**
			@brief	このオブジェクトを描画する位置を取得する。
		*/
		Vector2DF GetPosition() const;
		/**
			@brief	このオブジェクトを描画する位置を設定する。
		*/
		void SetPosition(Vector2DF position);
		/**
			@brief	このオブジェクトを描画する実際の位置を取得する。親子関係がある場合に、親の位置を考慮した値を取得できる。
		*/
		Vector2DF GetGlobalPosition();

		/**
			@brief	このオブジェクトを描画する際の回転角度を取得する。
			@note
			回転角度は度で表記され、回転方向は時計回りである。
		*/
		float GetAngle() const;
		/**
			@brief	このオブジェクトを描画する際の回転角度を設定する。
			@note
			回転角度は度で表記され、回転方向は時計回りである。
		*/
		void SetAngle(float value);

		/**
			@brief	このオブジェクトを描画する際の拡大率を取得する。
		*/
		Vector2DF GetScale() const;
		/**
			@brief	このオブジェクトを描画する際の拡大率を設定する。
		*/
		void SetScale(Vector2DF value);

		/**
			@brief	このオブジェクトの更新の優先順位を取得する。
		*/
		int GetUpdatePriority() const;
		/**
			@brief	このオブジェクトの更新の優先順位を設定する。
		*/
		void SetUpdatePriority(int value);

		/**
			@brief	このオブジェクトが親子関係を考慮して最終的に更新されるかどうかの真偽値を取得します。
		*/
		bool GetAbsoluteBeingUpdated() const;
		/**
			@brief	このオブジェクトが親子関係を考慮して最終的に描画されるかどうかの真偽値を取得します。
		*/
		bool GetAbsoluteBeingDrawn() const;
	};
}